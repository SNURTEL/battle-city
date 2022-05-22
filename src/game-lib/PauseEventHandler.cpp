//
// Created by Rafal on 21.05.2022.
//

#include <utility>
#include <iostream>

#include "include/PauseEventHandler.h"
#include "include/Game.h"
#include "../core-lib/include/EventHandler.h"  //FIXME SUPER UGLY


PauseEventHandler::PauseEventHandler(Game *game) {
    game_ = game;
}

void PauseEventHandler::processEvent(std::unique_ptr<Event> event) {
    switch (event->type) {
        case(Event::KeyPressed): {
            //TODO Implement response to KeyPressed
            std::cout << "Game is paused! " << event->info.keyInfo.keyCode << std::endl;
            break;
        }
        case (Event::KeyReleased):{
            if (event->info.keyInfo.keyCode == 36) {
                game_->setActiveState();
            }
            std::cout << "Game is paused! " << event->info.keyInfo.keyCode << std::endl;
            break;
        }
        case (Event::NullEvent):{
            break;
        }
        default:{
            throw InvalidEventException("Invalid event for PauseEventHandler");
        }
    }
}