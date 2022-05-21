//
// Created by tomek on 02.05.2022.
//

#ifndef PROI_PROJEKT_GAMESTATE_H
#define PROI_PROJEKT_GAMESTATE_H

#include <memory>

#include "ActiveEventHandler.h"


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

class ActiveGameState: public GameState{
public:
    ActiveGameState()=delete;
    explicit ActiveGameState(Game* game);
};

class PauseGameState: public GameState{
public:
    PauseGameState()=delete;
    explicit PauseGameState(Game* game);
};

#endif //PROI_PROJEKT_GAMESTATE_H

