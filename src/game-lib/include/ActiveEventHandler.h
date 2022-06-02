//
// Created by tomek on 02.05.2022.
//

#ifndef PROI_PROJEKT_ACTIVEEVENTHANDLER_H
#define PROI_PROJEKT_ACTIVEEVENTHANDLER_H

#include <memory>
#include "../../core-lib/include/EventHandler.h"
#include "../../core-lib/include/EventQueue.h"
#include "GameStatistics.h"

class Event;

class Board;

class Game;

class ActiveGameState;

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
    ActiveEventHandler(Game* game, ActiveGameState* state);

    /**
     * Responds to event
     * @param event
     */
    void processEvent(std::unique_ptr<Event> event) override;

private:
    Game* game_;
    ActiveGameState* state_;
    EventQueue<Event>* eventQueue_ = EventQueue<Event>::instance();
};


#endif //PROI_PROJEKT_ACTIVEEVENTHANDLER_H
