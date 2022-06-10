//
// Created by tomek on 03.05.2022.
//

#include <memory>
#include <vector>

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

#include "../include/EntityController.h"
#include "../include/Tank.h"
#include "../include/Bullet.h"

namespace {  // anonymous namespace to force internal linkage
    namespace helper {  // helper classes and functions
        /**
         * Extended EntityController for testing purposes
         */
        class TestEntityController : public EntityController {
        public:
            TestEntityController() : EntityController() {
            }

            /**
             * Provides access to entity vector
             * @return A pointer to entity vector
             */
            std::vector<std::shared_ptr<Entity>> *getEntities() {
                return &entities_;
            }
        };

        /**
         * Silently spawns a tank without adding an event to the queue
         * @param controller EntityController instance to store the tank in
         * @param x Tank's initial X coord
         * @param y Tank's initial Y coord
         * @param type Tank's type
         * @param facing The direction in which the tank should be faced (defaults to North)
         * @return A new tank wrapped in a unique_ptr
         */
        std::shared_ptr<Tank> spawnTank(EntityController *controller, unsigned int x, unsigned int y, Tank::TankType type) {
            std::shared_ptr<Tank> tank = controller->createTank(x, y, type);
            return controller->addEntity(std::move(tank));
        }

        /**
         * Silently spawns a bullet without adding an event to the queue
         * Will not spawn a bullet if given tank already fired
         * @param controller EntityController instance to store the tank in
         * @param tank A tank that should fire the bullet
         * @return If successful, a pointer to the new bullet; if failed, an std::nullopt
         */
        std::optional<std::shared_ptr<Bullet>> spawnBullet(EntityController *controller, std::shared_ptr<Tank> tank) {
            auto bullet = tank->createBullet();
            if (!bullet.has_value()) {
                return std::nullopt;
            }
            return controller->addEntity(bullet.value());
        }

        /**
         * Clears the event queue and returns a pointer to it
         * @return A pointer to EventQueue<Event> sole instance
         */
        EventQueue<Event> *getEmptyEventQueue() {
            EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
            eventQueue->clear();
            return eventQueue;
        }
    }
}

SCENARIO("Creating tanks") {
    GIVEN("An empty entity controller") {
        helper::TestEntityController test_entityController{};
        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Requesting to create a tank") {
            std::shared_ptr<Tank> tank1 = test_entityController.createTank(12, 99, Tank::ArmorTank);

            THEN("A tank of correct type should be returned") {
                REQUIRE(std::dynamic_pointer_cast<ArmorTank>(tank1)!= nullptr);

                AND_THEN("All fields should be set as expected") {
                    REQUIRE(tank1->getX() == 12);
                    REQUIRE(tank1->getY() == 99);
                    REQUIRE(tank1->getType() == Tank::ArmorTank);
                    REQUIRE(tank1->getLives() == 4);
                    REQUIRE(tank1->isMoving() == false);
                    REQUIRE(tank1->getFacing() == North);
                }
            }
        }
    }
}

SCENARIO("Spawning entities") {
    GIVEN("An empty entity controller") {
        helper::TestEntityController test_entityController{};
        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Requesting to spawn a tank") {
            auto t1 = test_entityController.createTank(5, 5, Tank::BasicTank);
            std::shared_ptr<Tank> tank1 = test_entityController.addEntity(std::move(t1));

            auto t2 = test_entityController.createTank(20, 20, Tank::PowerTank);
            std::shared_ptr<Tank> tank2 = test_entityController.addEntity(std::move(t2));

            THEN("Tanks should be added to the controller") {
                REQUIRE(std::dynamic_pointer_cast<Tank>((*(test_entityController.getEntities()))[0])->getType()==
                        Tank::BasicTank);
                REQUIRE(std::dynamic_pointer_cast<Tank>((*(test_entityController.getEntities()))[1])->getType()==
                        Tank::PowerTank);
            }
        }

        AND_WHEN("Requesting to spawn a bullet") {
            auto t1 = test_entityController.createTank(5, 5, Tank::ArmorTank);
            std::shared_ptr<Bullet> b1 = t1->createBullet().value();

            std::shared_ptr<Bullet> bullet = test_entityController.addEntity(std::move(b1));

            THEN("Bullet should be added to the controller") {
                REQUIRE(std::dynamic_pointer_cast<Bullet>((*(test_entityController.getEntities()))[0]) != nullptr);
            }
        }

        WHEN("Spawning the player") {
            std::shared_ptr<Tank> t = test_entityController.createTank(5, 5, Tank::PlayerTank);
            auto pt = std::dynamic_pointer_cast<PlayerTank>(t);
            std::shared_ptr<PlayerTank> newTank = test_entityController.addEntity(pt);

            THEN("New tank should be stored in a separate place") {
                REQUIRE(test_entityController.getPlayer() == newTank);
                REQUIRE(test_entityController.getEntities()->back() == newTank);
            }
        }
    }
}

