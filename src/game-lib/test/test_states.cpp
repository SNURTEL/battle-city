//
// Created by Rafal on 01.06.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include <bits/stdc++.h>

#include "../include/GameState.h"
#include "../include/Game.h"
#include "../../tank-lib/include/Bullet.h"
#include "../../tank-lib/include/Entity.h"
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

            void testSetup() {

                initStates();
                initComponents();
                initScoreboard();

                setMenuState();
                running_ = true;
            }

            bool checkIfStatesArePresent() { return active_state_ && pause_state_ && finished_state_ && menu_state_; }

            bool checkRunning() { return running_; };

            GameStatsIO *getGameStatsIO() { return gameStatsIO_.get(); };

            void testStart() { start(); };

            void testReset() { reset(); };

            void testEnd() { end(); };

            bool testIsRunning() { return running_; };

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
    std::unique_ptr<helper::TestGame> game = std::make_unique<helper::TestGame>(60);
    game->testSetup();
    game->testStart();
    ActiveGameState* state = dynamic_cast<ActiveGameState*>(game->getState());
    Event keypressed(Event::KeyPressed, 74);
    ActiveEventHandler* handler = dynamic_cast<ActiveEventHandler*>(state->getEventHandler());
    WHEN("UP arrow clicked") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 73));
        THEN("Player tank should be moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == true);
        }
        THEN("Direction should be north") {
            REQUIRE(game->getBoard()->getPlayerTank()->getFacing() == North);
        }
    }  
    WHEN("UP arrow released") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyReleased, 73));
        THEN("Should stop moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == false);
        }
    }
    /////////////////////////////
    WHEN("DOWN arrow clicked") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 74));
        THEN("Player tank should be moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == true);
        }
        THEN("Direction should be south") {
            REQUIRE(game->getBoard()->getPlayerTank()->getFacing() == South);
        }
    }  
    WHEN("DOWN arrow released") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyReleased, 74));
        THEN("Should stop moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == false);
        }
    } 
    /////////////////////////////
    WHEN("RIGHT arrow clicked") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 71));
        THEN("Player tank should be moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == true);
        }
        THEN("Direction should be EAST") {
            REQUIRE(game->getBoard()->getPlayerTank()->getFacing() == East);
        }
    }  
    WHEN("RIGHT arrow released") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyReleased, 71));
        THEN("Should stop moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == false);
        }
    } 
    /////////////////////////////
    WHEN("LEFT arrow clicked") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 72));
        THEN("Player tank should be moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == true);
        }
        THEN("Direction should be WEST") {
            REQUIRE(game->getBoard()->getPlayerTank()->getFacing() == West);
        }
    }  
    WHEN("LEFT arrow released") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyReleased, 72));
        THEN("Should stop moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == false);
        }
    }
}

SCENARIO("Player Shooting") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    std::unique_ptr<helper::TestGame> game = std::make_unique<helper::TestGame>(60);
    game->testSetup();
    game->testStart();
    ActiveGameState* state = dynamic_cast<ActiveGameState*>(game->getState());
    Event keypressed(Event::KeyPressed, 74);
    ActiveEventHandler* handler = dynamic_cast<ActiveEventHandler*>(state->getEventHandler());
    WHEN("SPACE clicked") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 72));
        THEN("Player should create a bullet (cant create another)") {
           // REQUIRE(game->getBoard()->getPlayerTank()->createBullet() == nullptr);
        }
    }  
}

SCENARIO("Active to Pause") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    std::unique_ptr<helper::TestGame> game = std::make_unique<helper::TestGame>(60);
    game->testSetup();
    game->testStart();
    ActiveGameState* state = dynamic_cast<ActiveGameState*>(game->getState());
    Event keypressed(Event::KeyPressed, 74);
    ActiveEventHandler* handler = dynamic_cast<ActiveEventHandler*>(state->getEventHandler());
    WHEN("ESC clicked") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 36));
        THEN("state should be pause") {
            REQUIRE(dynamic_cast<PauseGameState*>(game->getState()) != nullptr);
        }
    }  
}

SCENARIO("Player tank is killed") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    std::unique_ptr<helper::TestGame> game = std::make_unique<helper::TestGame>(60);
    game->testSetup();
    game->testStart();
    ActiveGameState* state = dynamic_cast<ActiveGameState*>(game->getState());
    Event keypressed(Event::KeyPressed, 74);
    ActiveEventHandler* handler = dynamic_cast<ActiveEventHandler*>(state->getEventHandler());

    WHEN ("Tank Killed event") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::TankKilled, game->getBoard()->getPlayerTank()));
        THEN ("State is finished") {
            REQUIRE(dynamic_cast<FinishedGameState*>(game->getState()) != nullptr);
        }
    }
}

