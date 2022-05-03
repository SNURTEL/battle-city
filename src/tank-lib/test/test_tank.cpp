//
// Created by tomek on 03.05.2022.
//
#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/Tank.h"

class TestTank: public Tank{
public:
    TestTank(): Tank(){
        x_ = 10;
        y_ = 10;
        facing_=North;

        lives_=1;
        tank_speed_=1;
        bullet_speed_=1;
        points_=100;
    };
};

SCENARIO("Changing move direction"){
    GIVEN("A test tank"){
        TestTank tank{};

        WHEN("Moving at positive x and y=0"){
            tank.moveX(2);
            THEN("Direction should change to east"){
                REQUIRE(tank.getFacing() == East);
            }
        }
        WHEN("Moving at negative x and y=0"){
            tank.moveX(-2);
            THEN("Direction should change to west"){
                REQUIRE(tank.getFacing() == West);
            }
        }
        WHEN("Moving at x=0 and positive y"){
            tank.moveY(2);
            THEN("Direction should change to south"){
                REQUIRE(tank.getFacing() == South);
            }
        }
        WHEN("Moving at x=0 and negative y"){
            tank.moveY(-2);
            THEN("Direction should change to south"){
                REQUIRE(tank.getFacing() == North);
            }
        }
        WHEN("Moving tank by 0 in X direction"){
            tank.moveX(0);
            THEN("Direction should not change"){
                REQUIRE(tank.getFacing() == North);
            }
        }
        WHEN("Moving tank by 0 in Y direction"){
            tank.moveY(0);
            THEN("Direction should not change"){
                REQUIRE(tank.getFacing() == North);
            }
        }
    }
}