SCENARIO("Tank killed") {
    GIVEN("A controller with some tanks") {
        helper::TestEntityController test_entityController{};
        std::shared_ptr<Tank> tank = spawnTank(&test_entityController, 20, 20, Tank::PowerTank);
        std::shared_ptr<Tank> tank2 = spawnTank(&test_entityController, 15, 15, Tank::BasicTank);

        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Controller is requested to kill a tank") {
            test_entityController.killTank(tank);

            THEN("Tank should be removed from the pool") {
                REQUIRE(test_entityController.getEntities()->size() == 1);

                AND_THEN("TankKilled event should be added to the queue") {
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankKilled);
                    REQUIRE(event->info.entityInfo.entity == tank);
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }
    }
}

SCENARIO("Removing entities") {
    GIVEN("A controller with some entities") {
        helper::TestEntityController test_entityController{};
        std::shared_ptr<Tank> tank = spawnTank(&test_entityController, 5, 5, Tank::ArmorTank);
        std::shared_ptr<Tank> tank1 = spawnTank(&test_entityController, 10, 10, Tank::PowerTank);
        std::shared_ptr<Bullet> bullet = spawnBullet(&test_entityController, tank).value();

        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Controller is requested to remove an entity") {
            test_entityController.removeEntity(tank);
            test_entityController.removeEntity(bullet);

            THEN("Entities should be removed from the pool") {
                REQUIRE(test_entityController.getEntities()->size() == 1);

                AND_THEN("EntityRemoved events should be added to the queue") {
                    auto tankEvent = eventQueue->pop();
                    REQUIRE(tankEvent->type == Event::EntityRemoved);
                    REQUIRE(tankEvent->info.entityInfo.entity == tank);

                    auto bulletEvent = eventQueue->pop();
                    REQUIRE(bulletEvent->type == Event::EntityRemoved);
                    REQUIRE(bulletEvent->info.entityInfo.entity == bullet);

                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }
    }
}

SCENARIO("Tank hit") {
    GIVEN("A controller with some tanks") {
        helper::TestEntityController test_entityController{};
        std::shared_ptr<Tank> tank1 = spawnTank(&test_entityController, 5, 5, Tank::ArmorTank);
        std::shared_ptr<Tank> tank2 = spawnTank(&test_entityController, 20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Tank receives fewer damage points than it has health") {
            test_entityController.hitTank(tank1);

            THEN("Tank's health should be decreased") {
                REQUIRE(tank1->getLives() == 3);
                AND_THEN("An event should be added to the queue") {
                    REQUIRE(eventQueue->pop()->type == Event::TankHit);
                }
            }
        }

        WHEN("Damage is greater or equal than tank's health") {
            test_entityController.hitTank(tank2);

            THEN("Tank should be destroyed") {
                REQUIRE(test_entityController.getEntities()->size() == 1);

                AND_THEN("An event should be added to the queue") {
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankKilled);
                    REQUIRE(event->info.entityInfo.entity == tank2);
                }
            }
        }
    }
}

SCENARIO("Rotating tanks") {
    GIVEN("An entity controller with some tanks") {
        helper::TestEntityController test_entityController{};

        std::shared_ptr<Tank> tank1 = spawnTank(&test_entityController, 5, 5, Tank::FastTank);
        std::shared_ptr<Tank> tank2 = spawnTank(&test_entityController, 20, 20, Tank::PlayerTank);

        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Requesting to rotate a tank") {
            test_entityController.setTankDirection(tank1, East);

            THEN("The direction in which the tank is faced should be updated") {
                REQUIRE(tank1->getFacing() == East);
            }
        }
    }
}

