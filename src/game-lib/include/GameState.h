//
// Created by tomek on 02.05.2022.
//

#ifndef PROI_PROJEKT_GAMESTATE_H
#define PROI_PROJEKT_GAMESTATE_H

#include <memory>

#include "ActiveEventHandler.h"


class PublisherEventHandler;

class Game;

class GameState {
public:
    virtual ~GameState()=default;
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

#endif //PROI_PROJEKT_GAMESTATE_H

