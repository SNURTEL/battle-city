//
// Created by tomek on 29.05.2022.
//

#include <optional>
#include <memory>

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/Board.h"
#include "../include/Grid.h"
#include "../include/GridBuilder.h"

#include "../../tank-lib/include/EntityController.h"
#include "../../tank-lib/include/Tank.h"
#include "../../tank-lib/include/Bullet.h"

#include "../../core-lib/include/Event.h"
#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Clock.h"

#include "../../bot-lib/include/BotController.h"

namespace {  // anonymous namespace to force internal linkage
    namespace helper {
        EventQueue<Event> *getEmptyEventQueue() {
            EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
            eventQueue->clear();
            return eventQueue;
        }

        class TestBoard : public Board {
        public:
            TestBoard(): Board() {
                clear();
            };

            std::shared_ptr<Entity> getLastEntity() {
                return entityController_->getAllEntities()->back();
            }

            Grid *getGrid() {
                return grid_.get();
            }

            std::vector<std::shared_ptr<Entity>> *getAllEntities() {
                return entityController_->getAllEntities();
            }

            bool testValidateEntityPosition(std::shared_ptr<Entity> target) {
                return validateEntityPosition(target);
            }

            std::unique_ptr<Event> testCreateCollisionEvent(std::shared_ptr<Entity> entity) {
                return std::move(createCollisionEvent(entity));
            }

            void clear() {
                setGrid(GridBuilder::buildLevel(9999));
            }
        };

        std::optional<std::shared_ptr<Tank>>
        placeTank(helper::TestBoard *board, unsigned int x, unsigned int y, Tank::TankType type,
                  Direction facing = North) {
            bool spawned = board->spawnTank(x, y, type, facing);

            if(!spawned){
                return std::nullopt;
            }

            auto tank = EventQueue<Event>::instance()->pop()->info.entityInfo.entity;

            std::shared_ptr<Tank> result = std::dynamic_pointer_cast<Tank>(tank);

            return result;
        }

        std::optional<std::shared_ptr<Bullet>> fireBullet(Board *board, std::shared_ptr<Tank> tank) {
            unsigned int prevSize = EventQueue<Event>::instance()->size();
            if (!board->fireTank(tank)) {
                return std::nullopt;
            }

            auto bullet = std::dynamic_pointer_cast<Bullet>(EventQueue<Event>::instance()->pop()->info.entityInfo.entity);

            if (prevSize + 1 == EventQueue<Event>::instance()->size()) {
                EventQueue<Event>::instance()->pop();
            }
            return bullet;
        }

        Grid *placeTile(helper::TestBoard *board, unsigned int x, unsigned int y, TileType type) {
            board->getGrid()->setTile(x, y, type);
            return EventQueue<Event>::instance()->pop()->info.tileInfo.grid;
        }

        void initSingletons(){
            Clock::initialize(60);
            BotController::initialize(4, 240);
            BotController::instance()->subscribe(Clock::instance());
        }
    }

}

SCENARIO("Spawning tanks") {
    helper::initSingletons();
    GIVEN("An empty board") {
        helper::TestBoard board{};

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Spawning tanks") {
            board.spawnTank(5, 10, Tank::FastTank, East);

            THEN("An event should be created") {
                auto event = eventQueue->pop();
                REQUIRE(event->type == Event::EntitySpawned);
                std::shared_ptr<Tank> spawnedTank = std::dynamic_pointer_cast<Tank>(board.getLastEntity());
                REQUIRE(event->info.entityInfo.entity == spawnedTank);

                AND_THEN("Tank's attrs should be set as expected") {
                    REQUIRE(spawnedTank->getX() == 5);
                    REQUIRE(spawnedTank->getY() == 10);
                    REQUIRE(spawnedTank->getFacing() == East);
                    REQUIRE(spawnedTank->getType() == Tank::FastTank);
                    REQUIRE(std::dynamic_pointer_cast<Tank>(spawnedTank) != nullptr);
                }
            }
        }

        WHEN("Spawning out of board (x>51 || y > 51)") {
            bool result = board.spawnTank(70, 34, Tank::PlayerTank);

            THEN("Operation should fail") {
                REQUIRE_FALSE(result);
            }
        }

        WHEN("Overlapping with another tank") {
            helper::placeTank(&board, 5, 5, Tank::PlayerTank);

            bool result = board.spawnTank(6, 6, Tank::ArmorTank);

            THEN("Operation should return false") {
                REQUIRE_FALSE(result);
            }
        }

        WHEN("Overlapping with a tile") {
            helper::placeTile(&board, 20, 30, Steel);

            bool result = board.spawnTank(19, 29, Tank::ArmorTank);

            THEN("Operation should fail") {
                REQUIRE_FALSE(result);
            }
        }
    }
}

