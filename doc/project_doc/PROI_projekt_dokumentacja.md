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
   2. Testy
3. Implementacja
   1. Struktura projektu
   2. Działanie gry
	   1. Ogólna zasada działania
       2. System zdarzeń
       3. Pętla zdarzeń
       4. Obsługa planszy
       5. Moduł graficzny
4. Użytkowanie
5. Uwagi
	1.  Napotkane problemy oraz możliwe usprawnienia w kodzie
    2. Zastosowane nietrywialne rozwiązania programistyczne
    3. Możliwości rozbudowy projektu
    4. Dokumentacja kodu

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
   │  └ lib   ├ build
   ├ doc
   │  ├ html
   │  └ rtf
   └ src
     ├ <name>-lib
     ...
     └ main
```


## 2.2 Testy
Do testów jednostkowych wykorzystano framework Catch2 w wersji `>=v3.0`, dzięki czemu nie było konieczne korzystanie z pliku nagłówkowego `catch.hpp` i każdorazowe kompilowanie go.
Wszystkie testy zostały napisane w oparciu o zaczerpniętą z metody *Behaviour-Driven Development* strukturę *Given-When-Then*, co zwiększyło ich czytelność przy jednoczesnym zmniejszeniu objętości kodu każdego z testów.

---

# 3. Implementacja

## 3.1 Struktura projektu
Projekt został podzielony na sześć bibliotek oraz moduł główny, a zależności między nimi opisane są na poniższym diagramie:

![](Attachments/Pasted%20image%2020220606224102.png)

- `core-lib` - podstawowe komponenty systemu, niepowiązane stricte z samą grą (m.in implementacja zegara czy kolejki zdarzeń)
- `tank-lib` - reprezentacje obiektów na planszy oraz mechanizmy zarządzania nimi
- `board-lib` - podsystem planszy, reprezentacje płytek, mechanizm generowania planszy na podstawie pliku tekstowego
- `bot-lib` - rozszerzenie `tank-lib` o system botów, zdolnych do samodzielnego poruszanie się po planszy
- `game-lib` - wysokopoziomowe mechanizmy gry - system stanów i obsługi zdarzeń, obsługa statystyk, oraz klasa głównego kontenera `Game`
- `graphic-lib` - biblioteka graficzna zaimplementowany w oparciu o bibliotekę SFML
- `main` - moduł główny z plikiem `.cpp` kompilowanym do postaci wykonywalnej

## 3.2 Działanie gry
### 3.2.1 Ogólna zasada działania
Gra opiera się na strukturze przedstawionej na poniższym diagramie (bez uwzględniania dziedziczenia):

![](Attachments/Pasted%20image%2020220610171743.png)

Obiekt `Game` stanowi główny kontener i tym samym kontroluje pracę całej gry. Koordynuje on współpracę obiektów klas:
- `Board` - mediator podsystemu planszy
- `GameState` - kapsułkuje stan gry, posiada skojarzony obiekt `EventHandler` definiujący metody obsługi zdarzeń dla danego stanu gry
- `GameStatistics` - przechowuje aktualne statystyki gry
- `Window` - kontener agregujący elementy interfejsu graficznego oraz graficzne reprezentacje obiektów planszy
Poza obiektem `Game` istnieją także dwa globalnie dostępne *singletony* kluczowe dla działania gry:
- `Clock` - odpowiada za utrzymywanie odpowiednich odstępów czasowych pomiędzy kolejnymi cyklami przetwarzania zdarzeń w grze; wyzwala reakcje subskrybujących go obiektów w każdym cyklu swojej pracy
- `EventQueue` - globalnie dostępna kolejka zdarzeń - agreguje zdarzenia wygenerowane przez obiekty składające się na grę, aby mogły potem zostać obsłużone przez odpowiednie obiekty `EventHandler` i `GraphicEventHandler`


### 3.2.2 System zdarzeń
W celu uniknięcia zbytecznego silnego wiązania obiektów, przyjęto założenie, że komunikacja między obiektami należącymi do różnych podsystemów będzie przebiegała za pośrednictwem globalnej (zaimplementowanej jako *singleton*) kolejki zdarzeń - zmiana stanu obiektu sygnalizowana jest utworzeniem instancji klasy zdarzenia, która następnie może zostać obsłużona za pomocą obiektów logicznie niepowiązanych z tymi, które zdarzenia wygenerowały.

Ponadto, sposób obsługi zdarzeń może się różnić w zależności od stanu gry - zaimplementowane zostały klasy `GameState` kapsułkujące stan gry (w obecnej wersji projektu wspierane są cztery stany - menu, aktywny, pauza i zakończony) oraz klasy `EventHandler` kapsułkujące sposoby obsługi zdarzeń - są one ściśle powiązane z obiektami stanu gry. Metoda służąca do obsługi zdarzeń (`EventHandler::HandleEvent()`) może być wołana w sposób polimorficzny (na wskazaniu na obiekt `GameState` obecnym wewnątrz głównego kontenera `Game`).

Obsługa każdego zdarzenia przebiega wieloetapowo:
- Zdarzenie pobierane jest z kolejki zdarzeń
- Zdarzenie przekazywane jest do obiektu `EventHandler` skojarzonego z aktualnym stanem gry, który obsługuje zdarzenie w kontekście logiki gry
- Obiekt `EventHandler` zwraca zdarzenie po przetworzeniu go
- Zdarzenie przekazywane jest do obiektu `GraphicEventHandler`, który obsługuje je w kontekście graficznej reprezentacji obiektów, które zmieniły swój stan

Proces ten ilustruje poniższy diagram:

![](Attachments/Pasted%20image%2020220610174601.png)

Takie rozwiązanie pozwala na niemalże całkowitą separację i niezależny rozwój systemów generowania zdarzeń, obsługi zdarzeń w kontekście logiki gry, oraz obsługi zdarzeń w kontekście elementów graficznych.

Na obiekt zdarzenia składa się wartość typu wyliczeniowego `Event::EventType`, która służy do identyfikacji rodzaju zdarzenia, oraz unia `info_u`, która może przechowywać struktury skojarzone z konkretnymi rodzajami zdarzeń i zawierające dodatkowe informacje o nich:

![](Attachments/Pasted%20image%2020220610184950.png)
(pełen diagram dostępny jest w dokumentacji kodu projektu)

Zaletą takiego rozwiązania jest brak konieczności tworzenia nowej klasy dla każdego typu zdarzenia, co znacznie skomplikowałoby hierarchię klas, oraz ułatwione rozróżnianie typów zdarzeń (wystarczy sprawdzić wartość pola `type`). Wadą natomiast jest znacznie bardziej skomplikowany sposób tworzenia obiektów zdarzeń - klasa posiada wiele konstruktorów obsługujących różne zestawy argumentów, a każdy z nich musi weryfikować, czy argumenty są poprawne dla podanego typu `EventType`.

### 3.2.3 Pętla zdarzeń
Zostało założone, że po inicjalizacji gry logika przetwarzana będzie w pętli zbudowanej w następujący sposób:
```
dopóki(gra_działa){

   wyzwól_zdarzenia_zegara();

   dopóki(kolejka_zdarzeń_nie_jest_puste){

	   obsłuż_i_usuń_pierwsze_zdarzenie();

	   }

   przesuń_wszystkie_byty();

   przerysuj_interfejs();

   uśpij_zegar();
}
```

W każdym cyklu pracy zegara powiadamiani są jego subskrybenci (m.in. `KeyboardController`, `Bot` i `BotController`), którzy kolejkują własne zdarzenia. Następnie zdarzenia pobierane są z kolejki obsługiwane są jedno po drugim, tak długo, aż kolejka zostanie opróżniona (obsługa zdarzeń może generować nowe zdarzenia).
Po obsłużeniu zdarzeń (w tym m.in. ustawieniu flag `moving` obiektów `Tank`) na każdym bycie na planszy wywoływana jest polimorficzna metoda `move()`, a następnie przerysowywane są reprezentacje graficzne obiektów oraz składowe interfejsu.
Na koniec gra jest usypiana, aż upłynie $\frac{1}{60}s$ od ostatniego powiadomienia subskrybentów zegara.

### 3.2.4 Obsługa planszy
Podsystem planszy zbudowany jest zgodnie z poniższym diagramem:

![](Attachments/Pasted%20image%2020220608104801.png)

Klasa `Board` będąca mediatorem podsystemu przechowuje w sobie obiekty `Grid` (płytki na planszy) i `EntityController` (byty na planszy):
- Klasa `Grid` stanowi kontener dla dwuwymiarowej tablicy obiektów typu wyliczeniowego `TileType`, reprezentujących poszczególne rodzaje płytek występujących na planszy (cegły, stal, drzewa, woda, oraz `NullTile`). Atrybuty płytek są współdzielone (zgodnie ze wzorcem projektowym *pyłek*), a dostęp do nich jest możliwy poprzez odpytanie obiektu `TileManager` (nieobecny na diagramie) o dany atrybut płytki.
- Klasa `EntityController` agreguje obiekty klas potomnych względem `Entity` i manipuluje nimi. Jest zdolna do wykrywania kolizji między bytami, ale nie generuje powiązanych z nimi zdarzeń - jest to domena kontenera `Board`.

#### Kolizje na planszy

`Board` udostępnia interfejs pozwalający na manipulację obiektami `Grid` i `Entity` z zewnątrz (w szczególności obsługę zdarzeń w kontekście obiektów planszy), a każda zmiana stanu obiektów planszy sygnalizowana jest dodaniem odpowiedniego zdarzenia do globalnej kolejki . Metody interfejsu pozwalają m.in. na:
- Tworzenie nowych bytów na planszy
- Przesuwanie (oraz obracanie) bytów
- Usuwanie bytów z planszy
- Usuwanie płytek z planszy

W przypadku wykrycia kolizji podczas operacji tworzenia bądź przesuwania bytu, wygenerowane może zostać dodatkowe zdarzenie kolizji (np. operacja `.fireTank()`), bądź operacja może się nie powieźć (np. operacja `.spawnTank()`). W drugim przypadku, niepowodzenie operacji sygnalizowane jest zwróceniem przez nią odpowiedniej wartości logicznej.

Zdarzenie kolizji przechowuje w sobie bardziej złożoną strukturę danych, niż inne zdarzenia - struktura `CollisionInfo` skojarzona z typem zdarzenia `Event::EventType::Collision` zawiera w sobie dwie składowe typu `std::variant<...>`, gdzie każda z nich zawiera odpowiednią strukturę danych przechowującą informacje o uczestniku kolizji. Pomimo skomplikowania takiej struktury, jej stosowanie przynosi wymierne korzyści - do reprezentacji zdarzeń kolizji wystarczy jedna wartość wyliczeniowa `Event::EventType::Collision`, a obsługa takiego zdarzenia może odbyć się za pomocą polimorficznej metody `handleCollision(CollisionInfoType1 member1, CollisionInfoTyp2 member2)`. Taka metoda definiowana jest wielokrotnie dla różnych par struktur przechowujących informacje o członkach kolizji, a decyzja o tym, którą metodę należy wywołać, podejmowana jest w trakcie działania programu (jest to także implementacja wzorca *wizytator*):

![](Attachments/Pasted%20image%2020220610191747.png)
```cpp
case (Event::Collision): {
    Game *g = game_;
    std::visit([g](auto &&arg1, auto &&arg2)
    { handleCollision(arg1, arg2, g); },
               event->info.collisionInfo.member1,
               event->info.collisionInfo.member2);
    break;}
