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
#include "../../tank-lib/include/Tank.h"
#include "Menu.h"


class PublisherEventHandler;

class Game;

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
    GameState()=default;
    std::unique_ptr<PublisherEventHandler> eventHandler_;
    Game* game_{};
};

// #####################################

/**
 * Represents Active Game State
 * Contains data related to it
 */
class ActiveGameState: public GameState{
private:
    std::unique_ptr<PlayerTank> playerTank_;
public:

    /**
     * Returns player tank pointer
     * @return Player Tank pointer
     */
    PlayerTank* get_player_tank();

    ActiveGameState()=delete;
    explicit ActiveGameState(Game* game);
};


/**
 * Represents Pause Game State
 * Contains data related to it
 */
class PauseGameState: public GameState{
public:
    PauseGameState()=delete;
    explicit PauseGameState(Game* game);
};


/**
 * Represents Menu Game State
 * Contains data related to it
 */
class MenuGameState: public GameState{
public:
    void set_position(unsigned int);
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

