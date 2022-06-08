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
            if (event->info.keyInfo.keyCode == 36 || event->info.keyInfo.keyCode == 58) {
                game_->quit();
            }
            break;
        }
        case (Event::KeyReleased):{
            break;
        }
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
            // throw InvalidEventException("Invalid event for FinishedEventHandler\nEvent enum cast: " + std::to_string(event->type));
            break;
        }
    }
    return std::move(event);
}