//
// Created by tomek on 02.05.2022.
//

#include <utility>
#include <iostream>
#include <type_traits>

#include "../board-lib/include/Board.h"
#include "../tank-lib/include/Bullet.h"
#include "include/ActiveEventHandler.h"
#include "../bot-lib/include/BotController.h"
#include "include/Game.h"
#include "../board-lib/include/Eagle.h"
#include <cmath>

ActiveEventHandler::ActiveEventHandler(Game *game, ActiveGameState *state) {
    game_ = game;
    state_ = state;
}

void handleCollision(Event::PlayerTankCollisionInfo member1,
                     Event::PlayerTankCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::PlayerTankCollisionInfo member1,
                     Event::FriendlyBulletCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::EnemyTankCollisionInfo member1,
                     Event::PlayerTankCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::EnemyTankCollisionInfo member1,
                     Event::FriendlyBulletCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::EnemyTankCollisionInfo member1,
                     Event::EnemyBulletCollisionInfo member2, Game *game_) {
    throw "37645982734";
}

void handleCollision(Event::FriendlyBulletCollisionInfo member1,
                     Event::PlayerTankCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::FriendlyBulletCollisionInfo member1,
                     Event::FriendlyBulletCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::FriendlyBulletCollisionInfo member1,
                     Event::EnemyBulletCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::EnemyBulletCollisionInfo member1,
                     Event::PlayerTankCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::EnemyBulletCollisionInfo member1,
                     Event::EnemyTankCollisionInfo member2, Game *game_) {
    throw "24523452345";
}

void handleCollision(Event::EnemyBulletCollisionInfo member1,
                     Event::FriendlyBulletCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::EnemyBulletCollisionInfo member1,
                     Event::EnemyBulletCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::BoardCollisionInfo member1,
                     Event::PlayerTankCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::BoardCollisionInfo member1,
                     Event::EnemyTankCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::BoardCollisionInfo member1,
                     Event::FriendlyBulletCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::BoardCollisionInfo member1,
                     Event::EnemyBulletCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::BoardCollisionInfo member1,
                     Event::BoardCollisionInfo member2, Game *game_) {

}

void handleCollision(Event::EnemyTankCollisionInfo member1,
                     Event::BoardCollisionInfo member2, Game *game) {
    game->getBoard()->snapTankToGrid(member1.enemyTank);
}

void handleCollision(Event::EnemyTankCollisionInfo member1,
                     Event::EnemyTankCollisionInfo member2, Game *game) {
    game->getBoard()->snapTankToGrid(member1.enemyTank);
    game->getBoard()->snapTankToGrid(member2.enemyTank);
}

void handleCollision(Event::PlayerTankCollisionInfo member1,
                     Event::EnemyTankCollisionInfo member2, Game *game) {

    game->getBoard()->snapTankToGrid(member1.playerTank);
    game->getBoard()->snapTankToGrid(member2.enemyTank);
}

void handleCollision(Event::PlayerTankCollisionInfo member1,
                     Event::EnemyBulletCollisionInfo member2, Game *game) {
    game->getBoard()->hitTank(member1.playerTank, 1);
    game->getBoard()->removeEntity(member2.enemyBullet);
}

void handleCollision(Event::PlayerTankCollisionInfo member1,
                     Event::BoardCollisionInfo member2, Game *game) {
    game->getBoard()->snapTankToGrid(member1.playerTank);
}

void handleCollision(Event::FriendlyBulletCollisionInfo member1,
                     Event::EnemyTankCollisionInfo member2, Game *game) {
    game->getBoard()->removeEntity(member1.friendlyBullet);
    game->getStats()->addPoints(member2.enemyTank->getPoints());
    game->getBoard()->removeEntity(member2.enemyTank);
//    BotController::instance()->deregisterBot();
}

void handleCollision(Event::FriendlyBulletCollisionInfo member1,
                     Event::BoardCollisionInfo member2, Game *game) {
    game->getBoard()->removeEntity(member1.friendlyBullet);
    game->getBoard()->deleteTile(member2.tile_x, member2.tile_y);
    game->getBoard()->deleteTile(std::ceil(member2.tile_x + member1.friendlyBullet->getSizeX()), member2.tile_y);
    game->getBoard()->deleteTile(member2.tile_x, std::ceil(member2.tile_y + member1.friendlyBullet->getSizeY()));
    game->getBoard()->deleteTile(std::ceil(member2.tile_x + member1.friendlyBullet->getSizeX()), std::ceil(member2.tile_y + member1.friendlyBullet->getSizeY()));
    // game->getBoard()->deleteTile(member2.tile_x, member2.tile_y);

}

