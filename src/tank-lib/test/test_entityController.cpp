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


class TestTankController : public EntityController {
public:
    TestTankController() : EntityController() {
    }

    std::vector<std::unique_ptr<Entity>> * getEntities() {
        return &entities_;
    }
};

SCENARIO("Spawning tanks") {
    GIVEN("An empty entity controller") {
        TestTankController test_entityController{};
        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();

        WHEN("Requesting to spawn a tank") {
            auto newTank1 = test_entityController.spawnTank(5, 5, Tank::BasicTank);
            auto newTank2 = test_entityController.spawnTank(20, 20, Tank::PowerTank);

            THEN("Tanks should be created") {
                REQUIRE(dynamic_cast<Tank*>((*(test_entityController.getEntities()))[0].get())->getType() == Tank::BasicTank);
                REQUIRE(dynamic_cast<Tank*>((*(test_entityController.getEntities()))[1].get())->getType() == Tank::PowerTank);

                AND_THEN("Tank creation events should be added to the pool") {
                    auto event1 = eventQueue->pop();
                    auto event2 = eventQueue->pop();

                    REQUIRE(event1->type == Event::EntitySpawned);
                    REQUIRE(event1->info.entityInfo.entity == newTank1);
                    REQUIRE(event2->type == Event::EntitySpawned);
                    REQUIRE(event2->info.entityInfo.entity == newTank2);
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }

        WHEN("Spawning the player") {
            auto newTank = test_entityController.spawnTank(5, 5, Tank::PlayerTank);

            THEN("New tank should be stored in a separate place") {
                REQUIRE(test_entityController.getPlayer() == newTank);
                REQUIRE(test_entityController.getEntities()->back().get() == newTank);

                AND_THEN("A correct event should be added to the pool") {
                    auto event = eventQueue->pop();
                    REQUIRE(eventQueue->isEmpty());

                    REQUIRE(event->type == Event::EntitySpawned);
                    REQUIRE(event->info.entityInfo.entity == newTank);
                }
            }
        }

    }
}

SCENARIO("Tank killed") {
    GIVEN("A controller with some tanks") {
        TestTankController test_entityController{};
        test_entityController.spawnTank(5, 5, Tank::ArmorTank);
        test_entityController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->pop();
        eventQueue->pop();

        WHEN("Controller is requested to kill a tank") {
            Tank *target = dynamic_cast<Tank*>((*(test_entityController.getEntities()))[0].get());
            test_entityController.killTank(target);

            THEN("Tank should be removed from the pool") {
                REQUIRE(test_entityController.getEntities()->size() == 1);

                AND_THEN("TankKilled event should be added to the queue") {
                    REQUIRE(eventQueue->pop()->type == Event::TankKilled);
                }
            }
        }
    }
}

SCENARIO("Tank removed") {
    GIVEN("A controller with some tanks") {
        TestTankController test_entityController{};
        test_entityController.spawnTank(5, 5, Tank::ArmorTank);
        test_entityController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Controller is requested to remove a tank") {
            Tank *target = dynamic_cast<Tank*>((*(test_entityController.getEntities()))[0].get());
            test_entityController.killTank(target);

            THEN("Tank should be removed from the pool") {
                REQUIRE(test_entityController.getEntities()->size() == 1);

                AND_THEN("TankRemoved event should be added to the queue") {
                    REQUIRE(eventQueue->pop()->type == Event::TankKilled);
                }
            }
        }
    }
}

SCENARIO("Tank hit") {
    GIVEN("A controller with some tanks") {
        TestTankController test_entityController{};
        test_entityController.spawnTank(5, 5, Tank::ArmorTank);
        test_entityController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Tank receives fewer damage points than it has health") {
            Tank *target = dynamic_cast<Tank*>((*(test_entityController.getEntities()))[0].get());
            test_entityController.hitTank(target);

            THEN("Tank's health should be decreased") {
                REQUIRE(target->getLives() == 3);
                AND_THEN("An event should be added to the queue") {
                    REQUIRE(eventQueue->pop()->type == Event::TankHit);
                }
            }
        }

        WHEN("Damage is greater or equal than tank's health") {
            Tank *target = dynamic_cast<Tank*>((*(test_entityController.getEntities()))[1].get());
            test_entityController.hitTank(target);

            THEN("Tank should be destroyed") {
                REQUIRE(test_entityController.getEntities()->size() == 1);

                AND_THEN("An event should be added to the queue") {
                    REQUIRE(eventQueue->pop()->type == Event::TankKilled);
                }
            }
        }
    }
}

SCENARIO("Moving a tank") {
    GIVEN("A tank controller with some tanks") {
        TestTankController test_entityController{};
        auto testTank1 = test_entityController.spawnTank(5, 5, Tank::ArmorTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Rotating tanks"){
            test_entityController.setTankDirection(testTank1, East);

            THEN("The direction in which the tank is faced should be updated"){
                REQUIRE(testTank1->getFacing()==East);

                AND_THEN("An event should be added to the queue"){
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankRotated);
                    REQUIRE(event->info.entityInfo.entity == testTank1);

                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }

        WHEN("Controller is requested to move a tank") {
            test_entityController.setTankMoving(testTank1, true);
            test_entityController.setTankDirection(testTank1, South);
            eventQueue->pop();
            test_entityController.moveEntity(testTank1);

            THEN("Tanks location an orientation should be updated") {
                REQUIRE(testTank1->getY() == 5 + testTank1->getSpeed());
                REQUIRE(testTank1->getFacing() == South);

                AND_THEN("An event should be added to the pool") {
                    std::unique_ptr<Event> event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == testTank1);
                }
            }
        }
    }
}

