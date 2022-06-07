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

std::unique_ptr<Event> MenuEventHandler::processEvent(std::unique_ptr<Event> event) {
    switch (event->type) {
        case(Event::KeyPressed): {
            if (event->info.keyInfo.keyCode == 74) {
                state_->get_menu()->sub_pos();
                state_->get_menu()->notify();
            }
            if (event->info.keyInfo.keyCode == 73) {
                state_->get_menu()->add_pos();
                state_->get_menu()->notify();
            }
            if (event->info.keyInfo.keyCode == 58) {
                if (state_->get_menu()->get_pos() == 1)
                {
                    game_->start();
                }
                if (state_->get_menu()->get_pos() == 2)
                {
                    game_->quit();
                }
            }
            break;
        }
       case (Event::KeyReleased):
       case (Event::EntityMoved):
       case (Event::EntityRemoved):
       case (Event::EntitySpawned):
       case (Event::StateChanged):
       case (Event::TilePlaced):
       case (Event::TileChanged):
       case (Event::TileDeleted):
       case (Event::PlayerSpawned):
       case (Event::TankKilled):
       case (Event::TankRotated):
       case (Event::TankHit):
       case (Event::MenuEnterClicked):
       case (Event::MenuSelectionChange):
       case (Event::NullEvent):
       case (Event::StatisticsChanged):
       case (Event::LevelLoaded): {
           break;
       }
        default:{
            throw InvalidEventException("Invalid event for MenuEventHandler\nEvent enum cast: " + std::to_string(event->type));
        }
    }
    return std::move(event);
}