//
// Created by tomek on 03.05.2022.
//

#include <memory>
#include <vector>

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

#include "../include/TankController.h"
#include "../include/Tank.h"


class TestTankController : public TankController {
public:
    TestTankController() : TankController() {
    }

    std::vector<std::unique_ptr<Tank>> *getTanks() {
        return &tanks_;
    }
};

SCENARIO("Spawning tanks") {
    GIVEN("An empty tank controller") {
        TestTankController test_tankController{};
        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();

        WHEN("Requesting to spawn a tank") {
            auto newTank1 = test_tankController.spawnTank(5, 5, Tank::BasicTank);
            auto newTank2 = test_tankController.spawnTank(20, 20, Tank::PowerTank);

            THEN("Tanks should be created") {
                REQUIRE((*(test_tankController.getTanks()))[0]->getType() == Tank::BasicTank);
                REQUIRE((*(test_tankController.getTanks()))[1]->getType() == Tank::PowerTank);

                AND_THEN("Tank creation events should be added to the pool") {
                    auto event1 = eventQueue->pop();
                    auto event2 = eventQueue->pop();

                    REQUIRE(event1->type == Event::TankSpawned);
                    REQUIRE(event1->info.tankInfo.tank == newTank1);
                    REQUIRE(event2->type == Event::TankSpawned);
                    REQUIRE(event2->info.tankInfo.tank == newTank2);
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }

        WHEN("Spawning the player") {
            auto newTank = test_tankController.spawnTank(5, 5, Tank::PlayerTank);

            THEN("New tank should be stored in a separate place") {
                REQUIRE(test_tankController.getPlayer() == newTank);
                REQUIRE(test_tankController.getTanks()->empty());

                AND_THEN("A correct event should be added to the pool") {
                    auto event = eventQueue->pop();
                    REQUIRE(eventQueue->isEmpty());

                    REQUIRE(event->type == Event::TankSpawned);
                    REQUIRE(event->info.tankInfo.tank == newTank);
                }
            }
        }

    }
}

SCENARIO("Tank killed") {
    GIVEN("A controller with some tanks") {
        TestTankController test_tankController{};
        test_tankController.spawnTank(5, 5, Tank::ArmorTank);
        test_tankController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->pop();
        eventQueue->pop();

        WHEN("Controller is requested to kill a tank") {
            Tank *target = (*(test_tankController.getTanks()))[0].get();
            test_tankController.killTank(target);

            THEN("Tank should be removed from the pool") {
                REQUIRE(test_tankController.getTanks()->size() == 1);

                AND_THEN("TankKilled event should be added to the queue") {
                    REQUIRE(eventQueue->pop()->type == Event::TankKilled);
                }
            }
        }
    }
}

SCENARIO("Tank removed") {
    GIVEN("A controller with some tanks") {
        TestTankController test_tankController{};
        test_tankController.spawnTank(5, 5, Tank::ArmorTank);
        test_tankController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Controller is requested to remove a tank") {
            Tank *target = (*(test_tankController.getTanks()))[0].get();
            test_tankController.killTank(target);

            THEN("Tank should be removed from the pool") {
                REQUIRE(test_tankController.getTanks()->size() == 1);

                AND_THEN("TankRemoved event should be added to the queue") {
                    REQUIRE(eventQueue->pop()->type == Event::TankKilled);
                }
            }
        }
    }
}

SCENARIO("Tank hit") {
    GIVEN("A controller with some tanks") {
        TestTankController test_tankController{};
        test_tankController.spawnTank(5, 5, Tank::ArmorTank);
        test_tankController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Tank receives fewer damage points than it has health") {
            Tank *target = (*(test_tankController.getTanks()))[0].get();
            test_tankController.hitTank(target);

            THEN("Tank's health should be decreased") {
                REQUIRE(target->getLives() == 3);
                AND_THEN("An event should be added to the queue") {
                    REQUIRE(eventQueue->pop()->type == Event::TankHit);
                }
            }
        }

        WHEN("Damage is greater or equal than tank's health") {
            Tank *target = (*(test_tankController.getTanks()))[1].get();
            test_tankController.hitTank(target);

            THEN("Tank should be destroyed") {
                REQUIRE(test_tankController.getTanks()->size() == 1);

                AND_THEN("An event should be added to the queue") {
                    REQUIRE(eventQueue->pop()->type == Event::TankKilled);
                }
            }
        }
    }
}