SCENARIO("Moving all tanks at once") {
    GIVEN("A tank controller with some tanks") {
        TestTankController testTankController{};
        auto testTank1 = testTankController.spawnTank(5, 5, Tank::BasicTank);
        testTank1->setFacing(North);
        testTank1->setMoving(true);
        auto testTank2 = testTankController.spawnTank(10, 10, Tank::ArmorTank);
        testTank2->setFacing(West);
        testTank2->setMoving(true);
        auto testTank3 = testTankController.spawnTank(15, 15, Tank::FastTank);
        testTank3->setMoving(false);
        auto playerTank = testTankController.spawnTank(20, 20, Tank::PlayerTank);
        playerTank->setFacing(South);
        playerTank->setMoving(true);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Calling the .moveAllTanks() method") {
            testTankController.moveAllEntities();

            THEN("Tanks with moving flag set should be moved") {
                REQUIRE(testTank1->getX() == 5);
                REQUIRE(testTank1->getY() == 5 - testTank1->getSpeed());
                REQUIRE(testTank2->getX() == 10 - testTank2->getSpeed());
                REQUIRE(testTank2->getY() == 10);
                REQUIRE(testTank3->getX() == 15);
                REQUIRE(testTank3->getY() == 15);
                REQUIRE(playerTank->getX() == 20);
                REQUIRE(playerTank->getY() == 20 + playerTank->getSpeed());

                AND_THEN("Proper events should be added to the queue"){
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == testTank1);
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == testTank2);
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::EntityMoved);
                    REQUIRE(event->info.entityInfo.entity == playerTank);
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }
    }
}

SCENARIO("Finding tanks by position") {
    GIVEN("A controller with some tanks") {
        TestTankController test_entityController{};
        test_entityController.spawnTank(5, 5, Tank::ArmorTank);
        test_entityController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Trying to find a tank at it's exact location") {
            std::optional<Entity *> found = test_entityController.getEntityAtPosition(20, 20);

            THEN("A correct tank should be returned") {
                REQUIRE(found.has_value());
                REQUIRE(dynamic_cast<Tank*>(found.value())->getType() == Tank::PowerTank);
            }
        }

        WHEN("Trying to find a tank in it's bounding box") {
            std::optional<Entity *> found = test_entityController.getEntityAtPosition(21, 21);

            THEN("A correct tank should be returned") {
                REQUIRE(found.has_value());
                REQUIRE(dynamic_cast<Tank*>(found.value())->getType() == Tank::PowerTank);
            }
        }

        WHEN("Trying to find a tank at the lower right corner of it's bounding box") {
            std::optional<Entity *> found = test_entityController.getEntityAtPosition(21.999, 21.999);

            THEN("A correct tank should be returned") {
                REQUIRE(found.has_value());
                REQUIRE(dynamic_cast<Tank*>(found.value())->getType() == Tank::PowerTank);
            }
        }

        WHEN("Trying to find a tank in a place where there's no tank") {
            std::optional<Entity *> found = test_entityController.getEntityAtPosition(90, 90);

            THEN("No tank should be returned") {
                REQUIRE_FALSE(found.has_value());
            }
        }

        WHEN("Trying to find a tank next to it's edge") {
            std::optional<Entity *> found = test_entityController.getEntityAtPosition(22, 21);

            THEN("No tank should be returned") {
                REQUIRE_FALSE(found.has_value());
            }

            found = test_entityController.getEntityAtPosition(19.999, 19.999);

            THEN("No tank should be returned") {
                REQUIRE_FALSE(found.has_value());
            }
        }

    }
}

SCENARIO("Collision detection"){
    GIVEN("An entity controller with some entities"){
        TestTankController test_entityController{};
        auto tank1 = test_entityController.spawnTank(5, 5, Tank::ArmorTank);
        auto bullet1 = test_entityController.addEntity(std::move(std::make_unique<Bullet>(10, 10, North, 1, Bullet::Friendly)));

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Two objects are away from each other"){
            THEN("No collision should be detected"){
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(tank1));
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(bullet1));
            }
        }
        WHEN("Two objects overlap"){
            auto tank2 = test_entityController.spawnTank(6, 6, Tank::PlayerTank);

            THEN("A collision should be detected"){
                REQUIRE(test_entityController.checkEntityCollisions(tank2));
                REQUIRE(test_entityController.checkEntityCollisions(tank1));
            }
        }
        WHEN("One objects sits right on other's edge (horizontally)"){
            auto tank2 = test_entityController.spawnTank(7, 5, Tank::PlayerTank);

            THEN("Np collision should be detected"){
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(tank1));
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(tank2));
            }
        }
        WHEN("One objects sits right on other's edge (vertically)"){
            auto tank2 = test_entityController.spawnTank(5, 7, Tank::PlayerTank);

            THEN("Np collision should be detected"){
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(tank1));
                REQUIRE_FALSE(test_entityController.checkEntityCollisions(tank2));
            }
        }
    eventQueue->clear();
    }
}