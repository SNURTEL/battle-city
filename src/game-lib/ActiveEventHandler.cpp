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
                state_->get_player_tank()->setFacing(South);
                state_->get_player_tank()->move();
            }
            // UP
            if(event->info.keyInfo.keyCode == 73) {
                state_->get_player_tank()->setFacing(North);
                state_->get_player_tank()->move();
            }
            // LEFT
            if(event->info.keyInfo.keyCode == 72) {
                state_->get_player_tank()->setFacing(West);
                state_->get_player_tank()->move();
            }
            // RIGHT
            if(event->info.keyInfo.keyCode == 71) {
                state_->get_player_tank()->setFacing(East);
                state_->get_player_tank()->move();
            }
            // SPACEBAR
            if(event->info.keyInfo.keyCode == 57) {
            }
            break;
        }
        case (Event::KeyReleased):{
            if (event->info.keyInfo.keyCode == 36) {
                game_->setPauseState();
            }
            std::cout << "State is active " << event->info.keyInfo.keyCode << std::endl;
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