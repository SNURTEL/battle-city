//
// Created by Rafal on 01.06.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/GameState.h"
#include "../include/Game.h"
#include "../include/PointSystem.h"
#include "../include/ActiveEventHandler.h"
#include "../include/FinishedEventHandler.h"
#include "../include/MenuEventHandler.h"
#include "../include/PauseEventHandler.h"
#include "../../core-lib/include/Event.h"

//TODO test the constructor lmao
namespace helper {
    EventQueue<Event> *getEmptyEventQueue() {
        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();
        return eventQueue;
    }

}

SCENARIO("testing point system") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    PointSystem p(0);
    WHEN("Adding points into system") {
        p.add_points(1);
        THEN("Event should be created") {
            REQUIRE(eq->isEmpty() == false);
        }
        THEN("points should be equal to 1") {
            REQUIRE(p.get_points() == 1);
        }
    }
    eq = helper::getEmptyEventQueue();
    WHEN("Setting points into system") {
        p.set_points(1);
        THEN("Event should be created") {
            REQUIRE(eq->isEmpty() == false);
        }
        THEN("Event should be created") {
            REQUIRE(p.get_points() == 1);
        }
    }
}
