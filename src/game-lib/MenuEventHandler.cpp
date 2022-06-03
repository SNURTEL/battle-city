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
            if (event->info.keyInfo.keyCode == 74) {
                state_->get_menu()->add_pos();
                state_->get_menu()->notify();
            }
            if (event->info.keyInfo.keyCode == 73) {
                state_->get_menu()->sub_pos();
                state_->get_menu()->notify();
            }
            if (event->info.keyInfo.keyCode == 58) {
                if (state_->get_menu()->get_pos() == 1)
                {
                    state_->get_menu()->notify_enter();
                    game_->setActiveState();
                }
                if (state_->get_menu()->get_pos() == 2)
                {
                    state_->get_menu()->notify_enter();
                    game_->quit();
                }
            }
            break;
        }
        case (Event::KeyReleased):{
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