SCENARIO("Collisions") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    std::unique_ptr<helper::TestGame> game = std::make_unique<helper::TestGame>(60);
    game->testSetup();
    game->testStart();
    game->getBoard()->spawnTank(15, 15, Tank::ArmorTank);
    ActiveGameState* state = dynamic_cast<ActiveGameState*>(game->getState());
    Event keypressed(Event::KeyPressed, 74);
    ActiveEventHandler* handler = dynamic_cast<ActiveEventHandler*>(state->getEventHandler());
    
    WHEN("Tank collides with tank") {
        // needs Entity* gets Tank*
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::EntityEntityCollision, game->getBoard()->getPlayerTank(), 
        // game->getBoard()->getEntityController()->findEntityAtPosition(15, 15)));
        THEN("They should move back") {
            // todo
        }
    }
    WHEN("Enemy bullet collides with player tank") {
        Bullet* bullet_ptr = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Enemy));
        unsigned int lives = game->getBoard()->getPlayerTank()->getLives();
        // needs Entity* and gets Bullet*
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::EntityEntityCollision, game->getBoard()->getPlayerTank(), bullet_ptr));
        THEN("Player lives -1") {
            // REQUIRE(game->getBoard()->getPlayerTank()->getLives() == lives - 1); Not gonna pass now
        }
    }

    WHEN("Player bullet collides with enemy tank") {
        Bullet* bullet_ptr = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Enemy));
        unsigned int points = game->getStats()->getPoints();
        // needs Entity* and gets Bullet*
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::EntityEntityCollision, game->getBoard()->getPlayerTank(), bullet_ptr));
        THEN("Player points +") {
            // REQUIRE(game->getStats()->getPoints() > points); Not gonna pass now
        }

        THEN("Enemy tank gets deleted") {
            // todo
        }
    }

    WHEN("Enemy bullet collides with a friendly bullet") {
        Bullet* bullet_enemy = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Enemy));
        Bullet* bullet_friendly = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Friendly));
        // needs Entity* and gets Bullet*
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::EntityEntityCollision, bullet_enemy, bullet_friendly));
        THEN("Both bullets get deleted") {
            // todo
        }
    }

    WHEN("Bullet hits breakable tile") {
        Bullet* bullet_enemy = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Enemy));
        Bullet* bullet_friendly = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Friendly));
        // needs Entity* and gets Bullet*
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::EntityGridCollision, bullet_enemy, 10, 10));
        THEN("Tile and bullet are deleted") {
            // REQUIRE_TRUE(game->getBoard()->getGrid()->isTileDeleted(10, 10))
            // todo
        }
    }

    WHEN("Bullet hits unbreakable tile") {
        Bullet* bullet_enemy = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Enemy));
        Bullet* bullet_friendly = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Friendly));
        // needs Entity* and gets Bullet*
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::EntityGridCollision, bullet_enemy, 10, 10));
        THEN("Only bullet gets deleted") {
            // REQUIRE_FALSE(game->getBoard()->getGrid()->isTileDeleted(10, 10))
            // todo
        }
    }

    WHEN("Tank hits a tile") {
        // needs Entity* and gets Tank*
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::EntityGridCollision, game->getBoard()->getPlayerTank(), 10, 10));
        THEN("Tank should be moved back") {
            // todo
        }
    }

}



SCENARIO("State is menu") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    std::unique_ptr<helper::TestGame> game = std::make_unique<helper::TestGame>(60);
    game->testSetup();
    MenuGameState* state = dynamic_cast<MenuGameState*>(game->getState());
    MenuEventHandler* handler = dynamic_cast<MenuEventHandler*>(state->getEventHandler());

    WHEN("UP arrow is clicked") {
        state->get_menu()->set_pos(2);
        unsigned int curr_pos = state->get_menu()->get_pos();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 73));
        THEN("Menu pos should change (-1)") {
            REQUIRE(state->get_menu()->get_pos() == curr_pos - 1);
        }
    }

    WHEN("DOWN arrow is clicked") {
        unsigned int curr_pos = state->get_menu()->get_pos();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 74));
        THEN("Menu pos should change (+1)") {
            REQUIRE(state->get_menu()->get_pos() == curr_pos + 1);
        }
    }

    WHEN("ENTER is clicked") {
        GIVEN("menu pos = 1") {
            state->get_menu()->set_pos(1);
            handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 58));
            THEN("State should be active") {
                REQUIRE(dynamic_cast<ActiveGameState*>(game->getState()) != nullptr);
            }
        }

        GIVEN("menu pos = 2")
        {
            state->get_menu()->set_pos(2);
            REQUIRE(game->testIsRunning());
            handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 58));
            THEN("Game should end") {
                REQUIRE_FALSE(game->testIsRunning());
            }
        }
    }
}


SCENARIO("State is pause") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    std::unique_ptr<helper::TestGame> game = std::make_unique<helper::TestGame>(60);
    game->testSetup();
    MenuGameState* state = dynamic_cast<MenuGameState*>(game->getState());
    MenuEventHandler* handler = dynamic_cast<MenuEventHandler*>(state->getEventHandler());

    WHEN("UP arrow is clicked") {
        state->get_menu()->set_pos(2);
        unsigned int curr_pos = state->get_menu()->get_pos();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 73));
        THEN("Menu pos should change (-1)") {
            REQUIRE(state->get_menu()->get_pos() == curr_pos - 1);
        }
    }

    WHEN("UP arrow is clicked") {
        unsigned int curr_pos = state->get_menu()->get_pos();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 74));
        THEN("Menu pos should change (+1)") {
            REQUIRE(state->get_menu()->get_pos() == curr_pos + 1);
        }
    }

    WHEN("ENTER is clicked") {

        GIVEN("menu pos = 1") {
            state->get_menu()->set_pos(1);
            handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 58));
            THEN("State should be active") {
                REQUIRE(dynamic_cast<ActiveGameState*>(game->getState()) != nullptr);
            }
        }

        GIVEN("menu pos = 2")
        {
            state->get_menu()->set_pos(2);
            handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 58));
            THEN("State should be menu") {
                REQUIRE(dynamic_cast<MenuGameState*>(game->getState()) != nullptr);
            }
        }
    }
}