SCENARIO("Rotating tanks at integer coords") {
    helper::initSingletons();
    GIVEN("A board with some tanks") {
        helper::TestBoard board{};
        std::shared_ptr<Tank> tank1 = helper::placeTank(&board, 5, 5, Tank::FastTank).value();

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Rotating a tank placed at integer coords") {
            board.setTankDirection(tank1, West);

            THEN("Tank should be rotated") {
                REQUIRE(tank1->getFacing() == West);
                AND_THEN("Two events should be created") {
                    REQUIRE_FALSE(eventQueue->isEmpty());
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == tank1);

                    REQUIRE_FALSE(eventQueue->isEmpty());
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankRotated);
                    REQUIRE(event->info.entityInfo.entity == tank1);

                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }
    }
}

SCENARIO("Rotating tanks an fractional coords") {
    helper::initSingletons();
    GIVEN("A board with some tanks") {
        helper::TestBoard board{};

        std::shared_ptr<Tank> tank1 = helper::placeTank(&board, 5, 5, Tank::FastTank).value();
        tank1->setY(17.8);

        std::shared_ptr<Tank> tank2 = helper::placeTank(&board, 8, 8, Tank::BasicTank, West).value();
        tank2->setX(9.2);

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Rotating a tank by 90 degrees") {
            board.setTankDirection(tank1, West);
            board.setTankDirection(tank2, South);

            THEN("Tank should be rotated and snapped to grid") {
                REQUIRE(tank1->getFacing() == West);
                REQUIRE(tank1->getX() == 5);
                REQUIRE(tank1->getY() == 18);

                REQUIRE(tank2->getFacing() == South);
                REQUIRE(tank2->getX() == 9);
                REQUIRE(tank2->getY() == 8);

                AND_THEN("Two events should be created") {
                    REQUIRE_FALSE(eventQueue->isEmpty());
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == tank1);

                    REQUIRE_FALSE(eventQueue->isEmpty());
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankRotated);
                    REQUIRE(event->info.entityInfo.entity == tank1);


                    REQUIRE_FALSE(eventQueue->isEmpty());
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == tank2);

                    REQUIRE_FALSE(eventQueue->isEmpty());
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankRotated);
                    REQUIRE(event->info.entityInfo.entity == tank2);

                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }WHEN("Rotating a tank by 180 degrees") {
            board.setTankDirection(tank1, South);
            board.setTankDirection(tank2, East);

            THEN("Tank should not be snapped to grid") {
                REQUIRE(tank1->getFacing() == South);
                REQUIRE(tank1->getX() == 5);
                REQUIRE(tank1->getY() == 17.8f);

                REQUIRE(tank2->getFacing() == East);
                REQUIRE(tank2->getX() == 9.2f);
                REQUIRE(tank2->getY() == 8);

                AND_THEN("Only TankRotated events should be created") {
                    REQUIRE_FALSE(eventQueue->isEmpty());
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankRotated);
                    REQUIRE(event->info.entityInfo.entity == tank1);
                    REQUIRE_FALSE(eventQueue->isEmpty());

                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankRotated);
                    REQUIRE(event->info.entityInfo.entity == tank2);

                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }

        WHEN("Rotating by 0 degrees") {
            board.setTankDirection(tank1, North);

            THEN("Tank's attrs should not change") {
                REQUIRE(tank1->getFacing() == North);
                REQUIRE(tank1->getX() == 5);
                REQUIRE(tank1->getY() == 17.8f);

                AND_THEN("No events should be created") {
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }

        WHEN("Cannot snap to grid - would collide with another entity") {
            std::shared_ptr<Tank> obstacleTank = helper::placeTank(&board, 5, 5, Tank::PlayerTank).value();
            obstacleTank->setY(19.9);

            board.setTankDirection(tank1, West);
            THEN("Tank should not be rotated") {
                REQUIRE(tank1->getX() == 5);
                REQUIRE(tank1->getY() == 17.8f);
                REQUIRE(tank1->getFacing() == North);
            }
        }
    }
}