void handleCollision(Event::EnemyBulletCollisionInfo member1,
                     Event::BoardCollisionInfo member2, Game *game) {
    game->getBoard()->removeEntity(member1.enemyBullet);
    game->getBoard()->deleteTile(member2.tile_x, member2.tile_y);
    game->getBoard()->deleteTile(std::ceil(member2.tile_x + member1.enemyBullet->getSizeX()), member2.tile_y);
    game->getBoard()->deleteTile(member2.tile_x, std::ceil(member2.tile_y + member1.enemyBullet->getSizeY()));
    game->getBoard()->deleteTile(std::ceil(member2.tile_x + member1.enemyBullet->getSizeX()), std::ceil(member2.tile_y + member1.enemyBullet->getSizeY()));
}

// ######

void handleCollision(Event::EagleCollisionInfo member1,
                     Event::EnemyTankCollisionInfo member2, Game *game) {
    game->getBoard()->snapTankToGrid(member2.enemyTank);

}

void handleCollision(Event::EagleCollisionInfo member1,
                     Event::PlayerTankCollisionInfo member2, Game *game) {
    game->getBoard()->snapTankToGrid(member2.playerTank);

}

void handleCollision(Event::EagleCollisionInfo member1,
                     Event::FriendlyBulletCollisionInfo member2, Game *game) {
    member1.eagle->kill();
}

void handleCollision(Event::EagleCollisionInfo member1,
                     Event::EnemyBulletCollisionInfo member2, Game *game) {
    member1.eagle->kill();
}

void handleCollision(Event::EagleCollisionInfo member1,
                     Event::EagleCollisionInfo member2, Game *game) {
}

void handleCollision(Event::EagleCollisionInfo member1,
                     Event::BoardCollisionInfo member2, Game *game) {
}

void handleCollision(Event::EnemyTankCollisionInfo member1,
                     Event::EagleCollisionInfo member2,  Game *game) {
    handleCollision(member2, member1, game);
}

void handleCollision(Event::PlayerTankCollisionInfo member1,
                     Event::EagleCollisionInfo member2, Game *game) {
    handleCollision(member2, member1, game);
}

void handleCollision(Event::FriendlyBulletCollisionInfo member1,
                     Event::EagleCollisionInfo member2, Game *game) {
    handleCollision(member2, member1, game);
}

void handleCollision(Event::EnemyBulletCollisionInfo member1,
                     Event::EagleCollisionInfo member2, Game *game) {
    handleCollision(member2, member1, game);
}

void handleCollision(Event::BoardCollisionInfo member1,
                     Event::EagleCollisionInfo member2, Game *game) {
    handleCollision(member2, member1, game);
}


std::unique_ptr<Event> ActiveEventHandler::processEvent(std::unique_ptr<Event> event) {
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
       case (Event::PlayerKilled):{
            game_->end();
            break;
        }
        case (Event::BotDecisionRequest): {
            BotController::instance()->makeBotDecision(event->info.botInfo.bot);
            break;
        }
        case (Event::BotFireDecision): {
            game_->getBoard()->fireTank(std::dynamic_pointer_cast<Tank>(event->info.fireDecisionInfo.bot));
            break;
        }
        case (Event::BotMoveDecision): {
            game_->getBoard()->setTankMoving(std::dynamic_pointer_cast<Tank>(event->info.moveDecisionInfo.bot),
                                             event->info.moveDecisionInfo.flag);
            break;
        }
        case (Event::BotRotateDecision): {
            game_->getBoard()->setTankDirection(std::dynamic_pointer_cast<Tank>(event->info.rotateDecisionInfo.bot),
                                                event->info.rotateDecisionInfo.direction);
            break;
        }
        case (Event::BotSpawnDecision): {
            game_->getBoard()->spawnTank(event->info.spawnDecisionInfo.x, event->info.spawnDecisionInfo.y,
                                         event->info.spawnDecisionInfo.type, event->info.spawnDecisionInfo.direction);
            break;
        }
        case (Event::Collision): {
            Game *g = game_;
            std::visit([g](auto &&arg1, auto &&arg2) { handleCollision(arg1, arg2, g); },
                       event->info.collisionInfo.member1, event.get()->info.collisionInfo.member2);
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
           throw InvalidEventException("Invalid event for ActiveEventHandler\nEvent enum cast: " + std::to_string(event->type));
       }
   }
   return std::move(event);
}
