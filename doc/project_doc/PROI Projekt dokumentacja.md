# Projekt PROI semestr 22L
Rafał Budnik
Michał Jakomulski
Tomasz Owienko 

---

## Spis treści

1. Założenia projektowe
    1. Cel projektu
    2. Plan realizacji
    3. Zastosowane technologie
2. Metodyka pracy
    1. Organizacja kodu
    2. Testy jednostkowe
3. Implementacja
    1. Struktura projektu
    2. Działanie gry
        1. System zdarzeń
        2. Pętla zdarzeń
        3. Obsługa planszy
        4. Moduł graficzny
4. Użytkowanie
5. Uwagi
    1. Nietrywialne rozwiązania programistyczne
    2. Możliwe dalsze modyfikacje

---

# 1. Założenia projektowe

## 1.1 Cel projektu
Celem projektu była implementacja gry zręcznościowej wzorowanej na tytule *Battle City* wydanej przez firmę *Namco* na platformę *Famicom*:

![](Attachments/Pasted%20image%2020220606201335.png)


## 1.2 Plan realizacji
Założono liniową realizację projektu zgodnie z metodyką *waterfall* wg następującego planu:
1. Implementacja kluczowych modułów składających się na silnik gry, m.in.:
	- Zegar gry
	- System zdarzeń
	- Re-używalne implementacje wybranych modeli interakcji między obiektami
2. Opracowanie podstawowych komponentów obsługujących logikę planszy
	- Plansza
	- Płytki
	- Byty (czołgi oraz pociski)
3. Rozpoczęcie prac nad modułem graficznym
4. Implementacja interakcji między elementami i systemu generowania zdarzeń
5. Wydzielenie stanów gry i powiązanie z nimi metod obsługi generowanych zdarzeń
6. Opracowanie algorytmu sterującego “wrogimi” czołgami
7. Integracja modułu graficznego z logiką gry


## 1.3 Zastosowane technologie
Gra zaimplementowana została w języku C++ w standardzie C++20. Moduł graficzny oparty został na bibliotece SFML, a testy jednostkowe utworzono przy użyciu frameworku Catch2.

---

# 2. Metodyka pracy

## 2.1 Organizacja kodu
Do przechowywania kodu źródłowego projektu wykorzystano platformę GitLab, a repozytorium zorganizowano w ramach modelu GitHub Flow:

![](Attachments/Pasted%20image%2020220606230115.png)

Pliki źródłowe projektu zorganizowano wg. następującej struktury:
```
proi-projekt  
   ├ bin
   │  └ lib  
   ├ build  
   ├ doc  
   │  ├ html  
   │  └ rtf  
   └ src     
	  ├ <name>-lib    
	  ...     
	  └ main
```


## 2.2 Testy jednostkowe
Do testów jednostkowych wykorzystano framework Catch2 w wersji `>=v3.0`, dzięki czemu nie było konieczne korzystanie z pliku nagłówkowego `catch.hpp` i każdorazowe kompilowanie go. 
Wszystkie testy zostały napisane w oparciu o zaczerpniętą z *behavior-driven development* strukturę *Given-When-Then*, co zwiększyło ich czytelność przy jednoczesnym zmniejszeniu objętości kodu każdego z testów.

---

# 3. Implementacja

## 3.1 Struktura projektu
Projekt został podzielony na sześć bibliotek oraz moduł główny, a zależności między nimi opisane są na poniższym diagramie:

![](Attachments/Pasted%20image%2020220606224102.png)

## 3.2 Działanie gry
### 3.2.1 System zdarzeń
W celu uniknięcia zbytecznego silnego wiązania obiektów, przyjęto założenie, że komunikacja między obiektami należącymi do różnych podsystemów będzie przebiegała za pośrednictwem globalnej (będącej *singletonem*) kolejki zdarzeń - zmiana stanu obiektu sygnalizowana jest utworzeniem instancji klasy zdarzenia, która następnie może zostać obsłużona za pomocą obiektów logicznie niepowiązanych z tymi, które zdarzenia wygenerowały.
Ponadto, sposób obsługi zdarzeń może się różnić w zależności od stanu gry - zaimplementowane zostały klasy `GameState` kapsułkujące stan gry (w obecnej wersji projektu wspierane są cztery stany - menu, aktywny, pauza i zakończony) oraz klasy `EventHandler` kapsułkujące sposoby obsługi zdarzeń - są one ściśle powiązane z obiektami stanu gry. Metoda służąca do obsługi zdarzeń (`EventHandler::HandleEvent()`) może być wołana w sposób polimorficzny (na wskazaniu na obiekt `GameState` obecnym wewnątrz głównego kontenera `Game`).