SCENARIO("Detecting entity - grid collisions") {
    helper::initSingletons();
    GIVEN("A board") {
        helper::TestBoard board{};

        WHEN("An entity does not collide with any tile") {
            std::shared_ptr<Tank> tank = helper::placeTank(&board, 15, 20, Tank::PlayerTank).value();

            THEN("No collisions should be detected") {
                REQUIRE(board.testValidateEntityPosition(tank));
            }
        }

        WHEN("An entity is placed in board's upper left corner") {
            std::shared_ptr<Tank> tank = helper::placeTank(&board, 0, 0, Tank::PlayerTank).value();

            THEN("No collisions should be detected") {
                REQUIRE(board.testValidateEntityPosition(tank));
            }
        }

        WHEN("An entity is placed in board's bottom right corner") {
            std::shared_ptr<Tank> tank = helper::placeTank(&board, board.getSizeX() - 4, board.getSizeY() - 4,
                                                           Tank::PlayerTank).value();

            THEN("No collisions should be detected") {
                REQUIRE(board.testValidateEntityPosition(tank));
            }
        }

        WHEN("Trying to place an entity on map's edge") {
            auto tank = helper::placeTank(&board, 10, board.getSizeY() - 1, Tank::PlayerTank);

            THEN("Collision should be detected") {
                REQUIRE_FALSE(tank.has_value());
            }
        }

        WHEN("Trying to place an entity out of the map") {
            auto tank = helper::placeTank(&board, 10, board.getSizeY() + 20, Tank::PlayerTank);

            THEN("Entity should not be placed") {
                REQUIRE_FALSE(tank.has_value());
            }
        }
    }
}

