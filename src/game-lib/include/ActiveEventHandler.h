//
// Created by tomek on 02.05.2022.
//

#ifndef PROI_PROJEKT_ACTIVEEVENTHANDLER_H
#define PROI_PROJEKT_ACTIVEEVENTHANDLER_H

#include <memory>

#include "../../core-lib/include/EventHandler.h"

class Event;

class Game;

/**
 * Event handler for active game state. Operates on Event class events
 */
class ActiveEventHandler: public PublisherEventHandler{
public:
    ActiveEventHandler()=delete;

    /**
     * Inits class ActiveEventHandler.
     * @param game
     */
    explicit ActiveEventHandler(Game* game);

    /**
     * Responds to event
     * @param event
     */
    void processEvent(std::unique_ptr<Event> event) override;

private:
    Game* game_;
};


#endif //PROI_PROJEKT_ACTIVEEVENTHANDLER_H