SCENARIO("Moving a single entity ") {
    GIVEN("An entity controller with some entities") {
        helper::TestEntityController test_entityController{};

        std::shared_ptr<Tank> tank = spawnTank(&test_entityController, 5, 5, Tank::FastTank);
        test_entityController.setTankMoving(tank, true);
        test_entityController.setTankDirection(tank, South);

        std::shared_ptr<Tank> stationaryTank = spawnTank(&test_entityController, 20, 20, Tank::PlayerTank);
        test_entityController.setTankMoving(stationaryTank, false);
        test_entityController.setTankDirection(stationaryTank, East);

        std::shared_ptr<Bullet> bullet = spawnBullet(&test_entityController, stationaryTank).value();
        float initialBulletX = bullet->getX();
        float initialBulletY = bullet->getY();

        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Controller is requested to move a tank") {
            test_entityController.setTankMoving(tank, true);
            test_entityController.setTankDirection(tank, South);
            eventQueue->clear();
            test_entityController.moveEntity(tank);

            THEN("Tanks location should be updated") {
                REQUIRE(tank->getY() == 5 + tank->getSpeed());
                REQUIRE(tank->getFacing() == South);

                AND_THEN("An event should be added to the queue") {
                    std::unique_ptr<Event> event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == tank);
                }
            }
        }

        WHEN("Controller is requested to move a tank but moving flag is not set") {
            test_entityController.moveEntity(stationaryTank);

            THEN("Tank should remain stationary") {
                REQUIRE(stationaryTank->getX() == 20);
                REQUIRE(stationaryTank->getY() == 20);

                AND_THEN("No events should be created") {
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }

        WHEN("Moving a bullet") {
            test_entityController.moveEntity(bullet);

            THEN("Bullet's loacion should be updated") {
                REQUIRE(bullet->getX() == initialBulletX + bullet->getSpeed());
                REQUIRE(bullet->getFacing() == East);

                AND_THEN("An event should be added to the queue") {
                    std::unique_ptr<Event> event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == bullet);

                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }
    }
}

SCENARIO("Moving all entities at once") {
    GIVEN("A entity controller with some entities") {
        helper::TestEntityController test_entityController{};

        std::shared_ptr<Tank> tank = spawnTank(&test_entityController, 5, 5, Tank::FastTank);
        test_entityController.setTankMoving(tank, true);
        test_entityController.setTankDirection(tank, South);

        std::shared_ptr<Tank> stationaryTank = spawnTank(&test_entityController, 20, 20, Tank::PlayerTank);
        test_entityController.setTankMoving(stationaryTank, false);
        test_entityController.setTankDirection(stationaryTank, East);

        std::shared_ptr<Bullet> bullet = spawnBullet(&test_entityController, stationaryTank).value();
        float initialBulletX = bullet->getX();
        float initialBulletY = bullet->getY();

        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Calling the .moveAllEntities() method") {
            test_entityController.moveAllEntities();

            THEN("Tanks with moving flag set should be moved") {
                REQUIRE(tank->getY() == 5 + tank->getSpeed());
                REQUIRE(tank->getFacing() == South);
            }

            THEN("Stationary tanks should not be moved") {
                REQUIRE(stationaryTank->getX() == 20);
                REQUIRE(stationaryTank->getY() == 20);
            }

            THEN("Bullets should be moved unconditionally") {
                REQUIRE(bullet->getX() == initialBulletX + bullet->getSpeed());
                REQUIRE(bullet->getFacing() == East);
            }

            THEN("Events representing all moved entities should be added to the queue") {
                std::unique_ptr<Event> event = eventQueue->pop();
                REQUIRE(event->type == Event::EntityMoved);
                REQUIRE(event->info.entityInfo.entity == tank);

                event = eventQueue->pop();
                REQUIRE(event->type == Event::EntityMoved);
                REQUIRE(event->info.entityInfo.entity == bullet);

                REQUIRE(eventQueue->isEmpty());
            }
        }
    }
}

