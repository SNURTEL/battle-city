//
// Created by Rafal on 21.05.2022.
//

#ifndef PROI_PROJEKT_FINISHEDEVENTHANDLER_H
#define PROI_PROJEKT_FINISHEDEVENTHANDLER_H

#include <memory>

#include "../../core-lib/include/EventHandler.h"

class Event;

class Game;

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
    explicit FinishedEventHandler(Game* game);

    /**
     * Responds to event
     * @param event
     */
    void processEvent(std::unique_ptr<Event> event) override;

private:
    Game* game_;
};


#endif //PROI_PROJEKT_FinishedEVENTHANDLER_H
