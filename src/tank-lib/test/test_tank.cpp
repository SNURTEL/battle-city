//
// Created by tomek on 03.05.2022.
//
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include <memory>

#include "../include/Tank.h"
#include "../include/Bullet.h"

class TestTank : public Tank {
public:
    TestTank() : Tank(Tank::PowerTank, 10, 10, 1, 1, 1, North, 100) {};
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
                REQUIRE(testTank->getY() == 10 - testTank->getSpeed());

                AND_WHEN("Moving more than once") {
                    testTank->move();
                    testTank->move();

                    THEN("Tank's location should be updated accordingly") {
                        REQUIRE(testTank->getX() == 10);
                        REQUIRE(testTank->getY() == 10 - 3 * testTank->getSpeed());
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

SCENARIO("Firing a bullet") {
    GIVEN("Some tanks") {
        std::unique_ptr<Tank> powerTank = std::make_unique<PowerTank>(10, 20, North);
        std::unique_ptr<Tank> basicTank = std::make_unique<BasicTank>(40, 50, East);
        std::unique_ptr<Tank> playerTank = std::make_unique<PlayerTank>(30, 40, 1, South);
        std::unique_ptr<Tank> armorTank = std::make_unique<ArmorTank>(100, 200, West);

        WHEN("Creating a bullet object") {
            auto powerBullet = powerTank->createBullet();
            auto basicBullet = basicTank->createBullet();
            auto playerBullet = playerTank->createBullet();
            auto armorBullet = armorTank->createBullet();

            THEN("It's position, rotation, and friendliness should be set according to tank's type") {
                Tank *t = powerTank.get();
                Bullet *b = powerBullet->get();
                REQUIRE(b->getX() == t->getX() + (t->getSizeX() - b->getSizeX()) / 2);
                REQUIRE(b->getY() == t->getY() - b->getSizeY());
                REQUIRE(b->getFacing() == North);
                REQUIRE(b->getSpeed() == 3);
                REQUIRE(b->isFriendly() == false);
                t = basicTank.get();
                b = basicBullet->get();
                REQUIRE(b->getX() == t->getX() + t->getSizeX());
                REQUIRE(b->getY() == t->getY() + (t->getSizeY() - b->getSizeY()) / 2);
                REQUIRE(b->getFacing() == East);
                REQUIRE(b->getSpeed() == 1);
                REQUIRE(b->isFriendly() == false);
                t = playerTank.get();
                b = playerBullet->get();
                REQUIRE(b->getX() == t->getX() + (t->getSizeX() - b->getSizeX()) / 2);
                REQUIRE(b->getY() == t->getY() + t->getSizeY());
                REQUIRE(b->getFacing() == South);
                REQUIRE(b->getSpeed() == 1);
                REQUIRE(b->isFriendly() == true);
                t = armorTank.get();
                b = armorBullet->get();
                REQUIRE(b->getX() == t->getX() - b->getSizeX());
                REQUIRE(b->getY() == t->getY() + (t->getSizeY() - b->getSizeY()) / 2);
                REQUIRE(b->getFacing() == West);
                REQUIRE(b->getSpeed() == 2);
                REQUIRE(b->isFriendly() == false);
            }
        }

        WHEN("Trying to fire when a bullet is already on screen") {
            auto bullet = powerTank->createBullet();

            THEN("Bullet should not be created") {
                REQUIRE_FALSE(powerTank->createBullet().has_value());

                AND_WHEN("The other bullet is deleted") {
                    bullet.reset();

                    THEN("New bullet can be created") {
                        auto otherBullet = powerTank->createBullet();

                        REQUIRE(otherBullet.has_value());
                    }
                }
            }
        }
    }
}