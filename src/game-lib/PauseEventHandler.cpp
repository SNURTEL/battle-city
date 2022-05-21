//
// Created by Rafal on 21.05.2022.
//

#include <utility>
#include <iostream>

#include "include/ActiveEventHandler.h"
#include "../core-lib/include/EventHandler.h"  //FIXME SUPER UGLY


ActiveEventHandler::ActiveEventHandler(Game *game) {
    game_ = game;
}

void ActiveEventHandler::processEvent(std::unique_ptr<Event> event) {
    switch (event->type) {
        case(Event::KeyPressed): {
            //TODO Implement response to KeyPressed
            std::cout << "Key pressed: " << event->info.keyInfo.keyCode << std::endl;
            break;
        }
        case (Event::KeyReleased):{
            std::cout << "Key released: " << event->info.keyInfo.keyCode << std::endl;
            break;
        }
        case (Event::NullEvent):{
            break;
        }
        default:{
            throw InvalidEventException("Invalid event for ActiveEventHandler");
        }
    }
}