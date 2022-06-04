//
// Created by Rafal on 01.06.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include <bits/stdc++.h>

#include "../include/GameState.h"
#include "../include/Game.h"
#include "../include/GameStatistics.h"
#include "../include/ActiveEventHandler.h"
#include "../include/FinishedEventHandler.h"
#include "../include/MenuEventHandler.h"
#include "../include/PauseEventHandler.h"
#include "../../core-lib/include/Event.h"

//TODO test the constructor lmao
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

SCENARIO("testing point system") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    GameStatistics p(0, 0, 0);
    WHEN("Adding points into system") {
        p.addPoints(1);
        THEN("Event should be created") {
            REQUIRE(eq->isEmpty() == false);
        }
        THEN("points should be equal to 1") {
            REQUIRE(p.getPoints() == 1);
        }
    }
    eq = helper::getEmptyEventQueue();
    WHEN("Setting points into system") {
        p.setPoints(1);
        THEN("Event should be created") {
            REQUIRE(eq->isEmpty() == false);
        }
        THEN("Event should be created") {
            REQUIRE(p.getPoints() == 1);
        }
    }
}

SCENARIO("Player Movement") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    Game* game = std::make_unique<Game>(60).get();
    ActiveGameState* state = std::make_unique<ActiveGameState>(game).get();
    Tank* player_tank = std::make_unique<PlayerTank>(3, 3).get();
    Board* board = std::make_unique<Board>().get();
    state->set_board(board);
    state->set_player_tank(player_tank);
    ActiveEventHandler* handler = std::make_unique<ActiveEventHandler>(game, state).get();
    WHEN("UP arrow clicked") {
        eq->registerEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 73));
        // std::unique_ptr<Event> event = eq->pop();
        // handler->handleEvent(event);
        THEN("Player tank should be moving") {
            REQUIRE(player_tank->isMoving() == true);
        }
        THEN("Direction should be north") {
            REQUIRE(player_tank->getFacing() == North);
        }
        REQUIRE(1 == 1);
    }  
    WHEN("UP arrow released") {
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyReleased, 73));
        THEN("Should stop moving") {
            REQUIRE(player_tank->isMoving() == false);
        }
    }
    /////////////////////////////
    WHEN("DOWN arrow clicked") {
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 74));
        THEN("Player tank should be moving") {
            REQUIRE(player_tank->isMoving() == true);
        }
        THEN("Direction should be south") {
            REQUIRE(player_tank->getFacing() == South);
        }
    }  
    WHEN("DOWN arrow released") {
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyReleased, 74));
        THEN("Should stop moving") {
            REQUIRE(player_tank->isMoving() == false);
        }
    } 
    /////////////////////////////
    WHEN("RIGHT arrow clicked") {
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 71));
        THEN("Player tank should be moving") {
            REQUIRE(player_tank->isMoving() == true);
        }
        THEN("Direction should be EAST") {
            REQUIRE(player_tank->getFacing() == East);
        }
    }  
    WHEN("RIGHT arrow released") {
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyReleased, 71));
        THEN("Should stop moving") {
            REQUIRE(player_tank->isMoving() == false);
        }
    } 
    /////////////////////////////
    WHEN("LEFT arrow clicked") {
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 72));
        THEN("Player tank should be moving") {
            REQUIRE(player_tank->isMoving() == true);
        }
        THEN("Direction should be WEST") {
            REQUIRE(player_tank->getFacing() == West);
        }
    }  
    WHEN("LEFT arrow released") {
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyReleased, 72));
        THEN("Should stop moving") {
            REQUIRE(player_tank->isMoving() == false);
        }
    }
}

SCENARIO("Player Shooting") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    Game* game = std::make_unique<Game>(60).get();
    ActiveGameState* state = std::make_unique<ActiveGameState>(game).get();
    Tank* player_tank = std::make_unique<PlayerTank>(3, 3).get();
    Board* board = std::make_unique<Board>().get();
    state->set_board(board);
    state->set_player_tank(player_tank);
    std::unique_ptr<ActiveEventHandler> handler = std::make_unique<ActiveEventHandler>(game, state);
    WHEN("SPACE clicked") {
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 72));
        THEN("Player should create a bullet (cant create another)") {
           //  REQUIRE(player_tank->createBullet() == std::nullopt);
        }
    }  
}

