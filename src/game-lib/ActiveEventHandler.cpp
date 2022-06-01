//
// Created by tomek on 02.05.2022.
//

#include <utility>
#include <iostream>

#include "../board-lib/include/Board.h"
#include "include/ActiveEventHandler.h"
#include "include/Game.h"
#include "../core-lib/include/EventHandler.h"  //FIXME SUPER UGLY


ActiveEventHandler::ActiveEventHandler(Game *game, ActiveGameState* state) {
    game_ = game;
    state_ = state;
}

void ActiveEventHandler::processEvent(std::unique_ptr<Event> event) {
    switch (event->type) {
        case(Event::KeyPressed): {
            //TODO Implement response to KeyPressed
            // DOWN
            if(event->info.keyInfo.keyCode == 74) {
                state_->get_board()->setTankDirection(state_->get_tank(), South);
                state_->get_board()->setTankMoving(state_->get_tank(), true);
            }
            // UP
            if(event->info.keyInfo.keyCode == 73) {
                state_->get_board()->setTankDirection(state_->get_tank(), North);
                state_->get_board()->setTankMoving(state_->get_tank(), true);
            }
            // LEFT
            if(event->info.keyInfo.keyCode == 72) {
                state_->get_board()->setTankDirection(state_->get_tank(), West);
                state_->get_board()->setTankMoving(state_->get_tank(), true);
            }
            // RIGHT
            if(event->info.keyInfo.keyCode == 71) {
                state_->get_board()->setTankDirection(state_->get_tank(), East);
                state_->get_board()->setTankMoving(state_->get_tank(), true);
            }
            // SPACEBAR
            if(event->info.keyInfo.keyCode == 57) {
                state_->get_board()->fireTank(state_->get_tank());
            }
            break;
            // ESCAPE
            if (event->info.keyInfo.keyCode == 36) {
                game_->setPauseState();
            }
        }
        case (Event::KeyReleased):{
            // DOWN
            if(event->info.keyInfo.keyCode == 74) {
                state_->get_board()->setTankMoving(state_->get_tank(), false);
                state_->get_board()->snapTankToGrid(state_->get_tank());
            }
            // UP
            if(event->info.keyInfo.keyCode == 73) {
                state_->get_board()->setTankMoving(state_->get_tank(), false);
                state_->get_board()->snapTankToGrid(state_->get_tank());
            }
            // LEFT
            if(event->info.keyInfo.keyCode == 72) {
                state_->get_board()->setTankMoving(state_->get_tank(), false);
                state_->get_board()->snapTankToGrid(state_->get_tank());
            }
            // RIGHT
            if(event->info.keyInfo.keyCode == 71) {
                state_->get_board()->setTankMoving(state_->get_tank(), false);
                state_->get_board()->snapTankToGrid(state_->get_tank());
            }
            // SPACEBAR
            if(event->info.keyInfo.keyCode == 57) {
            }
            break;
        }
        case (Event::TankKilled):{
            if(event->info.entityInfo.entity == state_->get_tank())
                game_->setFinishedState();
            break;
        }
        case (Event::EntityEntityCollision):{
            break;
        }
        case (Event::EntityGridCollision):{
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