### 3.2.2 Pętla zdarzeń
Zostało założone, że po inicjalizacji gry logika przetwarzana będzie w pętli zbudowanej w następujący sposób:
```
dopóki(gra_działa){
	wyzwól_zdarzenia_zegara();
	dopóki(kolejka_zdarzeń_nie_jest_puste){
		obsłuż_pierwsze_zdarzenie();
	}

	przesuń_wszystkie_byty();
	przerysuj_interfejs();

	uśpij_zegar();
}
```

W każdym cyklu zegara powiadamiani są jego subskrybenci (m.in. `KeyboardController`, `Bot` i `BotController`), którzy kolejkują własne zdarzenia. Następnie zdarzenia pobierane są z kolejki obsługiwane są jedno po drugim, tak długo, aż kolejka zostanie opróżniona (obsługa zdarzeń może generować nowe zdarzenia).
Po obsłużeniu zdarzeń (w tym m.in. ustawieniu flag `moving` obiektów `Tank`) na każdym bycie na planszy wywoływana jest metoda `move()`, a następnie przerysowywane są reprezentacje graficzne obiektów oraz składowe interfejsu.
Na koniec gra jest usypiana, aż upłynie $\frac{1}{60}s$ od poprzedniego powiadomienia subskrybentów zegara.

### 3.2.3 Obsługa planszy
Klasa Board i podsystem planszy, czołgi, boty, kolizje, ładowanie planszy z pliku

Podsystem planszy zbudowany jest zgodnie z poniższym diagramem:

![](Attachments/Pasted%20image%2020220608104801.png)

Klasa `Board` będąca mediatorem podsystemu przechowuje w sobie obiekty `Grid` (płytki na planszy) i `EntityController` (byty na planszy). `Board` udostępnia interfejs pozwalający na manipulację obiektami `Grid` i `Entity`.
Zgodnie z założeniami wzorca *mediator*, cała interakcja pomiędzy obiektami podsystemu odbywa się za pośrednictwem klasy `Board`. W związku z tym, klasa `Board` jako jedyna potrafi wykonywać operacje związane z detekcją kolizji między obiektami.
Obiekty `Grid` tworzone są na podstawie plików tekstowych zawierające graficzne reprezentacje poziomów. Przykładowy plik:
![](Attachments/Pasted%20image%2020220608110559.png)
( … )

Pojedyncze znaki w pliku reprezentują różne rodzaje płytek na planszy, oraz punkty na mapie o specjalnym przeznaczeniu. Przykładowo, ‘B’ symbolizuje płytkę reprezentującą ceglaną ścianę, a ‘\*’ to punkt, w którym mogą zostać utworzone obiekty wrogich czołgów.
W celu oszczędności pamięci zastosowano wzorzec *pyłek*, reprezentując poszczególne płytki jedynie z pomocą wartości `enum`, bez konieczności tworzenia do $52^2$ obiektów. Dostęp do cech poszczególnych płytek odbywa się za pośrednictwem statycznych metod obiektu `TileManager` (nieuwzględniony na diagramie).
Poza płytkami, w obiekcie przechowywane są także dodatkowe informacje pobrane z pliku (miejsca tworzenia wrogich czołgów, miejsce tworzenia czołgu gracza, lokalizacja orzełka)).

Obiekty niebędące płytkami reprezentowane są za pomocą klas potomnych względem `Entity`. Klasy takie posiadają następujące pola:
- Pozycja w osi X
- Pozycja w osi Y
- Rozmiar w osi X
- Rozmiar w osi Y
- Prędkość ruchu
- Kierunek, w którym obiekt jest skierowany
Oraz posiadają metody czysto wirtualne `move()` i `moveBack()`, które pozwalają na przesuwanie obiektów o określoną odległość w sposób polimorficzny.

Jedną z klas potomnych po `Entity` jest klasa `Tank`, która dodatkowo definiuje swoją liczbę żyć, prędkość tworzonego pocisku, oraz liczbę punktów będących nagrodą za zniszczenie czołgu. 
Dla każdego czołgu dozwolone jest istnienie jednego pocisku utworzonego przez dany czołg w danym momencie. Własność ta jest weryfikowana przez zasubskrybowanie utworzonego pocisku do czołgu w momencie wywołania metody `createBullet()`. Jeśli czołg jest już subskrybowany przez  jakiś pocisk, funkcja kończy działanie i nie tworzy obiektu `Bullet`. W momencie zniszczenia obiektu `Bullet` cofa on subskrybcję `Tank`, w wyniku czego ponowne utworzenie pocisku jest możliwe.

