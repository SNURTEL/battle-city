//
// Created by tomek on 29.05.2022.
//

#include <optional>
#include <memory>

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/Board.h"
#include "../include/Grid.h"

#include "../../tank-lib/include/EntityController.h"
#include "../../tank-lib/include/Tank.h"
#include "../../tank-lib/include/Bullet.h"

#include "../../core-lib/include/Event.h"
#include "../../core-lib/include/EventQueue.h"

namespace {  // anonymous namespace to force internal linkage
    namespace helper {
        EventQueue<Event> *getEmptyEventQueue() {
            EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
            eventQueue->clear();
            return eventQueue;
        }

        class TestBoard : public Board {
        public:
            Entity *getLastEntity() {
                return entityController_->getAllEntities()->back().get();
            }

            Grid *getGrid() {
                return grid_.get();
            }

            std::vector<std::unique_ptr<Entity>> *getAllEntities() {
                return entityController_->getAllEntities();
            }

            bool testValidateEntityPosition(Entity *target) {
                return validateEntityPosition(target);
            }
        };

        Tank *
        placeTank(helper::TestBoard *board, unsigned int x, unsigned int y, Tank::TankType type,
                  Direction facing = North) {
            bool collision = !board->spawnTank(x, y, type, facing);

            Tank *result = dynamic_cast<Tank *>(EventQueue<Event>::instance()->pop()->info.entityInfo.entity);
            if (collision)
                EventQueue<Event>::instance()->pop();

            return result;
        }

        std::optional<Bullet *> fireBullet(Board *board, Tank *tank) {
            if (!board->fireTank(tank)) {
                return std::nullopt;
            }
            return dynamic_cast<Bullet *>(EventQueue<Event>::instance()->pop()->info.entityInfo.entity);
        }

        Grid *placeTile(helper::TestBoard *board, unsigned int x, unsigned int y, TileType type) {
            board->getGrid()->setTile(x, y, type);
            return EventQueue<Event>::instance()->pop()->info.tileInfo.grid;
        }
    }

}

SCENARIO("Spawning tanks") {
    GIVEN("An empty board") {
        helper::TestBoard board{};

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Spawning tanks") {
            board.spawnTank(5, 10, Tank::FastTank, East);

            THEN("An event should be created") {
                auto event = eventQueue->pop();
                REQUIRE(event->type == Event::EntitySpawned);
                Tank *spawnedTank = dynamic_cast<Tank *>(board.getLastEntity());
                REQUIRE(event->info.entityInfo.entity == spawnedTank);

                AND_THEN("Tank's attrs should be set as expected") {
                    REQUIRE(spawnedTank->getX() == 5);
                    REQUIRE(spawnedTank->getY() == 10);
                    REQUIRE(spawnedTank->getFacing() == East);
                    REQUIRE(spawnedTank->getType() == Tank::FastTank);
                    REQUIRE(dynamic_cast<FastTank *>(spawnedTank) != nullptr);
                }
            }
        }

        WHEN("Spawning out of board (x>51 || y > 51)") {
            bool result = board.spawnTank(70, 34, Tank::PlayerTank);

            THEN("Operation should fail") {
                REQUIRE_FALSE(result);
            }THEN("A collision event should be created") {
                eventQueue->pop();
                auto event = eventQueue->pop();
                REQUIRE(event->type == Event::EntityGridCollision);
            }
        }

        WHEN("Overlapping with another tank") {
            helper::placeTank(&board, 5, 5, Tank::BasicTank);

            bool result = board.spawnTank(6, 6, Tank::ArmorTank);

            THEN("Operation should fail") {
                REQUIRE_FALSE(result);
            }THEN("A collision event should be created") {
                eventQueue->pop();
                auto event = eventQueue->pop();
                REQUIRE(event->type == Event::EntityEntityCollision);
            }
        }

        WHEN("Overlapping with a tile") {
            helper::placeTile(&board, 20, 30, Steel);

            bool result = board.spawnTank(19, 29, Tank::ArmorTank);

            THEN("Operation should fail") {
                REQUIRE_FALSE(result);
            }THEN("A collision event should be created") {
                eventQueue->pop();
                auto event = eventQueue->pop();
                REQUIRE(event->type == Event::EntityGridCollision);
            }
        }
    }
}

