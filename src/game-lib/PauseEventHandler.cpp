//
// Created by Rafal on 21.05.2022.
//

#include <utility>
#include <iostream>

#include "include/PauseEventHandler.h"
#include "../core-lib/include/EventHandler.h"  //FIXME SUPER UGLY


PauseEventHandler::PauseEventHandler(Game *game) {
    game_ = game;
}

void PauseEventHandler::processEvent(std::unique_ptr<Event> event) {
    switch (event->type) {
        case(Event::KeyPressed): {
            //TODO Implement response to KeyPressed
            std::cout << "Key pressed a: " << event->info.keyInfo.keyCode << std::endl;
            break;
        }
        case (Event::KeyReleased):{
            std::cout << "Key released a: " << event->info.keyInfo.keyCode << std::endl;
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