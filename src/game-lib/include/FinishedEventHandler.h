//
// Created by Rafal on 21.05.2022.
//

#ifndef PROI_PROJEKT_FINISHEDEVENTHANDLER_H
#define PROI_PROJEKT_FINISHEDEVENTHANDLER_H

#include <memory>

#include "../../core-lib/include/EventHandler.h"

class Event;

class Game;

class FinishedGameState;

/**
 * Event handler for Finished game state. Operates on Event class events
 */
class FinishedEventHandler: public PublisherEventHandler{
public:
    FinishedEventHandler()=delete;

    /**
     * Inits class FinishedEventHandler.
     * @param game
     */
    FinishedEventHandler(Game* game, FinishedGameState* state);

    /**
     * Responds to event
     * @param event
     */
    std::unique_ptr<Event> processEvent(std::unique_ptr<Event> event) override;

private:
    Game* game_;
    FinishedGameState* state_;
};


#endif //PROI_PROJEKT_FinishedEVENTHANDLER_H
