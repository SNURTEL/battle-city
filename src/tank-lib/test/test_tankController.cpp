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
            test_tankController.spawnTank(5, 5, Tank::BasicTank);
            test_tankController.spawnTank(20, 20, Tank::PowerTank);
            //TODO test with overlap

            THEN("Tanks should be created") {
                REQUIRE((*(test_tankController.getTanks()))[0]->getType() == Tank::BasicTank);
                REQUIRE((*(test_tankController.getTanks()))[1]->getType() == Tank::PowerTank);

                AND_THEN("Tank creation events should be added to the pool") {
                    REQUIRE(eventQueue->pop()->type == Event::TankSpawned);
                    REQUIRE(eventQueue->pop()->type == Event::TankSpawned);
                    REQUIRE(eventQueue->isEmpty());
                }
            }
        }

    }
}

SCENARIO("Tank killed"){
    GIVEN("A controller with some tanks"){
        TestTankController test_tankController{};
        test_tankController.spawnTank(5, 5, Tank::ArmorTank);
        test_tankController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->pop();
        eventQueue->pop();

        WHEN("Controller is requested to kill a tank"){
            Tank *target = (*(test_tankController.getTanks()))[0].get();
            test_tankController.killTank(target);

            THEN("Tank should be removed from the pool"){
                REQUIRE(test_tankController.getTanks()->size() == 1);

                AND_THEN("TankKilled event should be added to the queue"){
                    REQUIRE(eventQueue->pop()->type == Event::TankKilled);
                }
            }
        }
    }
}

SCENARIO("Tank removed"){
    GIVEN("A controller with some tanks"){
        TestTankController test_tankController{};
        test_tankController.spawnTank(5, 5, Tank::ArmorTank);
        test_tankController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->pop();
        eventQueue->pop();

        WHEN("Controller is requested to remove a tank"){
            Tank *target = (*(test_tankController.getTanks()))[0].get();
            test_tankController.killTank(target);

            THEN("Tank should be removed from the pool"){
                REQUIRE(test_tankController.getTanks()->size() == 1);

                AND_THEN("TankRemoved event should be added to the queue"){
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
        eventQueue->pop();
        eventQueue->pop();

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

            THEN("Tank should be destroyed"){
                REQUIRE(test_tankController.getTanks()->size() == 1);

                AND_THEN("An event should be added to the queue"){
                    REQUIRE(eventQueue->pop()->type == Event::TankKilled);
                }
            }
        }
    }
}

SCENARIO("Moving tanks"){
    GIVEN("A tank controller with some tanks"){
        TestTankController test_tankController{};
        test_tankController.spawnTank(5, 5, Tank::ArmorTank);
        test_tankController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->pop();
        eventQueue->pop();

        WHEN("Controller is requested to move a tank"){
            Tank *target = (*(test_tankController.getTanks()))[0].get();
            test_tankController.moveTank(target, South);

            THEN("Tanks location an orientation should be updated"){
                REQUIRE(target->getY()==6);
                REQUIRE(target->getFacing() == South);

                AND_THEN("An event should be added to the pool"){
                    REQUIRE(eventQueue->pop()->type == Event::TankMoved);
                }
            }
        }
    }
}

SCENARIO("Finding tanks by position"){
    GIVEN("A controller with some tanks"){
        TestTankController test_tankController{};
        test_tankController.spawnTank(5, 5, Tank::ArmorTank);
        test_tankController.spawnTank(20, 20, Tank::PowerTank);

        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->pop();
        eventQueue->pop();

        WHEN("Trying to find a tank at it's exact location"){
            std::optional<Tank*> found = test_tankController.getTankAtPosition(20, 20);

            THEN("A correct tank should be returned"){
                REQUIRE(found.has_value());
                REQUIRE(found.value()->getType() == Tank::PowerTank);
            }
        }

        WHEN("Trying to find a tank in it's bounding box"){
            std::optional<Tank*> found = test_tankController.getTankAtPosition(27, 27);

            THEN("A correct tank should be returned"){
                REQUIRE(found.has_value());
                REQUIRE(found.value()->getType() == Tank::PowerTank);
            }
        }

        WHEN("Trying to find a tank at the lower right corner of it's bounding box"){
            std::optional<Tank*> found = test_tankController.getTankAtPosition(32, 32);

            THEN("A correct tank should be returned"){
                REQUIRE(found.has_value());
                REQUIRE(found.value()->getType() == Tank::PowerTank);
            }
        }

        WHEN("Trying to find a tank in a place where there's no tank"){
            std::optional<Tank*> found = test_tankController.getTankAtPosition(90, 90);

            THEN("A correct tank should be returned"){
                REQUIRE_FALSE(found.has_value());
            }
        }

        WHEN("Trying to find a tank next to it's edge"){
            std::optional<Tank*> found = test_tankController.getTankAtPosition(33, 33);

            THEN("A correct tank should be returned"){
                REQUIRE_FALSE(found.has_value());
            }

            found = test_tankController.getTankAtPosition(19, 19);

            THEN("A correct tank should be returned"){
                REQUIRE_FALSE(found.has_value());
            }
        }

    }
}