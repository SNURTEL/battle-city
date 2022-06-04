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
}