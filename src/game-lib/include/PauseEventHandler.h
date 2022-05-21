//
// Created by Rafal on 21.05.2022.
//

#ifndef PROI_PROJEKT_PAUSEEVENTHANDLER_H
#define PROI_PROJEKT_PAUSEEVENTHANDLER_H

#include <memory>

#include "../../core-lib/include/EventHandler.h"

class Event;

class Game;

/**
 * Event handler for Pause game state. Operates on Event class events
 */
class PauseEventHandler: public PublisherEventHandler{
public:
    PauseEventHandler()=delete;

    /**
     * Inits class PauseEventHandler.
     * @param game
     */
    explicit PauseEventHandler(Game* game);

    /**
     * Responds to event
     * @param event
     */
    void processEvent(std::unique_ptr<Event> event) override;

private:
    Game* game_;
};


#endif //PROI_PROJEKT_PauseEVENTHANDLER_H
