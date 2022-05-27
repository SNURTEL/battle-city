//
// Created by tomek on 03.05.2022.
//
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include <memory>

#include "../include/Tank.h"

class TestTank : public Tank {
public:
    TestTank() : Tank() {
        x_ = 10;
        y_ = 10;
        facing_ = North;

        lives_ = 1;
        tank_speed_ = 1;
        bullet_speed_ = 1;
        points_ = 100;

        moving_ = false;
    };
};

SCENARIO("Moving the tank in the direction it is faced") {
    GIVEN("A test tank") {
        auto testTank = std::make_unique<TestTank>();  // facing north

        WHEN("Calling the 'move' method when moving flag is set") {
            testTank->setMoving(true);

            REQUIRE(testTank->getX() == 10);
            REQUIRE(testTank->getX() == 10);

            testTank->move();

            THEN("Tak should move by 1x its speed in a proper direction") {
                REQUIRE(testTank->getX() == 10);
                REQUIRE(testTank->getY() == 10-testTank->getTankSpeed());

                AND_WHEN("Moving more than once") {
                    testTank->move();
                    testTank->move();

                    THEN("Tank's location should be updated accordingly") {
                        REQUIRE(testTank->getX() == 10);
                        REQUIRE(testTank->getY() == 10 - 3*testTank->getTankSpeed());
                    }
                }
            }
        }WHEN("Rotating a tank and moving it") {
            testTank->setFacing(East);
            testTank->setMoving(true);
            testTank->move();

            THEN("The tank should move in a right direction") {
                REQUIRE(testTank->getX() == 11);
                REQUIRE(testTank->getY() == 10);
            }
        }WHEN("Manually specifying the offset") {
            testTank->offsetInCurrentDirection(0.7f);

            THEN("Tank should be offset by the given value") {
                REQUIRE(testTank->getX() == 10);
                REQUIRE(testTank->getY() == 9.3f);
            }
        }WHEN("Moving flag is not set") {
            testTank->setMoving(false);

            testTank->move();
            THEN("Tank's location should not be updated") {
                REQUIRE(testTank->getX() == 10);
                REQUIRE(testTank->getY() == 10);
            }
        }
    }
}