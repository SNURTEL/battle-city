//
// Created by tomek on 02.05.2022.
//

#ifndef PROI_PROJEKT_GAME_H
#define PROI_PROJEKT_GAME_H

#include <memory>
#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "KeyboardController.h"

class Clock;

template <class E>
class EventQueue;

class Event;

class GameState;

class KeyboardController;

class Game {
public:
    Game()=delete;

    explicit Game(unsigned int clock_freq);

    void run();

    void setActiveState();

    [[nodiscard]] Clock *getClock() const;

private:
    void setup();
    void createRenderWindow();
    std::unique_ptr<GameState> active_state_;
    GameState* state_{};

    Clock* clock_;
    EventQueue<Event>* eventQueue_;

    std::unique_ptr<sf::RenderWindow> window_;
    std::unique_ptr<KeyboardController> keyboardController_;

};


#endif //PROI_PROJEKT_GAME_H
