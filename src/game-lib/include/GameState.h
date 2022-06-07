//
// Created by tomek on 02.05.2022.
//

#ifndef PROI_PROJEKT_GAMESTATE_H
#define PROI_PROJEKT_GAMESTATE_H

#include <memory>

#include "ActiveEventHandler.h"
#include "MenuEventHandler.h"
#include "PauseEventHandler.h"
#include "FinishedEventHandler.h"


class Menu;
class PublisherEventHandler;
class Board;

class Game;

class PlayerTank;

/**
 * Encapsulates game's state and holds data related to it.
 * Each state contains an event handler, which defines game's responses to internal events
 */
class GameState {
public:
    virtual ~GameState()=default;

    /**
     * Returns state's event handler
     * @return State's event handler
     */
    PublisherEventHandler* getEventHandler();

protected:
    GameState(Game* n_game, std::unique_ptr<PublisherEventHandler> n_eventHandler) : game_(n_game), eventHandler_(std::move(n_eventHandler)) {};
    std::unique_ptr<PublisherEventHandler> eventHandler_;
    Game* game_;
};

// #####################################

/**
 * Represents Active Game State
 * Contains data related to it
 */
class ActiveGameState: public GameState{
private:
    std::shared_ptr<PlayerTank> player_tank_;
    Board* board_;
public:

    /**
     * Returns board pointer
     * @return Board pointer
     */
    Board* get_board();

    /**
     * Returns player tank pointer
     * @return player tank pointer
     */
    std::shared_ptr<PlayerTank> get_player_tank();

    ActiveGameState()=delete;
    explicit ActiveGameState(Game* game);
};


/**
 * Represents Pause Game State
 * Contains data related to it
 */
class PauseGameState: public GameState{
public:

    /**
     * Returns menu pointer
     * @return menu pointer
     */
    Menu* get_menu();

    PauseGameState()=delete;
    explicit PauseGameState(Game* game);
private:
    std::unique_ptr<Menu> menu_;
};


/**
 * Represents Menu Game State
 * Contains data related to it
 */
class MenuGameState: public GameState{
public:

    /**
     * Returns menu pointer
     * @return menu pointer
     */
    Menu* get_menu();

    MenuGameState()=delete;
    explicit MenuGameState(Game* game);
private:
    std::unique_ptr<Menu> menu_;
};


/**
 * Represents Finished Game State
 * Contains data related to it
 */
class FinishedGameState: public GameState{
public:
    FinishedGameState()=delete;
    explicit FinishedGameState(Game* game);
};

#endif //PROI_PROJEKT_GAMESTATE_H