SCENARIO("Active to Pause") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    Game* game = std::make_unique<Game>(60).get();
    ActiveGameState* state = std::make_unique<ActiveGameState>(game).get();
    Tank* player_tank = std::make_unique<PlayerTank>(3, 3).get();
    Board* board = std::make_unique<Board>().get();
    state->set_board(board);
    state->set_player_tank(player_tank);
    std::unique_ptr<ActiveEventHandler> handler = std::make_unique<ActiveEventHandler>(game, state);
    WHEN("ESC clicked") {
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 72));
        THEN("state should be pause") {
            REQUIRE(dynamic_cast<PauseGameState*>(game->getState()) != nullptr);
        }
    }  
}

SCENARIO("Player tank is killed") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    Game* game = std::make_unique<Game>(60).get();
    ActiveGameState* state = std::make_unique<ActiveGameState>(game).get();
    Tank* player_tank = std::make_unique<PlayerTank>(3, 3).get();
    Board* board = std::make_unique<Board>().get();
    state->set_board(board);
    state->set_player_tank(player_tank);
    std::unique_ptr<ActiveEventHandler> handler = std::make_unique<ActiveEventHandler>(game, state);

    WHEN ("Tank Killed event") {
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::TankKilled, player_tank);
        THEN ("State is finished") {
            REQUIRE(dynamic_cast<FinishedGameState*>(game->getState()) != nullptr);
        }
    }
}

SCENARIO("Collisions") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    Game* game = std::make_unique<Game>(60).get();
    ActiveGameState* state = std::make_unique<ActiveGameState>(game).get();
    Tank* player_tank = std::make_unique<PlayerTank>(3, 3).get();
    Tank* other_tank = std::make_unique<FastTank>(3, 3).get();
    Board* board = std::make_unique<Board>().get();
    state->set_board(board);
    state->set_player_tank(player_tank);
    std::unique_ptr<ActiveEventHandler> handler = std::make_unique<ActiveEventHandler>(game, state);
    
    WHEN("Tank collides with tank") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::EntityEntityCollision, player_tank, other_tank));
        THEN("They should move back") {
            //
        }
    }

    WHEN("Enemy bullet collides with player tank") {

        THEN("Player lives -1") {

        }
    }

    WHEN("Player bullet collides with enemy tank") {
        
        THEN("Player points +") {

        }

        THEN("Enemy tank gets deleted") {

        }
    }

    WHEN("Enemy bullet collides with a friendly bullet") {

        THEN("Both bullets get deleted") {

        }
    }

    WHEN("Bullet hits breakable tile") {

        THEN("Tile and bullet are deleted") {

        }
    }

    WHEN("Bullet hits unbreakable tile") {

        THEN("Only bullet gets deleted") {
            
        }
    }

    WHEN("Tank hits a tile") {

        THEN("Tank should be moved back") {

        }
    }

}



SCENARIO("State is menu") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    Game* game = std::make_unique<Game>(60).get();
    MenuGameState* state = std::make_unique<MenuGameState>(game).get();
    std::unique_ptr<MenuEventHandler> handler = std::make_unique<MenuEventHandler>(game, state);

    WHEN("UP arrow is clicked") {
        eq->registerEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 73));
        THEN("Menu pos should change (+1)") {

        }
    }

    WHEN("UP arrow is clicked") {
        eq->registerEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 74));
        THEN("Menu pos should change (-1)") {

        }
    }

    WHEN("ENTER is clicked") {

        GIVEN("menu pos = 1") {

            THEN("State should be active") {

            }
        }

        GIVEN("menu pos = 2")
        {
            
            THEN("Game should end") {

            }
        }
    }
}


SCENARIO("State is pause") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    Game* game = std::make_unique<Game>(60).get();
    PauseGameState* state = std::make_unique<PauseGameState>(game).get();
    std::unique_ptr<PauseEventHandler> handler = std::make_unique<PauseEventHandler>(game, state);

    WHEN("UP arrow is clicked") {
        eq->registerEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 73));
        THEN("Menu pos should change (+1)") {

        }
    }

    WHEN("UP arrow is clicked") {
        eq->registerEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 74));
        THEN("Menu pos should change (-1)") {

        }
    }

    WHEN("ENTER is clicked") {

        GIVEN("menu pos = 1") {

            THEN("State should be active") {

            }
        }

        GIVEN("menu pos = 2")
        {
            
            THEN("State should be menu") {
                
            }
        }
    }
}