```

Zgodnie z założeniami wzorca *mediator*, cała interakcja pomiędzy obiektami podsystemu odbywa się za pośrednictwem klasy `Board`. W związku z tym, klasa `Board` jako jedyna potrafi wykonywać operacje związane z detekcją kolizji między obiektami.

#### Inicjalizacja planszy
Obiekty `Grid` tworzone są na podstawie plików tekstowych zawierające graficzne reprezentacje poziomów. Fragment przykładowego pliku:

![](Attachments/Pasted%20image%2020220610202453.png)

Pojedyncze znaki w pliku reprezentują różne rodzaje płytek na planszy, oraz punkty na mapie o specjalnym przeznaczeniu. Przykładowo, ‘B’ symbolizuje płytkę reprezentującą ceglaną ścianę, a ‘\*’ to punkt, w którym mogą zostać utworzone obiekty wrogich czołgów.
W celu oszczędności pamięci zastosowano wzorzec *pyłek*, reprezentując poszczególne płytki jedynie z pomocą wartości `enum`, bez konieczności tworzenia do $52^2$ obiektów. Dostęp do cech poszczególnych płytek odbywa się za pośrednictwem statycznych metod obiektu `TileManager` (nieuwzględniony na diagramie).
Poza płytkami, w obiekcie przechowywane są także dodatkowe informacje pobrane z pliku (miejsca tworzenia wrogich czołgów, miejsce tworzenia czołgu gracza, lokalizacja orzełka)).

#### Reprezentacja bytów
Obiekty niebędące płytkami reprezentowane są za pomocą klas potomnych względem `Entity`. Klasy takie posiadają następujące pola:
- Pozycja w osi X
- Pozycja w osi Y
- Rozmiar w osi X
- Rozmiar w osi Y
- Prędkość ruchu
- Kierunek, w którym obiekt jest skierowany
Oraz posiadają metody czysto wirtualne `move()` i `moveBack()`, które pozwalają na przesuwanie obiektów o określoną odległość w sposób polimorficzny.

#### Reprezentacja czołgów
Jedną z klas potomnych po `Entity` jest klasa `Tank`, która dodatkowo definiuje swoją liczbę żyć, prędkość tworzonego pocisku, oraz liczbę punktów będących nagrodą za zniszczenie czołgu.
Dla każdego czołgu dozwolone jest istnienie jednego pocisku utworzonego przez dany czołg w danym momencie. Własność ta jest weryfikowana przez zasubskrybowanie utworzonego pocisku do czołgu w momencie wywołania metody `createBullet()`. Jeśli czołg jest już subskrybowany przez  jakiś pocisk, funkcja kończy działanie i nie tworzy obiektu `Bullet`. W momencie zniszczenia obiektu `Bullet` cofa on subskrybcję `Tank`, w wyniku czego ponowne utworzenie pocisku jest możliwe.

#### Boty
Obiekty klas potomnych względem `Tank` (innych niż `PlayerTank`) dodatkowo dziedziczą wirtualnie po klasie `Bot`, co skutkuje przekształceniem ich w zdolne do samodzielnego poruszania się czołgi.
Obiekty klasy `Bot` są *subskrybentami* i w momencie utworzenia subskrybują one globalny zegar. Z każdym otrzymaniem powiadomienia `Bot` dekrementuje swój wewnętrzny licznik `decisionCooldown`, a gdy osiągnie on $0$, tworzy zdarzenie sygnalizujące konieczność podjęcia przez bota decyzji odnośnie jego następnego ruchu (możliwy jest ruch prosto, obrót o $+/-90$ stopni, oraz wystrzelenie pocisku). Z obiektu zdarzenia ekstraktowany jest obiekt czołgu i przekazywany do obiektu `BotController`, który następnie generuje zdarzenie kapsułkujące podjętą decyzję.
W podobny sposób przebiega tworzenie nowych botów - obiekt `BotController` także subskrybuje zegar, ponadto posiada wewnętrzny licznik `botSpawnCooldown`. Z każdym cyklem zegara licznik jest dekrementowany, a gdy osiągnie $0$, tworzone jest zdarzenie `BotSpawnDecision`, a licznik ustawiany jest na maksymalną wartość. W momencie utworzenia bota inkrementowany jest drugi licznik obiektu - `registeredBots`, który zlicza aktualnie znajdujące się na planszy boty. Jeśli licznik osiągnie pewną z góry ustaloną (w trakcie inicjalizacji obiektu) wartość, nowe zdarzenia `BotSpawnDecision` nie będą generowane aż do obniżenia wartości licznika. Ma to miejsce w trakcie deinicjalizacji obiektu bota.

### 3.2.5 Moduł graficzny
Moduł graficzny działa na zasadzie *kompozytu*. Prawie wszystkie jego klasy dziedziczą po klasie bazowej *AbstrctWindow*.
"Korzeniem" kompozytu jest klasa Window, wywołana na jej obiekcie metoda render wywołuję tą metodą we właściwych gałęziach kompozytu.
Kompozyt jest podzielony na na dwie główne gałęzi *ActiveStateGraphic* i *StaticStateGraphic*.
*ActiveStateGrafic* odpowiada za obsługę stanu aktywnego gry, natomiast *StaticStateGrafic* zajmuje się resztą stanów.

*ActiveStateGrafic* rozgałęzia się na *BoardGrafic*, odpowiedzialne za wyświetlanie planszy i *FrameGrafic* odpowiedzialne za ramkę okalającą planszę
i występujące na niej napisy. Każdy z rodzajów obiektów planszy (*BoardGrafic*) (Czołgi, Kafelki, Pociski, Orzełek) posiada swoją własną klasę, która
odpowiada za jego wyświetlanie na ekranie.

*StaticStateGrafic* składa się z klas *StaticGraphic* i *MenuStateGraphic*. *StaticGraphic* ustala ustawinie elementów, a *MenuStateGraphic* rysuje je
z odpowiednimi napisami, w zależności od stanu gry (*MenuState*, *PauseState*, *FinishedState*).

Moduł graficzny przechowuje informacje o obiektach do wyświetlenia, za pomocą shared pointerów. Umieszcza on wskaźniki zwrócone przez eventy gry,
w swoich własnych wskaźnikach. Dzięki temu nie ma potrzeby istnienia metod aktualizujących kompozyt, wszystkie wskaźniki na elementy gry, do wyświetlenia, są zaalokowane
w shared pointerach modułu graficznego.

Zarządza kompozytem klasa *GraficEventHandler*, która obsługuje eventy tworzone przez resztę bibliotek.

Moduł zaimplementowany w oparciu o bibliotekę SFML.

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

### 5.1 Napotkane problemy oraz możliwe usprawnienia w kodzie
Struktura projektu zmieniała się wielokrotnie w trakcie jego realizacji, co było naturalnym efektem odkrywania nowych problemów w oryginalnej idei struktury projektu, oraz poszerzania wiedzy na temat mechanizmów języka C++. Oto niektóre z odkrytych problemów (razem z propozycjami rozwiązania), których nie udało się rozwiązać z uwagi na ograniczony czas przeznaczony na realizację projektu:

#### Przechowywanie informacji na temat zdarzeń
W obecnej wersji, dodatkowe informacje o zdarzeniach przechowywane są w unii. Znaczną wadą takiej implementacji jest brak domyślnego destruktora w uniach i znacznie utrudniony proces implementacji własnego. W większości przypadków problem ten jest niewidoczny (a dokładniej dla prostych typów danych i obiektów, które nie wymagają specjalnego sposobu deinicjalizacji), jednak powoduje on nieprawidłowe działanie programu w momencie, gdy w jednym z członków unii przechowywane są sprytne wskaźniki `std::shared_ptr<T>`. W momencie destrukcji unii nie jest wywoływany destruktor sprytnego wskaźnika, przez co zmianie nie ulega jego wewnętrzny licznik referencji - mimo, że jedna z nich jest właśnie usuwana. W projekcie utrudniło to realizację systemu botów - zgodnie z założeniem, w momencie utworzenia bot miał inkrementować licznik `registeredBots` obiektu `BotController`, oraz dekrementować go podczas swojej destrukcji. Drugi proces mógł zostać rozpoczęty tylko przez obiekt `std::shared_ptr<T>`, w którym bot się znajdował - jednak jego wewnętrzny licznik referencji nie był poprawnie dekrementowany, gdy utworzono zdarzenie powiązane z botem (tym samym przechowujące sprytny wskaźnik na bota), a następnie je usunięto - destruktor sprytnego wskaźnika nie był wołany, a liczba referencji pozostawała sztucznie zawyżona.
W obecnej wersji projektu problem ten został rozwiązany poprzez wywoływanie
metody `.deregisterBot()` kontrolera botów w momencie obsługi zdarzenia `Event::TankKilled` przez obiekt  `EventHandler`, co wprowadza pewne niespójności do projektu (automatyczna inkrementacja licznika botów i konieczność jego ręcznej dekrementacji). Prawidłowym rozwiązanem byłoby zastąpienie unii kontenerem `std::variant<...>` - jednak wymagałoby to kosztownej czasowo refaktoryzacji pełnej objętości kodu projektu.

#### Manipulacja bytami
W pierwotnej wersji projektu istnienie klasy bazowej `Entity` nie było przewidziane - klasy `Tank` i `Bullet` należały do osobnych hierarchii klas i posiadały osobne kontenery manipulujące nimi. W wyniku wprowadzenia klasy bazowej `Entity` powstały niespójności w sygnaturach metod klas `EntityController` i `Board `- niektóre z nich operowały na niskim poziomie wyspecjalizowania (`Entity`), a inne na wysokim (np. `PlayerTank`). Te niespójności wprowadziły konieczność sprawdzania oraz rzutowania typów (`dynamic_cast, dynamic_pointer_cast`) w miejscach, gdzie dałoby się tego uniknąć. Na szczególną uwagę zasługuje tu metoda `generateCollisionEvent` klasy `Board`, oparta na rozwlekłych konstrukcjach `if - else if - ...` oraz sprawdzaniu typów.
Problem można rozwiązać ujednolicając metody klas `Entityontroller` i `Board` tak, aby operowały na wysokim poziomie wyspecjalizowania - może być wtedy konieczne utworzenie wielu wersji metod dla obsługi różnych typów, jednak będzie to rozwiązanie dużo bardziej zgrabne, niż wykonywanie nadmiarowego rzutowania.

#### Pełna niezależność biblioteki  `core-lib`
Z uwagi na trudną do przewidzenia z góry strukturę klasy `Event`, w pewnym momencie konieczne było wprowadzenie zależności biblioteki `core-lib` od innych bibliotek, aby była możliwa poprawna inicjalizacja obiektów zdarzeń. W konsekwencji, biblioteka `core-lib` przestała być (niezgodnie ze swoim założeniem) w pełni reużywalna. Reużywalność można przywrócić dzieląc implementację systemu zdarzeń na dwie części - o niskim poziomie specjalizacji (uproszczona wersja klasy `Event`, definiowana w bibliotece `core-lib`) oraz o wysokim poziomie specjalizacji (utworzenie nowej klasy np. `GameEvent`, definiującej sposoby tworzenia i identyfikacji obiektów zdarzeń, definiowanej poza biblioteką `core-lib`). Podobny podział został już zastosowany dla obiektów `EventHandler`, gdzie wyspecjalizowane podklasy definiowane są w bibliotece `game-lib`.

#### Jednolity sposób użytkowania obiektów o niskim poziomie abstrakcji
W chwili obecnej, przyjęte jest, że manipulacja obiektami `Entity` zwartymi w obiektach zdarzeń możliwa jest tylko za pośrednictwem klasy `Board`, a bezpośrednie wołanie ich metod jest zabronione (w takiej sytuacji nie są generowane zdarzenia informujące o zmianach stanu obiektów, a także nie jest wykonywana weryfikacja poprawności żądanych operacji). Aby wymusić obsługę obiektów poprzez klasę `Board`, można wprowadzić klasę pomocnika zabezpieczającego `EntityProxy`, która pozwalałaby na odczyt stanu bytu, ale uniemożliwiała jego zmianę. Taka klasa byłaby wtedy zaprzyjaźniona względem klasy `Board`, dzięki czemu klasa `Board` mogłaby wydobyć z pełnomocnika oryginalny obiekt i operować na nim, a metody klasy `Board` przyjmowałyby jako argumenty obiekty `EntityProxy` zamiast `Entity`.


### 5.2 Zastosowane nietrywialne rozwiązania programistyczne
- Zastosowanie wzorców projektowych:
   - *Singleton* (np. *Clock*)
   - *Pyłek* (przechowywanie płytek)
   - *Stan* (stany gry)
   - *Kompozyt* (biblioteka graficzna)
   - *Metoda wytwórcza* (*GridBuilder*)
   - *Mediator* (podsystem planszy)
   - *Wizytator* (obsługa kolizji)
- Skorzystanie z globalnej kolejki zdarzeń
- Implementacja własnego systemu kolizji

### 5.3 Możliwości rozbudowy projektu
- Docelowa implementacja systemu ładowania poziomów i zapisywania wyników - szkielety wymaganych klas są już napisane i zintegrowane z grą (`ScoreboardIO`, `GridBuilder`), wymagane jest jedynie zaimplementowanie odpowiednich metod, oraz napisanie testów
- Rozszerzenie zbieranych statystyk gry - przykładowo, mierzona może być liczba zniszczonych czołgów danego typu, lub łączny czas rozgrywki
- Optymalizacja wydajności modułu graficznego - eliminacja losowych spadków wydajności tuż po uruchomieniu gry i konieczności przerysowywania całego interfejsu w każdym cyklu zegara
- Usprawnienie algorytmu kontrolującego boty - zmniejszenie losowości operacji
- Rozszerzenie obsługi sytuacji wyjątkowych, w tym:
	- Obsługa niepoprawnego wejścia z pliku podczas ładowania planszy
	- Usunięcie możliwości wystąpienia niezdefiniowanego zachowania z funkcji `EventQueue<T>::pop()` i `Board::CreateCollsionEvent()`
- Dalsza refaktoryzacja celem uzyskania czystego kodu ;)

### 5.4 Dokumentacja kodu
Pełna dokumentacja kodu, razem z automatycznie wygenerowanymi diagramami UML, dostępna jest w katalogach `doc/html` o `doc/rtf` projektu.