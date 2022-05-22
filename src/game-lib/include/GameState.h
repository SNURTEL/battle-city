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

    virtual void set_position(unsigned int) {}
    virtual unsigned int get_position() {return 0;}

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

class MenuGameState: public GameState{
private:
    unsigned int position_ = 1;
public:
    void set_position(unsigned int);
    unsigned int get_position();
    MenuGameState()=delete;
    explicit MenuGameState(Game* game);
};

class FinishedGameState: public GameState{
public:
    FinishedGameState()=delete;
    explicit FinishedGameState(Game* game);
};

#endif //PROI_PROJEKT_GAMESTATE_H

