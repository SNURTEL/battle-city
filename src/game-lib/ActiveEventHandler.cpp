//
// Created by tomek on 02.05.2022.
//

#include <utility>
#include <cmath>
#include <type_traits>

#include "../board-lib/include/Board.h"
#include "../tank-lib/include/Bullet.h"
#include "include/ActiveEventHandler.h"
#include "../bot-lib/include/BotController.h"
#include "include/Game.h"
#include "../board-lib/include/Eagle.h"
#include "../core-lib/include/ObserverExceptions.h"
#include "../board-lib/include/TileManager.h"

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
                     Event::EnemyBulletCollisionInfo member2, Game *game_) {
    if (member1.enemyTank->getBullet() == member2.enemyBullet.get()) {
        return;
    }
    try {
        game_->getBoard()->removeEntity(member2.enemyBullet);
    }
    catch (EntityDoesNotExistException) {}
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
    if (member2.enemyTank->getBullet() == member1.enemyBullet.get()) {
        return;
    }
    try {
        game_->getBoard()->removeEntity(member1.enemyBullet);
    }
    catch (EntityDoesNotExistException) {}
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
    game->getBoard()->setTankMoving(member1.enemyTank, false);

}

void handleCollision(Event::EnemyTankCollisionInfo member1,
                     Event::EnemyTankCollisionInfo member2, Game *game) {
    // disable collisions for enemy tanks
//   game->getBoard()->snapTankToGrid(member1.enemyTank);
//   game->getBoard()->snapTankToGrid(member2.enemyTank);
//   game->getBoard()->setTankMoving(member1.enemyTank, false);
//   game->getBoard()->setTankMoving(member2.enemyTank, false);
}

void handleCollision(Event::PlayerTankCollisionInfo member1,
                     Event::EnemyTankCollisionInfo member2, Game *game) {

    game->getBoard()->snapTankToGrid(member1.playerTank);
    game->getBoard()->snapTankToGrid(member2.enemyTank);
    game->getBoard()->setTankMoving(member1.playerTank, false);
    game->getBoard()->setTankMoving(member2.enemyTank, false);
}

void handleCollision(Event::PlayerTankCollisionInfo member1,
                     Event::EnemyBulletCollisionInfo member2, Game *game) {
    try {
        game->getBoard()->removeEntity(member2.enemyBullet);
        game->getStats()->decrementLives(1);
    }
    catch (EntityDoesNotExistException) {}
}

void handleCollision(Event::PlayerTankCollisionInfo member1,
                     Event::BoardCollisionInfo member2, Game *game) {
    game->getBoard()->snapTankToGrid(member1.playerTank);
    game->getBoard()->setTankMoving(member1.playerTank, false);
}

void handleCollision(Event::FriendlyBulletCollisionInfo member1,
                     Event::EnemyTankCollisionInfo member2, Game *game) {

    try {
        game->getBoard()->removeEntity(member1.friendlyBullet);
        game->getBoard()->hitTank(member2.enemyTank, 1);

    }
    catch (EntityDoesNotExistException &) {}
    catch (ObserverException &) {}
//    BotController::instance()->deregisterBot();
}

void handleCollision(Event::EnemyTankCollisionInfo member1,
                     Event::FriendlyBulletCollisionInfo member2, Game *game_) {
    handleCollision(member2, member1, game_);
}


void destroyBulletCollisionTiles(std::shared_ptr<Bullet> b, Game *game) {
    std::vector<std::pair<unsigned int, unsigned int>> tilesToDestroy{};

    if (b->getFacing() % 2 == 0) {
        // north, south
        auto y = static_cast<unsigned int>(b->getY() + (b->getFacing() == South ? b->getSizeY() : 0));
        auto baseX = static_cast<unsigned int>(b->getX());

        try {
            if (!TileManager::isTileDestructible(game->getBoard()->getGrid()->getTileAtPosition(baseX, y)) &&
                !TileManager::isTileDestructible(game->getBoard()->getGrid()->getTileAtPosition(baseX + 1, y))) {
                return;  // hit indestructible tiles
            }
        } catch (OutOfGridException &) {};


        for (int i = -1; i <= 2; i++) {
            tilesToDestroy.emplace_back(std::make_pair(baseX + i, y));
        }
    } else {
        // east, west
        auto x = static_cast<unsigned int>(b->getX() + (b->getFacing() == East ? b->getSizeX() : 0));
        auto baseY = static_cast<unsigned int>(b->getY());
        try {
            if (!TileManager::isTileDestructible(game->getBoard()->getGrid()->getTileAtPosition(x, baseY)) &&
                !TileManager::isTileDestructible(game->getBoard()->getGrid()->getTileAtPosition(x, baseY + 1))) {
                return;  // hit indestructible tiles
            }
        } catch (OutOfGridException &) {}


        for (int i = -1; i <= 2; i++) {
            tilesToDestroy.emplace_back(std::make_pair(x, baseY + i));
        }

    }

    for (std::pair<unsigned int, unsigned int> tileCoords: tilesToDestroy) {
        if (tileCoords.first > game->getBoard()->getSizeX() - 1 ||
            tileCoords.second > game->getBoard()->getSizeY() - 1) {
            continue;
        }
        game->getBoard()->deleteTile(tileCoords.first, tileCoords.second);
    }
}

