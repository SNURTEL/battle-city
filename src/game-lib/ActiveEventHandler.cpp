//
// Created by tomek on 02.05.2022.
//

#include <utility>
#include <iostream>

#include "../board-lib/include/Board.h"
#include "../tank-lib/include/Bullet.h"
#include "include/ActiveEventHandler.h"
#include "include/Game.h"


ActiveEventHandler::ActiveEventHandler(Game *game, ActiveGameState* state) {
    game_ = game;
    state_ = state;
}

void ActiveEventHandler::processEvent(std::unique_ptr<Event> event) {
<<<<<<< HEAD
//    switch (event->type) {  //FIXME commented out to avoid refactoring the same code twice
//        case(Event::KeyPressed): {
//            if(event->info.keyInfo.keyCode == 74) {
//                state_->get_board()->setTankDirection(state_->get_player_tank(), South);
//                state_->get_board()->setTankMoving(state_->get_player_tank(), true);
//            }
//            // UP
//            if(event->info.keyInfo.keyCode == 73) {
//                state_->get_board()->setTankDirection(state_->get_player_tank(), North);
//                state_->get_board()->setTankMoving(state_->get_player_tank(), true);
//            }
//            // LEFT
//            if(event->info.keyInfo.keyCode == 72) {
//                state_->get_board()->setTankDirection(state_->get_player_tank(), West);
//                state_->get_board()->setTankMoving(state_->get_player_tank(), true);
//            }
//            // RIGHT
//            if(event->info.keyInfo.keyCode == 71) {
//                state_->get_board()->setTankDirection(state_->get_player_tank(), East);
//                state_->get_board()->setTankMoving(state_->get_player_tank(), true);
//            }
//            // SPACEBAR
//            if(event->info.keyInfo.keyCode == 57) {
//                state_->get_board()->fireTank(state_->get_player_tank());
//            }
//            break;
//            // ESCAPE
//            if (event->info.keyInfo.keyCode == 36) {
//                game_->setPauseState();
//            }
//        }
//        case (Event::KeyReleased):{
//            // DOWN
//            if(event->info.keyInfo.keyCode == 74) {
//                state_->get_board()->setTankMoving(state_->get_player_tank(), false);
//                state_->get_board()->snapTankToGrid(state_->get_player_tank());
//            }
//            // UP
//            if(event->info.keyInfo.keyCode == 73) {
//                state_->get_board()->setTankMoving(state_->get_player_tank(), false);
//                state_->get_board()->snapTankToGrid(state_->get_player_tank());
//            }
//            // LEFT
//            if(event->info.keyInfo.keyCode == 72) {
//                state_->get_board()->setTankMoving(state_->get_player_tank(), false);
//                state_->get_board()->snapTankToGrid(state_->get_player_tank());
//            }
//            // RIGHT
//            if(event->info.keyInfo.keyCode == 71) {
//                state_->get_board()->setTankMoving(state_->get_player_tank(), false);
//                state_->get_board()->snapTankToGrid(state_->get_player_tank());
//            }
//            // SPACEBAR
//            if(event->info.keyInfo.keyCode == 57) {
//            }
//            break;
//        }
//        case (Event::TankKilled):{
//            if(event->info.entityInfo.entity == state_->get_player_tank())
//                game_->setFinishedState();
//            break;
//        }
//        case (Event::EntityEntityCollision):{
//            // TANK
//            if (std::static_pointer_cast<Tank>(event->info.entityEntityCollisionInfo.entity1) != nullptr)
//            {
//                if (std::static_pointer_cast<Tank>(event->info.entityEntityCollisionInfo.entity2) != nullptr)
//                {
//                    event->info.entityEntityCollisionInfo.entity1->moveBack();
//                    event->info.entityEntityCollisionInfo.entity2->moveBack(); // TWO TANKS HITTING THEMSELVES
//                }
//                if (std::static_pointer_cast<Bullet>(event->info.entityEntityCollisionInfo.entity2) != nullptr)
//                {
//                    // BULLET HITTING TANK
//                    if (event->info.entityEntityCollisionInfo.entity1 == state_->get_player_tank()
//                        // && event->info.entityEntityCollisionInfo.entity2->isFriendly() == false
//                        )
//                    {
//                        // IF ENEMY BULLET HIT PLAYER'S TANK
//                        state_->get_player_tank()->deltaLives(-1);
//                        // state_->get_board()->removeEntity(event->info.entityEntityCollisionInfo.entity2) Kill bullet after hitting
//                        if (state_->get_player_tank()->getLives() == 0)
//                        {
//                            // TANK DIED
//                            eventQueue_->registerEvent(std::make_unique<Event>(Event::TankKilled, event->info.entityEntityCollisionInfo.entity1)); // kill player tank
//                        }
//                    }
//                    if (event->info.entityEntityCollisionInfo.entity1 != state_->get_player_tank()
//                        // && event->info.entityEntityCollisionInfo.entity2->isFriendly() == true
//                        )
//                    {
//                        // IF FRIENDLY BULLET HIT ENEMY'S TANK
//                        eventQueue_->registerEvent(std::make_unique<Event>(Event::TankKilled, event->info.entityEntityCollisionInfo.entity1)); // kill tank
//                            // game_->get_point_system()->add_points(event->info.entityEntityCollisionInfo.entity1->getPoints()); // get points for killing a tank
//                        // state_->get_board()->removeEntity(event->info.entityEntityCollisionInfo.entity2) kill bullet after hit
//                    }
//                }
//            }
//            // BULLET
//            if (std::static_pointer_cast<Bullet>(event->info.entityEntityCollisionInfo.entity1) != nullptr)
//            {
//                if (std::static_pointer_cast<Bullet>(event->info.entityEntityCollisionInfo.entity2) != nullptr)
//                {
//                    // TWO BULLETS HITTING THEMSELVES
//
//                }
//                if (std::static_pointer_cast<Tank>(event->info.entityEntityCollisionInfo.entity2) != nullptr)
//                {
//                    // BULLET HITTING TANK
//                    if (event->info.entityEntityCollisionInfo.entity2 == state_->get_player_tank()
//                        // && event->info.entityEntityCollisionInfo.entity1->isFriendly() == false
//                        )
//                    {
//                        // IF ENEMY BULLET HIT PLAYER'S TANK
//                        state_->get_player_tank()->deltaLives(-1);
//                        // state_->get_board()->removeEntity(event->info.entityEntityCollisionInfo.entity1) Kill bullet after hitting
//                        if (state_->get_player_tank()->getLives() == 0)
//                        {
//                            // TANK DIED
//                            eventQueue_->registerEvent(std::make_unique<Event>(Event::TankKilled, event->info.entityEntityCollisionInfo.entity2)); // kill player tank
//                        }
//                    }
//                    if (event->info.entityEntityCollisionInfo.entity2 != state_->get_player_tank()
//                        // && event->info.entityEntityCollisionInfo.entity1->isFriendly() == true
//                        )
//                    {
//                        // IF FRIENDLY BULLET HIT ENEMY'S TANK
//                            eventQueue_->registerEvent(std::make_unique<Event>(Event::TankKilled, event->info.entityEntityCollisionInfo.entity2)); // kill tank
//                            // game_->get_point_system()->add_points(event->info.entityEntityCollisionInfo.entity2->getPoints()); // get points for killing a tank
//                        // state_->get_board()->removeEntity(event->info.entityEntityCollisionInfo.entity1) Kill bullet after hitting
//                    }
//                }
//            }
//
//            break;
//        }
//        case (Event::EntityGridCollision):{
//            // TANK
//            if (std::static_pointer_cast<Tank>(event->info.entityGridCollisionInfo.entity) != nullptr)
//            {
//                event->info.entityGridCollisionInfo.entity->moveBack(); // tank hitting the wall
//            }
//            // BULLET
//            if (std::static_pointer_cast<Bullet>(event->info.entityGridCollisionInfo.entity) != nullptr)
//            {
//                unsigned int x = event->info.entityGridCollisionInfo.x;
//                unsigned int y = event->info.entityGridCollisionInfo.y;
//                state_->get_board()->deleteTile(x, y);
//                // state_->get_board()-> Kill bullet after hitting
//            }
//            break;
//        }
//        case (Event::NullEvent):{
//            break;
//        }
//        default:{
//            throw InvalidEventException("Invalid event for ActiveEventHandler\nEvent enum cast: " + std::to_string(event->type));
//        }
//    }
=======
    switch (event->type) {
        case(Event::KeyPressed): {
            if(event->info.keyInfo.keyCode == 74) {
                game_->getBoard()->setTankDirection(game_->getBoard()->getPlayerTank(), South);
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), true);
            }
            // UP
            if(event->info.keyInfo.keyCode == 73) {
                game_->getBoard()->setTankDirection(game_->getBoard()->getPlayerTank(), North);
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), true);
            }
            // LEFT
            if(event->info.keyInfo.keyCode == 72) {
                game_->getBoard()->setTankDirection(game_->getBoard()->getPlayerTank(), West);
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), true);
            }
            // RIGHT
            if(event->info.keyInfo.keyCode == 71) {
                game_->getBoard()->setTankDirection(game_->getBoard()->getPlayerTank(), East);
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), true);
            }
            // SPACEBAR
            if(event->info.keyInfo.keyCode == 57) {
                game_->getBoard()->fireTank(game_->getBoard()->getPlayerTank());
            }
            // ESCAPE
            if (event->info.keyInfo.keyCode == 36) {
                game_->setPauseState();
            }
            break;
        }
        case (Event::KeyReleased):{
            // DOWN
            if(event->info.keyInfo.keyCode == 74) {
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), false);
                game_->getBoard()->snapTankToGrid(game_->getBoard()->getPlayerTank());
            }
            // UP
            if(event->info.keyInfo.keyCode == 73) {
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), false);
                game_->getBoard()->snapTankToGrid(game_->getBoard()->getPlayerTank());
            }
            // LEFT
            if(event->info.keyInfo.keyCode == 72) {
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), false);
                game_->getBoard()->snapTankToGrid(game_->getBoard()->getPlayerTank());
            }
            // RIGHT
            if(event->info.keyInfo.keyCode == 71) {
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), false);
                game_->getBoard()->snapTankToGrid(game_->getBoard()->getPlayerTank());
            }
            // SPACEBAR
            if(event->info.keyInfo.keyCode == 57) {
            }
            break;
        }
        case (Event::TankKilled):{
            if(event->info.entityInfo.entity == game_->getBoard()->getPlayerTank())
                game_->setFinishedState();
            break;
        }
        case (Event::EntityEntityCollision):{
            // TANK 
            if (dynamic_cast<Tank*>(event->info.entityEntityCollisionInfo.entity1) != nullptr)
            {
                if (dynamic_cast<Tank*>(event->info.entityEntityCollisionInfo.entity2) != nullptr)
                {
                    // game_->getBoard()->snapTankToGrid(event->info.entityGridCollisionInfo.entity1);
                    // game_->getBoard()->snapTankToGrid(event->info.entityGridCollisionInfo.entity2); // TWO TANKS HITTING THEMSELVES
                }
                if (dynamic_cast<Bullet*>(event->info.entityEntityCollisionInfo.entity2) != nullptr)
                {
                    // BULLET HITTING TANK
                    if (event->info.entityEntityCollisionInfo.entity1 == game_->getBoard()->getPlayerTank()
                        // && event->info.entityEntityCollisionInfo.entity2->isFriendly() == false
                        )
                    {
                        // IF ENEMY BULLET HIT PLAYER'S TANK
                        game_->getBoard()->getPlayerTank()->deltaLives(-1);
                        game_->getBoard()->removeEntity(event->info.entityEntityCollisionInfo.entity2); // Kill bullet after hitting
                        if (game_->getBoard()->getPlayerTank()->getLives() == 0)
                        {
                            // TANK DIED
                        }
                    }
                    if (event->info.entityEntityCollisionInfo.entity1 != game_->getBoard()->getPlayerTank()
                        // && event->info.entityEntityCollisionInfo.entity2->isFriendly() == true
                        )
                    {
                        // IF FRIENDLY BULLET HIT ENEMY'S TANK
                        // game_->getStats()->addPoints(event->info.entityEntityCollisionInfo.entity1->getPoints()); // get points for killing a tank
                        // game_->getBoard()->removeEntity(event->info.entityEntityCollisionInfo.entity2) kill bullet after hit
                    }
                }
            }
            // BULLET
            if (dynamic_cast<Bullet*>(event->info.entityEntityCollisionInfo.entity1) != nullptr)
            {
                if (dynamic_cast<Bullet*>(event->info.entityEntityCollisionInfo.entity2) != nullptr)
                {
                    // TWO BULLETS HITTING THEMSELVES

                }
                if (dynamic_cast<Tank*>(event->info.entityEntityCollisionInfo.entity2) != nullptr)
                {
                    // BULLET HITTING TANK
                    if (event->info.entityEntityCollisionInfo.entity2 == game_->getBoard()->getPlayerTank()
                        // && event->info.entityEntityCollisionInfo.entity1->isFriendly() == false
                        )
                    {
                        // IF ENEMY BULLET HIT PLAYER'S TANK
                        game_->getBoard()->getPlayerTank()->deltaLives(-1);
                        // game_->getBoard()->removeEntity(event->info.entityEntityCollisionInfo.entity1) Kill bullet after hitting
                        if (game_->getBoard()->getPlayerTank()->getLives() == 0)
                        {
                            // TANK DIED
                        }
                    }
                    if (event->info.entityEntityCollisionInfo.entity2 != game_->getBoard()->getPlayerTank()
                        // && event->info.entityEntityCollisionInfo.entity1->isFriendly() == true
                        )
                    {
                        // IF FRIENDLY BULLET HIT ENEMY'S TANK
                            // game_->getStats()->addPoints(event->info.entityEntityCollisionInfo.entity2->getPoints()); // get points for killing a tank
                            // game_->getBoard()->removeEntity(event->info.entityEntityCollisionInfo.entity1) Kill bullet after hitting
                    }
                }
            }

            break;
        }
        case (Event::EntityGridCollision):{
            // TANK 
            if (dynamic_cast<Tank*>(event->info.entityGridCollisionInfo.entity) != nullptr)
            {
                // game_->getBoard()->snapTankToGrid(event->info.entityGridCollisionInfo.entity); // tank hitting the wall
            }
            // BULLET
            if (dynamic_cast<Bullet*>(event->info.entityGridCollisionInfo.entity) != nullptr)
            {
                unsigned int x = event->info.entityGridCollisionInfo.x;
                unsigned int y = event->info.entityGridCollisionInfo.y;
                game_->getBoard()->deleteTile(x, y);
                // game_->getBoard()-> Kill bullet after hitting
            }
            break;
        }
        case (Event::NullEvent):{
            break;
        }
        default:{
            throw InvalidEventException("Invalid event for ActiveEventHandler\nEvent enum cast: " + std::to_string(event->type));
        }
    }
>>>>>>> Added test samples for active state
}