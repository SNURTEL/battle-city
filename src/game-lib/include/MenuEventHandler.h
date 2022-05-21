//
// Created by Rafal on 21.05.2022.
//

#ifndef PROI_PROJEKT_MENUEVENTHANDLER_H
#define PROI_PROJEKT_MENUEVENTHANDLER_H

#include <memory>

#include "../../core-lib/include/EventHandler.h"

class Event;

class Game;

/**
 * Event handler for Menu game state. Operates on Event class events
 */
class MenuEventHandler: public PublisherEventHandler{
public:
    MenuEventHandler()=delete;

    /**
     * Inits class MenuEventHandler.
     * @param game
     */
    explicit MenuEventHandler(Game* game);

    /**
     * Responds to event
     * @param event
     */
    void processEvent(std::unique_ptr<Event> event) override;

private:
    Game* game_;
};


#endif //PROI_PROJEKT_MENUEVENTHANDLER_H