void handleCollision(Event::FriendlyBulletCollisionInfo member1,
                     Event::BoardCollisionInfo member2, Game *game) {
    try {
        game->getBoard()->removeEntity(member1.friendlyBullet);
    }
    catch (EntityDoesNotExistException &) { return; }

    destroyBulletCollisionTiles(member1.friendlyBullet, game);
}

void handleCollision(Event::EnemyBulletCollisionInfo member1,
                     Event::BoardCollisionInfo member2, Game *game) {
    try {
        game->getBoard()->removeEntity(member1.enemyBullet);
    }
    catch (EntityDoesNotExistException &) { return; }

    destroyBulletCollisionTiles(member1.enemyBullet, game);
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
                     Event::EagleCollisionInfo member2, Game *game) {
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
        case (Event::KeyPressed): {
            if (event->info.keyInfo.keyCode == 74) {
                game_->getBoard()->setTankDirection(game_->getBoard()->getPlayerTank(), South);
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), true);
            }
            // UP
            if (event->info.keyInfo.keyCode == 73) {
                game_->getBoard()->setTankDirection(game_->getBoard()->getPlayerTank(), North);
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), true);
            }
            // LEFT
            if (event->info.keyInfo.keyCode == 71) {
                game_->getBoard()->setTankDirection(game_->getBoard()->getPlayerTank(), West);
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), true);
            }
            // RIGHT
            if (event->info.keyInfo.keyCode == 72) {
                game_->getBoard()->setTankDirection(game_->getBoard()->getPlayerTank(), East);
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), true);
            }
            // SPACEBAR
            if (event->info.keyInfo.keyCode == 57) {
                game_->getBoard()->fireTank(game_->getBoard()->getPlayerTank());
            }
            // ESCAPE
            if (event->info.keyInfo.keyCode == 36) {
                game_->setPauseState();
            }
            break;
        }
        case (Event::KeyReleased): {
            // DOWN
            if (event->info.keyInfo.keyCode == 74) {
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), false);
                //game_->getBoard()->snapTankToGrid(game_->getBoard()->getPlayerTank());
            }
            // UP
            if (event->info.keyInfo.keyCode == 73) {
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), false);
                //game_->getBoard()->snapTankToGrid(game_->getBoard()->getPlayerTank());
            }
            // LEFT
            if (event->info.keyInfo.keyCode == 72) {
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), false);
                //game_->getBoard()->snapTankToGrid(game_->getBoard()->getPlayerTank());
            }
            // RIGHT
            if (event->info.keyInfo.keyCode == 71) {
                game_->getBoard()->setTankMoving(game_->getBoard()->getPlayerTank(), false);
                //game_->getBoard()->snapTankToGrid(game_->getBoard()->getPlayerTank());
            }
            // SPACEBAR
            if (event->info.keyInfo.keyCode == 57) {
            }
            break;
        }
        case (Event::PlayerKilled): {
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
            if (!game_->getBoard()->spawnTank(event->info.spawnDecisionInfo.x,
                                              event->info.spawnDecisionInfo.y,
                                              event->info.spawnDecisionInfo.type,
                                              event->info.spawnDecisionInfo.direction)) {
                BotController::instance()->deregisterBot();
                BotController::instance()->zeroCooldown();
            }
            break;
        }
        case (Event::Collision): {
            Game *g = game_;
            std::visit([g](auto &&arg1, auto &&arg2) { handleCollision(arg1, arg2, g); },
                       event->info.collisionInfo.member1, event->info.collisionInfo.member2);
            break;
        }
        case (Event::TankKilled): {
            if (std::dynamic_pointer_cast<Tank>(event->info.entityInfo.entity) != nullptr)
                game_->getStats()->addPoints(
                        std::dynamic_pointer_cast<Tank>(event->info.entityInfo.entity)->getPoints());
            break;
        }
        case (Event::GameEnded): {
            game_->setFinishedState();
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
        case (Event::TankRotated):
        case (Event::TankHit):
        case (Event::MenuEnterClicked):
        case (Event::MenuSelectionChange):
        case (Event::NullEvent):
        case (Event::StatisticsChanged):
        case (Event::LevelLoaded): {
            break;
        }
        default: {
            throw InvalidEventException(
                    "Invalid event for ActiveEventHandler\nEvent enum cast: " + std::to_string(event->type));
        }
    }
    return std::move(event);
}
