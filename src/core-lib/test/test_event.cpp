//
// Created by tomek on 29.04.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/Event.h"

//TODO test the constructor lmao

SCENARIO("Initializing Event object"){
    WHEN("Creating a valid keyboard event"){
        Event test_event(Event::KeyPressed, 123);

        THEN("Event should be created"){
            REQUIRE(test_event.type == Event::KeyPressed);
            REQUIRE(test_event.info.keyInfo.keyCode == 123);
            REQUIRE(test_event.info.keyInfo.action == Event::KeyEventInfo::Pressed);
        }
    }

    WHEN("Creating an invalid event"){
        THEN("An exception should be thrown"){
            REQUIRE_THROWS_AS(Event(Event::KeyReleased), EventConstructionException);
        }
    }

//    WHEN("Creating a valid tank event") {   // impossible to test without including Tank from tank-lib
//        class Tank{};
//        Tank test_tank{};
//        Tank* tankPtr = &test_tank;
//        Event test_event(Event::TankSpawned, tan)
//
//    }
}