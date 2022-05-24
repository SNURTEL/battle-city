//
// Created by Rafal on 21.05.2022.
//

#include <utility>
#include <iostream>

#include "include/MenuEventHandler.h"
#include "include/Game.h"
#include "../core-lib/include/EventHandler.h"  //FIXME SUPER UGLY


MenuEventHandler::MenuEventHandler(Game *game, MenuGameState* state) {
    game_ = game;
    state_ = state;
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
                state_->set_position(2);
            }
            if (event->info.keyInfo.keyCode == 73) {
                state_->set_position(1);
            }
            std::cout << "current pos: " << state_->get_position() << std::endl;
            if (event->info.keyInfo.keyCode == 58) {
                if (state_->get_position() == 1)
                {
                    game_->setActiveState();
                }
                if (state_->get_position() == 2)
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