Obiekty klas potomnych względem `Tank` (innych niż `PlayerTank`) dodatkowo dziedziczą wirtualnie po klasie `Bot`, co skutkuje przekształceniem ich w zdolne do samodzielnego poruszania się czołgi. 
Obiekty klasy `Bot` są *subskrybentami* i w momencie utworzenia subskrybują do globalnego zegara. Z każdym otrzymaniem powiadomienia `Bot` dekrementuje swój wewnętrzny licznik `decisionCooldown`, a gdy osiągnie on $0$, tworzy zdarzenie sygnalizujące konieczność podjęcia przez bota decyzji odnośnie jego następnego ruchu (możliwy jest ruch prosto, obrót o $+/-90$ stopni, oraz wystrzelenie pocisku). Z obiektu zdarzenia ekstraktowany jest obiekt czołgu i przekazywany do obiektu `BotController`, który następnie generuje zdarzenie kapsułkujące podjętą decyzję.

### 3.2.4 Moduł graficzny
Moduł graficzny działa na zasadzie *kompozytu*. Prawie wszystkie jego klasy dziedziczą po klasie bazowej *AbstrctWindow*.
"Korzeniem" kompozytu jest klasa Window, wywołana na jej obiekcie metoda render wywołuję tą metodą we właściwych gałęziach kompozytu.
Kompozyt jest podzielony na na dwie główne gałęzi *ActiveStateGraphic* i *StaticStateGraphic*.
*ActiveStateGrafic* odpowiada za obsługę stanu aktywnego gry, natomiast *StaticStateGrafic* zajmuje się resztą stanów.

*ActiveStateGrafic* rozgałęzia się na *BoardGrafic*, odpowiedzialne za rysowanie planszy i *FrameGrafic* odpowiedzialne za ramkę okalającą planszę
i występujące na niej napisy. Każdy z rodzajów obiektów planszy (*BoardGrafic*) (Czołgi, Kafelki, Pociski, Orzełek) posiada swoją własną klasę, która
odpowiada za jego rysowanie na ekranie.

*StaticStateGrafic* składa się z klas *StaticGraphic* i *MenuStateGraphic*. *StaticGraphic* ustala ustawinie elementów, a *MenuStateGraphic* rysuje je
z odpowiednimi napisami, w zależności od stanu gry (*MenuState*, *PauseState*, *FinishedState*).

Moduł graficzny przechowuje informacje o obiektach do narysowania, za pomocą shared pointerów. Umieszcza on wskaźniki zwrócone przez eventy gry,
w swoich własnych wskaźnikach. Dzięki temu nie ma potrzeby pisania metod aktualizujących kompozyt, wszystkie wskaźniki na elementy gry są zaalokowane 
w shared pointerach modułu graficznego.

Zarządza kompozytem klasa *GraficEventHandler*, która obsługuje eventy tworzone przez resztę bibliotek.

---
# 4. Użytkowanie
Gra przeznaczona jest do kompilacji z użyciem systemu CMake:

```bash
# katalog /bin
cmake .
cmake --build ../build --target tanks --clean-first
```

Pliki wykonywalne powinny zostać umieszczone w katalogu `\bin`.
Do poprawnej kompilacji wymagane jest spełnienie zależności w postaci bibliotek Catch2 (v≥v3.0) i SFML.

Aby program działał poprawnie, podczas jego uruchamiania w katalogu roboczym powinien znajdować się katalog `levels` zawierający tekstowe reprezentacje poziomów.
Uruchamianie gry:
```bash
./tanks
```
Sterowanie w grze odbywa się za pomocą strzałek, zatwierdzanie wyboru przy użyciu klawisza enter, a wystrzelenie pocisku z czołgu odbywa się po wciśnięciu spacji.

---

# 5. Uwagi

### 5.1 Nietrywialne rozwiązania programistyczne
- Zastosowanie wzorców projektowych:
	- *Singleton* (np. *Clock*)
	- *Pyłek* (przechowywanie płytek)
	- *Stan* (stany gry)
	- *Kompozyt* (biblioteka graficzna)
	- *Metoda wytwórcza* (*GridBuilder*)
	- *Mediator* (podsystem planszy)
- Skorzystanie z globalnej kolejki zdarzeń

## 5.2 Możliwe dalsze modyfikacje
- Docelowa implementacja systemu ładowania poziomów i zapisywania wyników - szkielety wymaganych klas są już napisane i zintegrowane z grą
- Refaktoryzacja klasy `Event` - zastosowanie `std::variant` zamiast `union`
- Optymalizacja wydajności modułu graficznego
- Obsługa niepoprawnego wejścia z pliku podczas ładowania planszy
