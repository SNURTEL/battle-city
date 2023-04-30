//
// Created by tomek on 02.06.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/Game.h"

namespace {
    namespace helper {
        class TestGame : public Game {
        public:
            explicit TestGame(unsigned int clockFreq) : Game(clockFreq) {};

            void testSetup() { setup(); }

            bool checkIfStatesArePresent() { return active_state_ && pause_state_ && finished_state_ && menu_state_; }

            bool checkRunning() { return running_; };

            Board *getBoard() { return board_.get(); };

            GameStatsIO *getGameStatsIO() { return gameStatsIO_.get(); };

            void testStart() { start(); };

            void testReset() { reset(); };

            void testEnd() { end(); };

        };

        EventQueue<Event> *getEmptyEventQueue() {
            EventQueue<Event>::instance()->clear();
            return EventQueue<Event>::instance();
        }
    }
}

SCENARIO("Initializing the game") {
    helper::getEmptyEventQueue();
    GIVEN("Program just started running") {
        WHEN("Initializing the main container") {
            helper::TestGame game = helper::TestGame(60);
            REQUIRE_NOTHROW(game.testSetup());

            THEN("All (a lot) of fields should be set correctly") {
                REQUIRE(game.checkRunning());
                REQUIRE(game.checkIfStatesArePresent());
                REQUIRE(dynamic_cast<MenuGameState *>(game.getState()) != nullptr);
                REQUIRE(game.getBoard());
                REQUIRE(game.getStats()->getLives() == 3);
                REQUIRE(game.getStats()->getPoints() == 0);
                REQUIRE(game.getStats()->getLevel() == 1);
                REQUIRE(game.getGameStatsIO());
                auto event = EventQueue<Event>::instance()->pop();
                REQUIRE(event->type == Event::StateChanged);
                REQUIRE(event->info.stateInfo.state_ == game.getState());
                REQUIRE(EventQueue<Event>::instance()->isEmpty());
            }
        }
    }
}

SCENARIO("Starting the game") {
    GIVEN("Game object was already initialized") {
        helper::TestGame game = helper::TestGame(60);
        game.testSetup();

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Calling the start() method") {
            game.testStart();

            THEN("Stats should reset (won't make any difference), level 1 should be loaded, player's tank should spawn, and game's state should change") {
                REQUIRE_FALSE(eventQueue->isEmpty());
                auto event = eventQueue->pop();
                REQUIRE(event->type == Event::StatisticsChanged);

                REQUIRE_FALSE(eventQueue->isEmpty());
                event = eventQueue->pop();
                REQUIRE(event->type == Event::LevelLoaded);
                REQUIRE(event->info.levelInfo.levelNumber == 1);

                REQUIRE_FALSE(eventQueue->isEmpty());
                event = eventQueue->pop();
                REQUIRE(event->type == Event::PlayerSpawned);

                // will not work without level1.txt access
//                REQUIRE(event->info.entityInfo.entity->getX() == 18);
//                REQUIRE(event->info.entityInfo.entity->getY() == 48);

                REQUIRE_FALSE(eventQueue->isEmpty());
                event = eventQueue->pop();
                REQUIRE(event->type == Event::StateChanged);
                REQUIRE(dynamic_cast<ActiveGameState *>(event->info.stateInfo.state_) == game.getState());
                REQUIRE(game.getState() != nullptr);

                REQUIRE(eventQueue->isEmpty());
            }
        }
    }
}

SCENARIO("Resetting game's state") {
    GIVEN("Game's attrs were changed") {
        helper::TestGame game = helper::TestGame(60);
        game.testSetup();
        game.getStats()->setLevel(123);
        game.getStats()->setPoints(456);
        game.getStats()->setLives(999);
        game.getBoard()->spawnTank(12, 34, Tank::BasicTank);
        game.getBoard()->spawnPlayer(22, 44);

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Resetting game's state") {
            game.testReset();

            THEN("Entities should be deleted from the board and stats should be reset") {
                REQUIRE(game.getStats()->getLevel() == 1);
                REQUIRE(game.getStats()->getPoints() == 0);
                REQUIRE(game.getStats()->getLives() == 3);

                REQUIRE_FALSE(eventQueue->isEmpty());
                auto event = eventQueue->pop();
                REQUIRE(event->type == Event::EntityRemoved);

                // entity pointer in EntityRemoved events may only be used for
                // removing empty references - dynamic_cast will not work even if ptr value is correct!
//                REQUIRE(dynamic_cast<BasicTank *>(event->info.entityInfo.entity) != nullptr);

                REQUIRE_FALSE(eventQueue->isEmpty());
                event = eventQueue->pop();
                REQUIRE(event->type == Event::EntityRemoved);
//                REQUIRE(dynamic_cast<PlayerTank *>(event->info.entityInfo.entity) != nullptr);

                REQUIRE_FALSE(eventQueue->isEmpty());
                event = eventQueue->pop();
                REQUIRE(event->type == Event::StatisticsChanged);

                REQUIRE(eventQueue->isEmpty());
            }
        }
    }
}

SCENARIO("Ending the game") {
    GIVEN("Game's attrs were changed") {
        helper::TestGame game = helper::TestGame(60);
        game.testSetup();
        game.getStats()->setLevel(123);
        game.getStats()->setPoints(456);
        game.getBoard()->spawnTank(12, 34, Tank::BasicTank);

        auto eventQueue = helper::getEmptyEventQueue();

        WHEN("Calling the .end method") {
            game.testEnd();

            THEN("Entities should be removed and game's state should change") {
                REQUIRE_FALSE(eventQueue->isEmpty());
                auto event = eventQueue->pop();
                REQUIRE(event->type == Event::EntityRemoved);

                REQUIRE_FALSE(eventQueue->isEmpty());
                event = eventQueue->pop();
                REQUIRE(event->type == Event::StateChanged);
                REQUIRE(dynamic_cast<FinishedGameState*>(event->info.stateInfo.state_) != nullptr);

                REQUIRE(eventQueue->isEmpty());
            }
        }
    }
}