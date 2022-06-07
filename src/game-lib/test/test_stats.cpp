//
// Created by tomek on 07.06.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/GameStatistics.h"
#include "../../core-lib/include/EventQueue.h"


namespace {
    namespace helper {
        EventQueue<Event> *getEmptyEventQueue() {
            EventQueue<Event>::instance()->clear();
            return EventQueue<Event>::instance();
        }
    }
}

SCENARIO("Player was killed"){
    GIVEN("A gamestats object with one life left"){
        GameStatistics stats(100, 1, 1);

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Number of lives decreases to 0"){
            stats.decrementLives(1);

            THEN("StatsChanged and GameEnded events should be generated"){
                REQUIRE_FALSE(eventQueue->isEmpty());
                auto event = eventQueue->pop();

                REQUIRE(event->type == Event::StatisticsChanged);
                REQUIRE(event->info.pointsInfo.stats_ == &stats);

                REQUIRE_FALSE(eventQueue->isEmpty());
                event = eventQueue->pop();

                REQUIRE(event->type == Event::GameEnded);
                REQUIRE(event->info.pointsInfo.stats_ == &stats);

                REQUIRE(eventQueue->isEmpty());
            }
        }

        WHEN("Overflow"){

        }
    }
}