SCENARIO("Moving a single entity") {
    helper::initSingletons();
    GIVEN("A board") {
        helper::TestBoard board{};

        auto eventQueue = helper::getEmptyEventQueue();
        WHEN("Movement would not result in a collision") {
            std::shared_ptr<Tank> tank = helper::placeTank(&board, 4, 8, Tank::PlayerTank).value();
            board.setTankMoving(tank, true);
            std::shared_ptr<Tank> stationaryTank = helper::placeTank(&board, 20, 22, Tank::PowerTank).value();
            board.setTankMoving(stationaryTank, false);
            auto b = helper::fireBullet(&board, stationaryTank);
            std::shared_ptr<Bullet> bullet = b.value();

            float initialBulletX = bullet->getX();
            float initialBulletY = bullet->getY();

            board.moveEntity(tank);
            board.moveEntity(stationaryTank);
            board.moveEntity(bullet);
            THEN("Bullets and tanks with moving flag set should be moved") {
                REQUIRE(tank->getX() == 4);
                REQUIRE(tank->getY() == 8 - tank->getSpeed());

                REQUIRE(stationaryTank->getX() == 20);
                REQUIRE(stationaryTank->getY() == 22);

                REQUIRE(bullet->getX() == initialBulletX);
                REQUIRE(bullet->getY() == initialBulletY - bullet->getSpeed());

                AND_THEN("Proper events should be created") {
                    REQUIRE_FALSE(eventQueue->isEmpty());
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == tank);

                    REQUIRE_FALSE(eventQueue->isEmpty());
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == bullet);




                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }

        WHEN("Movement would result in placing the entity right on the board's edge") {
            std::shared_ptr<Tank> tank1 = helper::placeTank(&board, 10, 10, Tank::BasicTank, East).value();
            board.setTankMoving(tank1, true);
            tank1->setX(board.getSizeX() - tank1->getSizeX() - tank1->getSpeed());

            std::shared_ptr<Tank> tank2 = helper::placeTank(&board, 10, 10, Tank::BasicTank, North).value();
            tank2->setY(0 + tank2->getSizeX() + tank2->getSpeed());
            board.setTankMoving(tank2, true);

            board.moveEntity(tank1);
            board.moveEntity(tank2);

            THEN("No collision should be reported") {
                REQUIRE_FALSE(eventQueue->isEmpty());
                auto event1 = eventQueue->pop();
                auto event2 = eventQueue->pop();
                REQUIRE(eventQueue->isEmpty());
            }
        }

        WHEN("Moving over map's border") {
            std::shared_ptr<Tank> tank1 = helper::placeTank(&board, 0, 10, Tank::BasicTank, West).value();
            board.setTankMoving(tank1, true);

            std::shared_ptr<Tank> tank2 = helper::placeTank(&board, 10, 10, Tank::BasicTank, South).value();
            tank2->setY(board.getSizeY() - tank2->getSizeY());
            board.setTankMoving(tank2, true);

            board.moveEntity(tank1);
            board.moveEntity(tank2);

            THEN("Collision events should be added to the queue") {
                REQUIRE(eventQueue->size() == 4);
                eventQueue->pop();
                auto collisionEvent = eventQueue->pop();
                REQUIRE(collisionEvent->type == Event::Collision);
                REQUIRE(std::get<Event::EnemyTankCollisionInfo>(collisionEvent->info.collisionInfo.member1).enemyTank ==
                        tank1);

                eventQueue->pop();
                collisionEvent = eventQueue->pop();
                REQUIRE(collisionEvent->type == Event::Collision);
                REQUIRE(std::get<Event::EnemyTankCollisionInfo>(collisionEvent->info.collisionInfo.member1).enemyTank ==
                        tank2);
                REQUIRE(eventQueue->isEmpty());

            }
        }

        WHEN("Two entities collide with each other (and it is not an enemy tank - enemy tank collision)") {
            std::shared_ptr<Tank> tank1 = helper::placeTank(&board, 10, 10, Tank::PlayerTank, South).value();
            board.setTankMoving(tank1, false);

            std::shared_ptr<Tank> tank2 = helper::placeTank(&board, 15, 10,
                                                            Tank::BasicTank, West).value();
            tank2->setX(10 + +static_cast<unsigned int>(tank1->getSizeX()) + 0.5);
            std::shared_ptr<Bullet> bullet = helper::fireBullet(&board, tank2).value();

            board.moveEntity(bullet);

            THEN("Collision event should be additionally added to the queue") {
                REQUIRE(eventQueue->size() == 2);
                REQUIRE(eventQueue->pop()->type == Event::EntityMoved);
                REQUIRE(eventQueue->pop()->type == Event::Collision);

            }
        }

        WHEN("Entity collides with grid") {
            std::shared_ptr<Tank> tank = helper::placeTank(&board, 10, 10, Tank::BasicTank, South).value();
            board.setTankMoving(tank, true);
            helper::placeTile(&board, 11, 12, TileType::Bricks);

            board.moveEntity(tank);
            THEN("Collision event should be added to the queue") {
                REQUIRE(eventQueue->size() == 2);
                eventQueue->pop();
                auto collisionEvent = eventQueue->pop();
                REQUIRE(collisionEvent->type == Event::Collision);
            }
        }
    }
}

SCENARIO("Removing all enemy tanks from the board") {
    helper::initSingletons();
    GIVEN("A board with some tanks and bullets") {
        helper::TestBoard board{};

        std::shared_ptr<Tank> basicTank = helper::placeTank(&board, 5, 5, Tank::BasicTank).value();
        std::shared_ptr<Bullet> basicBullet = helper::fireBullet(&board, basicTank).value();

        std::shared_ptr<Tank> playerTank = helper::placeTank(&board, 10, 10, Tank::PlayerTank).value();
        std::shared_ptr<Bullet> playerBullet = helper::fireBullet(&board, playerTank).value();

        std::shared_ptr<Tank> armorTank = helper::placeTank(&board, 15, 15, Tank::ArmorTank).value();
        std::shared_ptr<Bullet> armorBullet = helper::fireBullet(&board, armorTank).value();

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Calling the killAllEnemyEntities() method") {
            board.killAllEnemyEntities();

            THEN("Only friendly entities should remain on the board") {
                auto entityVector = board.getAllEntities();
                REQUIRE(entityVector->size() == 2);
                REQUIRE(std::dynamic_pointer_cast<PlayerTank>((*entityVector)[0]) != nullptr);
                REQUIRE(std::dynamic_pointer_cast<Bullet>((*entityVector)[1]) != nullptr);
                REQUIRE(std::dynamic_pointer_cast<Bullet>((*entityVector)[1])->isFriendly());

                AND_THEN("Proper events should be created") {
                    REQUIRE(eventQueue->size() == 4);

                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityRemoved);
                    REQUIRE(event->info.entityInfo.entity == armorBullet);

                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankKilled);
                    REQUIRE(event->info.entityInfo.entity == armorTank);

                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityRemoved);
                    REQUIRE(event->info.entityInfo.entity == basicBullet);

                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankKilled);
                    REQUIRE(event->info.entityInfo.entity == basicTank);
                }
            }
        }
    }
}

