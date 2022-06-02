//
// Created by tomek on 02.05.2022.
//

#ifndef PROI_PROJEKT_GAME_H
#define PROI_PROJEKT_GAME_H

#include <memory>

#include "GameState.h"
#include "KeyboardController.h"
#include "Menu.h"
#include "GameStatsIO.h"
#include "../../board-lib/include/Board.h"


class Clock;

template<class E>
class EventQueue;

class Event;

class KeyboardController;

class GameStats;

/**
 * Main container
 * Hold references to all other objects and runs the main event loop
 */
class Game {
public:
    Game() = delete;

    /**
     * Inits class Game
     * @param clockFreq Internal clock frequency
     */
    explicit Game(unsigned int clockFreq);

    /**
     * Starts the game
     */
    void run();

    /**
     * Switches the current state to active
     */
    void setActiveState();

    /**
     * Switches the current state to finished
     */
    void setFinishedState();

    /**
     * Switches the current state to pause
     */
    void setPauseState();

    /**
     * Switches the current state to menu
     */
    void setMenuState();

    /**
     * Quits the game
     */
    void quit();

    /**
     * Returns current state pointer
     * @return State pointer
     */
    GameState *getState();


    /**
     * Returns Point system state pointer
     * @return point system pointer
     */
    GameStats *getStats();

protected:
    /**
     * Called right before starting the event loop. Sets all remaining attrs, creates the render window,
     * loads the scoreboard and links subscribers to clock.
     */
    void setup();

    void initStates();

    void initComponents();

    void initScoreboard();

    /**
     * Creates an SFML render window and saves it
     */
    void initUI();

    void start();

    void reset();

    void prepareLevel(unsigned int levelNum);

    void end();

    void redrawUI();

    std::unique_ptr<GameState> active_state_;
    std::unique_ptr<GameState> pause_state_;
    std::unique_ptr<GameState> finished_state_;
    std::unique_ptr<GameState> menu_state_;
    GameState *state_{};

    std::unique_ptr<Board> board_;

    std::unique_ptr<GameStats> gameStats_;
    std::unique_ptr<GameStatsIO> gameStatsIO_;

    bool running_ = true;

    Clock *clock_;
    EventQueue<Event> *eventQueue_;

    std::unique_ptr<sf::RenderWindow> window_;
    std::unique_ptr<KeyboardController> keyboardController_;

};


#endif //PROI_PROJEKT_GAME_H