SCENARIO("Moving a tank") {
    GIVEN("A tank controller with some tanks") {
        TestTankController test_tankController{};
        auto testTank1 = test_tankController.spawnTank(5, 5, Tank::ArmorTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Rotating tanks"){
            test_tankController.setTankDirection(testTank1, East);

            THEN("The direction in which the tank is faced should be updated"){
                REQUIRE(testTank1->getFacing()==East);

                AND_THEN("An event should be added to the queue"){
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankRotated);
                    REQUIRE(event->info.tankInfo.tank == testTank1);

                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }

        WHEN("Controller is requested to move a tank") {
            test_tankController.setTankMoving(testTank1, true);
            test_tankController.setTankDirection(testTank1, South);
            eventQueue->pop();
            test_tankController.moveTank(testTank1);

            THEN("Tanks location an orientation should be updated") {
                REQUIRE(testTank1->getY() == 5 + testTank1->getTankSpeed());
                REQUIRE(testTank1->getFacing() == South);

                AND_THEN("An event should be added to the pool") {
                    std::unique_ptr<Event> event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankMoved);
                    REQUIRE(event->info.tankInfo.tank == testTank1);
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
        auto testTank3 = testTankController.spawnTank(15, 15, Tank::ArmorTank);
        testTank3->setMoving(false);
        auto playerTank = testTankController.spawnTank(20, 20, Tank::PlayerTank);
        playerTank->setFacing(South);
        playerTank->setMoving(true);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Calling the .moveAllTanks() method") {
            testTankController.moveAllTanks();

            THEN("Tanks with moving flag set should be moved") {
                REQUIRE(testTank1->getX() == 5);
                REQUIRE(testTank1->getY() == 5 - testTank1->getTankSpeed());
                REQUIRE(testTank2->getX() == 10 - testTank2->getTankSpeed());
                REQUIRE(testTank2->getY() == 10);
                REQUIRE(testTank3->getX() == 15);
                REQUIRE(testTank3->getY() == 15);
                REQUIRE(playerTank->getX() == 20);
                REQUIRE(playerTank->getY() == 20 + playerTank->getTankSpeed());

                AND_THEN("Proper events should be added to the queue"){
                    auto event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankMoved);
                    REQUIRE(event->info.tankInfo.tank == testTank1);
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankMoved);
                    REQUIRE(event->info.tankInfo.tank == testTank2);
                    event = eventQueue->pop();
                    REQUIRE(event->type == Event::TankMoved);
                    REQUIRE(event->info.tankInfo.tank == playerTank);
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }
    }
}

SCENARIO("Finding tanks by position") {
    GIVEN("A controller with some tanks") {
        TestTankController test_tankController{};
        test_tankController.spawnTank(5, 5, Tank::ArmorTank);
        test_tankController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();

        WHEN("Trying to find a tank at it's exact location") {
            std::optional<Tank *> found = test_tankController.getTankAtPosition(20, 20);

            THEN("A correct tank should be returned") {
                REQUIRE(found.has_value());
                REQUIRE(found.value()->getType() == Tank::PowerTank);
            }
        }

        WHEN("Trying to find a tank in it's bounding box") {
            std::optional<Tank *> found = test_tankController.getTankAtPosition(21, 21);

            THEN("A correct tank should be returned") {
                REQUIRE(found.has_value());
                REQUIRE(found.value()->getType() == Tank::PowerTank);
            }
        }

        WHEN("Trying to find a tank at the lower right corner of it's bounding box") {
            std::optional<Tank *> found = test_tankController.getTankAtPosition(21.999, 21.999);

            THEN("A correct tank should be returned") {
                REQUIRE(found.has_value());
                REQUIRE(found.value()->getType() == Tank::PowerTank);
            }
        }

        WHEN("Trying to find a tank in a place where there's no tank") {
            std::optional<Tank *> found = test_tankController.getTankAtPosition(90, 90);

            THEN("No tank should be returned") {
                REQUIRE_FALSE(found.has_value());
            }
        }

        WHEN("Trying to find a tank next to it's edge") {
            std::optional<Tank *> found = test_tankController.getTankAtPosition(22, 21);

            THEN("No tank should be returned") {
                REQUIRE_FALSE(found.has_value());
            }

            found = test_tankController.getTankAtPosition(19.999, 19.999);

            THEN("No tank should be returned") {
                REQUIRE_FALSE(found.has_value());
            }
        }

    }
}