SCENARIO("Generating collision events") {
    helper::initSingletons();
    GIVEN("A board") {
        helper::TestBoard board{};

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Two enemy tanks collide") {
            auto tank1 = helper::placeTank(&board, 10, 10, Tank::BasicTank).value();
            auto tank2 = helper::placeTank(&board, 11, 11, Tank::ArmorTank).value();

            auto event = board.testCreateCollisionEvent(tank1);

            THEN("Event should be created as expected") {
                REQUIRE(event->type == Event::Collision);
                REQUIRE(std::get<Event::EnemyTankCollisionInfo>(event->info.collisionInfo.member1).enemyTank == tank1);
                REQUIRE(std::get<Event::EnemyTankCollisionInfo>(event->info.collisionInfo.member2).enemyTank == tank2);
            }
        }WHEN("An enemy tank collides with board") {
            auto tank1 = helper::placeTank(&board, 10, 10, Tank::BasicTank).value();
            helper::placeTile(&board, 11, 11, Bricks);

            auto event = board.testCreateCollisionEvent(tank1);

            THEN("Event should be created as expected") {
                REQUIRE(event->type == Event::Collision);
                REQUIRE(std::get<Event::EnemyTankCollisionInfo>(event->info.collisionInfo.member1).enemyTank == tank1);
                REQUIRE(std::get<Event::BoardCollisionInfo>(event->info.collisionInfo.member2).grid == board.getGrid());
            }
        }WHEN("An enemy tank collides with enemy bullet") {
            auto tank1 = helper::placeTank(&board, 10, 10, Tank::BasicTank).value();
            auto tank2 = helper::placeTank(&board, 10, 14, Tank::ArmorTank).value();
            auto bullet = helper::fireBullet(&board, tank2).value();

            auto event = board.testCreateCollisionEvent(tank1);

            THEN("Event should be created as expected") {
                REQUIRE(event->type == Event::Collision);
                REQUIRE(std::get<Event::EnemyTankCollisionInfo>(event->info.collisionInfo.member1).enemyTank ==
                        tank1);
                REQUIRE(std::get<Event::EnemyBulletCollisionInfo>(event->info.collisionInfo.member2).enemyBullet ==
                        bullet);
            }
        }WHEN("An enemy tank collides with friendly bullet") {
            auto tank1 = helper::placeTank(&board, 10, 10, Tank::BasicTank).value();
            auto tank2 = helper::placeTank(&board, 10, 14, Tank::PlayerTank).value();
            auto bullet = helper::fireBullet(&board, tank2).value();

            auto event = board.testCreateCollisionEvent(tank1);

            THEN("Event should be created as expected (friendly bullet should be the first member)") {
                REQUIRE(event->type == Event::Collision);
                REQUIRE(std::get<Event::FriendlyBulletCollisionInfo>(
                        event->info.collisionInfo.member1).friendlyBullet ==
                        bullet);
                REQUIRE(std::get<Event::EnemyTankCollisionInfo>(event->info.collisionInfo.member2).enemyTank ==
                        tank1);
            }
        }
        WHEN("Two enemy bullets collide") {
            auto tank1 = helper::placeTank(&board, 10, 10, Tank::BasicTank, East).value();
            auto tank2 = helper::placeTank(&board, 12, 12, Tank::BasicTank).value();
            tank2->setX(12.2);
            tank2->setY(12.2);
            auto bullet1 = helper::fireBullet(&board, tank1).value();
            auto bullet2 = helper::fireBullet(&board, tank2).value();

            auto event = board.testCreateCollisionEvent(bullet1);

            THEN("Event should be created as expected (friendly bullet should be the first member)") {
                REQUIRE(event->type == Event::Collision);
                REQUIRE(std::get<Event::EnemyBulletCollisionInfo>(
                        event->info.collisionInfo.member1).enemyBullet ==
                        bullet1);
                REQUIRE(std::get<Event::EnemyBulletCollisionInfo>(
                        event->info.collisionInfo.member2).enemyBullet ==
                        bullet2);
            }
        }WHEN("Enemy bullet collides with friendly bullet (friendly bullet should be the first member)") {
            auto tank1 = helper::placeTank(&board, 10, 10, Tank::BasicTank, East).value();
            auto tank2 = helper::placeTank(&board, 15, 10, Tank::PlayerTank, West).value();
            tank2->setX(14.5f);
            auto bullet1 = helper::fireBullet(&board, tank1).value();
            auto bullet2 = helper::fireBullet(&board, tank2).value();


            auto event = board.testCreateCollisionEvent(bullet1);

            THEN("Event should be created as expected (friendly bullet should be the first member)") {
                REQUIRE(event->type == Event::Collision);

                REQUIRE(std::get<Event::FriendlyBulletCollisionInfo>(
                        event->info.collisionInfo.member1).friendlyBullet ==
                        bullet2);
                REQUIRE(std::get<Event::EnemyBulletCollisionInfo>(
                        event->info.collisionInfo.member2).enemyBullet ==
                        bullet1);
            }
        }WHEN("Enemy bullet collides with grid") {
            auto tank1 = helper::placeTank(&board, 10, 10, Tank::BasicTank).value();
            helper::placeTile(&board, 11, 9, Steel);
            auto bullet = helper::fireBullet(&board, tank1).value();

            auto event = board.testCreateCollisionEvent(bullet);

            THEN("Event should be created as expected") {
                REQUIRE(event->type == Event::Collision);
                REQUIRE(std::get<Event::EnemyBulletCollisionInfo>(
                        event->info.collisionInfo.member1).enemyBullet ==
                        bullet);
                REQUIRE(std::get<Event::BoardCollisionInfo>(
                        event->info.collisionInfo.member2).grid ==
                        board.getGrid());
            }
        }WHEN("Friendly bullet collides with grid") {
            auto tank1 = helper::placeTank(&board, 10, 10, Tank::PlayerTank).value();
            helper::placeTile(&board, 11, 9, Steel);
            auto bullet = helper::fireBullet(&board, tank1).value();

            auto event = board.testCreateCollisionEvent(bullet);

            THEN("Event should be created as expected (friendly bullet should be the first member)") {
                REQUIRE(event->type == Event::Collision);
                REQUIRE(std::get<Event::FriendlyBulletCollisionInfo>(
                        event->info.collisionInfo.member1).friendlyBullet ==
                        bullet);
                REQUIRE(std::get<Event::BoardCollisionInfo>(
                        event->info.collisionInfo.member2).grid ==
                        board.getGrid());
            }
        }WHEN("Player collides with enemy bullet") {
            auto tank1 = helper::placeTank(&board, 10, 10, Tank::PlayerTank).value();
            auto tank2 = helper::placeTank(&board, 10, 14, Tank::BasicTank).value();
            auto bullet = helper::fireBullet(&board, tank2).value();

            auto event = board.testCreateCollisionEvent(tank1);

            THEN("Event should be created as expected (friendly bullet should be the first member)") {
                REQUIRE(event->type == Event::Collision);
                REQUIRE(std::get<Event::PlayerTankCollisionInfo>(
                        event->info.collisionInfo.member1).playerTank ==
                        tank1);
                REQUIRE(std::get<Event::EnemyBulletCollisionInfo>(event->info.collisionInfo.member2).enemyBullet ==
                        bullet);
            }
        }WHEN("Player collides with board") {
            auto tank1 = helper::placeTank(&board, 10, 10, Tank::PlayerTank).value();
            helper::placeTile(&board, 11, 11, Bricks);

            auto event = board.testCreateCollisionEvent(tank1);

            THEN("Event should be created as expected") {
                REQUIRE(event->type == Event::Collision);
                REQUIRE(std::get<Event::PlayerTankCollisionInfo>(event->info.collisionInfo.member1).playerTank == tank1);
                REQUIRE(std::get<Event::BoardCollisionInfo>(event->info.collisionInfo.member2).grid == board.getGrid());
            }
        }
    }
}
