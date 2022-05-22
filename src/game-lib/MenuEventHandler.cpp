//
// Created by Rafal on 21.05.2022.
//

#include <utility>
#include <iostream>

#include "include/MenuEventHandler.h"
#include "include/Game.h"
#include "../core-lib/include/EventHandler.h"  //FIXME SUPER UGLY


MenuEventHandler::MenuEventHandler(Game *game) {
    game_ = game;
}

void MenuEventHandler::processEvent(std::unique_ptr<Event> event) {
    switch (event->type) {
        case(Event::KeyPressed): {
            //TODO Implement response to KeyPressed
            std::cout << "State is Menu!" << event->info.keyInfo.keyCode << std::endl;
            break;
        }
        case (Event::KeyReleased):{
            if (event->info.keyInfo.keyCode == 74) {
                game_->get_state()->set_position(2);
            }
            if (event->info.keyInfo.keyCode == 73) {
                game_->get_state()->set_position(1);
            }
            std::cout << "current pos: " << game_->get_state()->get_position() << std::endl;
            if (event->info.keyInfo.keyCode == 58) {
                if (game_->get_state()->get_position() == 1)
                {
                    game_->setActiveState();
                }
                if (game_->get_state()->get_position() == 2)
                {
                    game_->setFinishedState();
                }
            }
            break;
        }
        case (Event::NullEvent):{
            break;
        }
        default:{
            throw InvalidEventException("Invalid event for MenuEventHandler");
        }
    }
}