//
// Created by Rafal on 21.05.2022.
//

#ifndef PROI_PROJEKT_MENUEVENTHANDLER_H
#define PROI_PROJEKT_MENUEVENTHANDLER_H

#include <memory>

#include "../../core-lib/include/EventHandler.h"
#include "GameState.h"

class Event;

class Game;

class MenuGameState;

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
    MenuEventHandler(Game* game, MenuGameState* state);

    /**
     * Responds to event
     * @param event
     */
    void processEvent(std::unique_ptr<Event> event) override;

private:
    Game* game_;
    MenuGameState* state_;
};


#endif //PROI_PROJEKT_MENUEVENTHANDLER_H
