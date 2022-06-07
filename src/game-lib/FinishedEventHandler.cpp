//
// Created by Rafal on 21.05.2022.
//

#include <utility>
#include <iostream>

#include "include/FinishedEventHandler.h"
#include "include/Game.h"
#include "../core-lib/include/EventHandler.h"  //FIXME SUPER UGLY


FinishedEventHandler::FinishedEventHandler(Game *game, FinishedGameState* state) {
    game_ = game;
    state_ = state;
}

std::unique_ptr<Event> FinishedEventHandler::processEvent(std::unique_ptr<Event> event) {
    switch (event->type) {
        case(Event::KeyPressed): {
            //TODO Implement response to KeyPressed
            std::cout << "State is finished! " << event->info.keyInfo.keyCode << std::endl;
            break;
        }
        case (Event::KeyReleased):{
            if (event->info.keyInfo.keyCode == 36) {
                game_->quit();
            }
            std::cout << "State is finished! " << event->info.keyInfo.keyCode << std::endl;
            break;
        }
        case (Event::NullEvent):{
            break;
        }
        default:{
            throw InvalidEventException("Invalid event for FinishedEventHandler\nEvent enum cast: " + std::to_string(event->type));
        }
    }
    return std::move(event);
}