SCENARIO("Rotating tanks an integer coords") {
    GIVEN("A board with some tanks") {
        helper::TestBoard board{};
        Tank *tank1 = helper::placeTank(&board, 5, 5, Tank::FastTank);

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Rotating a tank placed at integer coords") {
            board.setTankDirection(tank1, West);

            THEN("Tank should be rotated") {
                REQUIRE(tank1->getFacing() == West);
            }THEN("Two events should be created") {
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

SCENARIO("Rotating tanks an fractional coords") {
    GIVEN("A board with some tanks") {
        helper::TestBoard board{};

        Tank *tank1 = helper::placeTank(&board, 5, 5, Tank::FastTank);
        tank1->setY(17.8);

        Tank *tank2 = helper::placeTank(&board, 8, 8, Tank::BasicTank, West);
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
            Tank *obstacleTank = helper::placeTank(&board, 5, 5, Tank::FastTank);
            obstacleTank->setY(19.9);

            board.setTankDirection(tank1, West);
            THEN("Tank should not be rotated, nor snapped to grid") {
                REQUIRE(tank1->getX() == 5);
                REQUIRE(tank1->getY() == 17.8f);
                REQUIRE(tank1->getFacing() == North);

                AND_THEN("No events should be created") {
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }
    }
}

SCENARIO("Detecting entity - grid collisions") {
    GIVEN("A board") {
        helper::TestBoard board{};

        WHEN("An entity does not collide with any tile") {
            Tank *tank = helper::placeTank(&board, 15, 20, Tank::PlayerTank);

            THEN("No collisions should be detected") {
                REQUIRE(board.testValidateEntityPosition(tank));
            }
        }

        WHEN("An entity is placed in board's upper left corner") {
            Tank *tank = helper::placeTank(&board, 0, 0, Tank::PlayerTank);

            THEN("No collisions should be detected") {
                REQUIRE(board.testValidateEntityPosition(tank));
            }
        }

        WHEN("An entity is placed in board's bottom right corner") {
            Tank *tank = helper::placeTank(&board, board.getSizeX() - 4, board.getSizeY() - 4, Tank::PlayerTank);

            THEN("No collisions should be detected") {
                REQUIRE(board.testValidateEntityPosition(tank));
            }
        }

        WHEN("An entity is placed on map's edge") {
            Tank *tank = helper::placeTank(&board, 10, board.getSizeY() - 1, Tank::PlayerTank);

            THEN("Collision should be detected") {
                REQUIRE_FALSE(board.testValidateEntityPosition(tank));
            }
        }

        WHEN("An entity is placed out of map") {
            Tank *tank = helper::placeTank(&board, 10, board.getSizeY() + 20, Tank::PlayerTank);

            THEN("Collision should be detected") {
                REQUIRE_FALSE(board.testValidateEntityPosition(tank));
            }
        }
    }
}

SCENARIO("Moving a single entity") {
    GIVEN("A board") {
        helper::TestBoard board{};

        auto eventQueue = helper::getEmptyEventQueue();
        WHEN("Movement would not result in a collision") {
            Tank *tank = helper::placeTank(&board, 4, 8, Tank::PlayerTank);
            board.setTankMoving(tank, true);
            Tank *stationaryTank = helper::placeTank(&board, 20, 22, Tank::PowerTank);
            board.setTankMoving(stationaryTank, false);
            auto b = helper::fireBullet(&board, stationaryTank);
            Bullet *bullet = b.value();

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
            Tank *tank1 = helper::placeTank(&board, 10, 10, Tank::BasicTank, East);
            board.setTankMoving(tank1, true);
            tank1->setX(board.getSizeX() - tank1->getSizeX() - tank1->getSpeed());

            Tank *tank2 = helper::placeTank(&board, 10, 10, Tank::BasicTank, North);
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
            Tank *tank1 = helper::placeTank(&board, 0, 10, Tank::BasicTank, West);
            board.setTankMoving(tank1, true);

            Tank *tank2 = helper::placeTank(&board, 10, 10, Tank::BasicTank, South);
            tank2->setY(board.getSizeY() - tank2->getSizeY());
            board.setTankMoving(tank2, true);

            board.moveEntity(tank1);
            board.moveEntity(tank2);

            THEN("Collision events should be added to the queue") {
                REQUIRE(eventQueue->size() == 4);
                eventQueue->pop();
                auto collisionEvent = eventQueue->pop();
                REQUIRE(collisionEvent->type == Event::EntityGridCollision);
                REQUIRE(collisionEvent->info.entityGridCollisionInfo.entity == tank1);

                eventQueue->pop();
                collisionEvent = eventQueue->pop();
                REQUIRE(collisionEvent->type == Event::EntityGridCollision);
                REQUIRE(collisionEvent->info.entityGridCollisionInfo.entity == tank2);

            }
        }

        WHEN("Two entities collide with each other") {
            Tank *tank1 = helper::placeTank(&board, 10, 10, Tank::BasicTank, South);
            board.setTankMoving(tank1, false);

            Tank *tank2 = helper::placeTank(&board, 10, 10,
                                            Tank::BasicTank, North);
            tank2->setY(10 + + static_cast<unsigned int>(tank1->getSizeY()) + 0.4);
            Bullet *bullet = helper::fireBullet(&board, tank2).value();

            board.moveEntity(bullet);

            THEN("Collision event should be added to the queue") {
                REQUIRE(eventQueue->size() == 2);
                eventQueue->pop();
                auto collisionEvent = eventQueue->pop();
                REQUIRE(collisionEvent->type == Event::EntityEntityCollision);
                REQUIRE(collisionEvent->info.entityEntityCollisionInfo.entity1 == bullet);
                REQUIRE(collisionEvent->info.entityEntityCollisionInfo.entity2 == tank1);
            }
        }

        WHEN("Entity collides with grid") {
            Tank *tank = helper::placeTank(&board, 10, 10, Tank::BasicTank, South);
            board.setTankMoving(tank, true);
            helper::placeTile(&board, 11, 12, TileType::Bricks);

            board.moveEntity(tank);
            THEN("Collision event should be added to the queue") {
                REQUIRE(eventQueue->size() == 2);
                eventQueue->pop();
                auto collisionEvent = eventQueue->pop();
                REQUIRE(collisionEvent->type == Event::EntityGridCollision);
            }
        }
    }
}

SCENARIO("Removing all enemy tanks from the board") {
    GIVEN("A board with some tanks and bullets") {
        helper::TestBoard board{};

        Tank *basicTank = helper::placeTank(&board, 5, 5, Tank::BasicTank);
        Bullet *basicBullet = helper::fireBullet(&board, basicTank).value();

        Tank *playerTank = helper::placeTank(&board, 10, 10, Tank::PlayerTank);
        Bullet *playerBullet = helper::fireBullet(&board, playerTank).value();

        Tank *armorTank = helper::placeTank(&board, 15, 15, Tank::ArmorTank);
        Bullet *armorBullet = helper::fireBullet(&board, armorTank).value();

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Calling the killAllEnemyEntities() method") {
            board.killAllEnemyEntities();

            THEN("Only friendly entities should remain on the board") {
                auto entityVector = board.getAllEntities();
                REQUIRE(entityVector->size() == 2);
                REQUIRE(dynamic_cast<PlayerTank *>((*entityVector)[0].get()) != nullptr);
                REQUIRE(dynamic_cast<Bullet *>((*entityVector)[1].get()) != nullptr);
                REQUIRE(dynamic_cast<Bullet *>((*entityVector)[1].get())->isFriendly());

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