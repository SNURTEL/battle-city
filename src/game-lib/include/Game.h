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
#include "../../graphic-lib/include/Window.h"



class Clock;

template<class E>
class EventQueue;

class Event;

class KeyboardController;

class GameStatistics;

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
     *
     * Queues Event::StateChanged
     */
    void setActiveState();

    /**
     * Switches the current state to finished
     *
     * Queues Event::StateChanged
     */
    void setFinishedState();

    /**
     * Switches the current state to pause
     *
     * Queues Event::StateChanged
     */
    void setPauseState();

    /**
     * Switches the current state to menu
     *
     * Queues Event::StateChanged
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
     * Returns the pointer to the stats object
     * @return stats object pointer
     */
    GameStatistics *getStats();

protected:
    /**
     * Called right before starting the event loop. Sets all remaining attrs, creates the render window,
     * loads the scoreboard and links subscribers to clock; sets current state to Menu
     *
     * Queues Event::StateChanged
     */
    void setup();

    /**
     * Inits all internal state objects
     */
    void initStates();

    /**
     * Inits other remaining components (clock, board, keyboard controller, etc)
     */
    void initComponents();

    /**
     * Loads the scoreboard and inits the stats object
     */
    void initScoreboard();

    /**
     * Inits graphical components
     */
    void initUI();

    /**
     * Starts the game at level 1
     *
     * Queues Event::StateChanged
     */
    void start();

    /**
     * Resets stats and the board
     *
     * Possibly queues multiple instances of Event::EntityRemoved and a single instance ofEvent::StateChanged and Event::PlayerSpawned
     */
    void reset();

    /**
     * Prepares the board to start a given level
     *
     * Possibly queues multiple instances of Event::EntityRemoved, always queues Event::PlayerSpawned
     * @param levelNum Level number
     */
    void prepareLevel(unsigned int levelNum);

    /**
     * Ends the game and clears the board
     *
     * Possibly queues multiple instances of Event::EntityRemoved, always queues Event::StateChanged
     */
    void end();

    /**
     * Redraws the UI; called in every tick
     */
    void redrawUI();

    std::unique_ptr<GameState> active_state_;
    std::unique_ptr<GameState> pause_state_;
    std::unique_ptr<GameState> finished_state_;
    std::unique_ptr<GameState> menu_state_;
    GameState *state_{};

    std::unique_ptr<Board> board_;

    std::unique_ptr<GameStatistics> gameStats_;
    std::unique_ptr<GameStatsIO> gameStatsIO_;

    bool running_ = true;

    Clock *clock_;
    EventQueue<Event> *eventQueue_;

    std::unique_ptr<Window> window_;
    std::unique_ptr<KeyboardController> keyboardController_;

};


#endif //PROI_PROJEKT_GAME_H