SCENARIO("Finding entities by their position") {
    GIVEN("A controller with some entities") {
        helper::TestEntityController test_entityController{};

        std::shared_ptr<Tank> tank1 = spawnTank(&test_entityController, 5, 5, Tank::ArmorTank);
        std::shared_ptr<Tank> tank2 = spawnTank(&test_entityController, 20, 20, Tank::PowerTank);
        std::shared_ptr<Bullet> bullet = spawnBullet(&test_entityController, tank1).value();

        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Trying to find an entity at it's exact location") {
            std::optional<std::shared_ptr<Entity> > found = test_entityController.findEntityAtPosition(bullet->getX(), bullet->getY());

            THEN("A correct entity should be returned") {
                REQUIRE(found.has_value());
                REQUIRE(std::dynamic_pointer_cast<Bullet>(found.value()) != nullptr);
            }
        }

        WHEN("Trying to find an entity in it's bounding box") {
            std::optional<std::shared_ptr<Entity> > found = test_entityController.findEntityAtPosition(21, 21);

            THEN("A correct entity should be returned") {
                REQUIRE(found.has_value());
                REQUIRE(std::dynamic_pointer_cast<Tank>(found.value()) != nullptr);
            }
        }

        WHEN("Trying to find an entity at the lower right corner of it's bounding box") {
            std::optional<std::shared_ptr<Entity> > found = test_entityController.findEntityAtPosition(21.999, 21.999);

            THEN("A correct entity should be returned") {
                REQUIRE(found.has_value());
                REQUIRE(std::dynamic_pointer_cast<Tank>(found.value()) != nullptr);
            }
        }

        WHEN("Trying to find an entity in a place where there's no tank") {
            std::optional<std::shared_ptr<Entity> > found = test_entityController.findEntityAtPosition(90, 90);

            THEN("No entity should be returned") {
                REQUIRE_FALSE(found.has_value());
            }
        }

        WHEN("Trying to find an entity next to it's edge") {
            std::optional<std::shared_ptr<Entity> > found = test_entityController.findEntityAtPosition(24, 21);

            THEN("No entity should be returned") {
                REQUIRE_FALSE(found.has_value());
            }

            found = test_entityController.findEntityAtPosition(19.999, 19.999);

            THEN("No entity should be returned") {
                REQUIRE_FALSE(found.has_value());
            }
        }

        WHEN("Trying to find an entity at a point located right between two entities") {
            std::optional<std::shared_ptr<Entity> > found = test_entityController.findEntityAtPosition(5, 5 + (tank1->getSizeY() -
                                                                                               bullet->getSizeY()) / 2);

            THEN("Entity on the right should be returned") {

                REQUIRE(found.has_value());
                REQUIRE(std::dynamic_pointer_cast<Tank>(found.value()) != nullptr);
            }

        }

    }
}

SCENARIO("Collision detection") {
    GIVEN("An entity controller with some entities") {
        helper::TestEntityController test_entityController{};

        std::shared_ptr<Tank> tank1 = spawnTank(&test_entityController, 5, 5, Tank::ArmorTank);
        std::shared_ptr<Bullet> bullet1 = spawnBullet(&test_entityController, tank1).value();

        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Two objects are away from each other") {
            THEN("No collision should be detected") {
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(tank1));
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(bullet1));
            }
        }WHEN("Two objects overlap") {
            std::shared_ptr<Tank> tank2 = spawnTank(&test_entityController, 6, 6, Tank::PlayerTank);

            THEN("A collision should be detected") {
                REQUIRE(test_entityController.checkEntityCollisions(tank2).has_value());
                REQUIRE(test_entityController.checkEntityCollisions(tank1).has_value());
            }
        }WHEN("One objects sits right on other's edge (horizontally)") {
            std::shared_ptr<Tank> tank2 = spawnTank(&test_entityController, 9, 5, Tank::PlayerTank);

            THEN("Np collision should be detected") {
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(tank1));
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(tank2));
            }
        }WHEN("One objects sits right on other's edge (vertically)") {
            std::shared_ptr<Tank> tank2 = spawnTank(&test_entityController, 5, 9, Tank::PlayerTank);

            THEN("Np collision should be detected") {
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(tank1));
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(tank2));
            }
        }WHEN("One object is located inside another"){
            tank1->setX(5);
            tank1->setY(3);

            THEN("A collision should be detected") {
                REQUIRE(test_entityController.checkEntityCollisions(tank1).has_value());
                REQUIRE(test_entityController.checkEntityCollisions(bullet1).has_value());
            }
        }
        eventQueue->clear();

    }
}

SCENARIO("Clearing the board") {
    GIVEN("An entity controller with some contents") {
        helper::TestEntityController test_entityController{};

        std::shared_ptr<Tank> tank1 = spawnTank(&test_entityController, 5, 5, Tank::ArmorTank);
        std::shared_ptr<Tank> tank2 = spawnTank(&test_entityController, 20, 20, Tank::PowerTank);
        std::shared_ptr<Bullet> bullet = spawnBullet(&test_entityController, tank1).value();

        EventQueue<Event> *eventQueue = helper::getEmptyEventQueue();

        WHEN("Calling the .clear() method") {
            test_entityController.clear();

            THEN("All entities should be removed") {
                REQUIRE(test_entityController.getAllEntities()->empty());

                AND_THEN("EntityRemoved events should be created") {
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityRemoved);
                    REQUIRE(event->info.entityInfo.entity == bullet);
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityRemoved);
                    REQUIRE(event->info.entityInfo.entity == tank2);
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityRemoved);
                    REQUIRE(event->info.entityInfo.entity == tank1);
                }
            }
        }
    }
}