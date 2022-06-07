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
        std::shared_ptr<Tank>
        placeTank(Board *board, unsigned int x, unsigned int y, Tank::TankType type,
                  Direction facing = North) {
            bool collision = !board->spawnTank(x, y, type, facing);

            auto tank = EventQueue<Event>::instance()->pop()->info.entityInfo.entity;

            std::shared_ptr<Tank> result = std::dynamic_pointer_cast<Tank>(tank);
            if (collision)
                EventQueue<Event>::instance()->pop();

            return result;
        }

        std::optional<std::shared_ptr<Bullet>> fireBullet(Board *board, std::shared_ptr<Tank> tank) {
            unsigned int prevSize = EventQueue<Event>::instance()->size();
            if (!board->fireTank(tank)) {
                return std::nullopt;
            }

            auto bullet = std::dynamic_pointer_cast<Bullet>(EventQueue<Event>::instance()->pop()->info.entityInfo.entity);

            if (prevSize + 1 == EventQueue<Event>::instance()->size()) {
                EventQueue<Event>::instance()->pop();
            }
            return bullet;
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
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 57));
        THEN("Player should create a bullet (cant create another)") {
           REQUIRE_FALSE(game->getBoard()->getPlayerTank()->createBullet().has_value());
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

    WHEN ("Player Killed event") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::PlayerKilled, game->getBoard()->getPlayerTank()));
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
    ActiveGameState* state = dynamic_cast<ActiveGameState*>(game->getState());
    Event keypressed(Event::KeyPressed, 74);
    ActiveEventHandler* handler = dynamic_cast<ActiveEventHandler*>(state->getEventHandler());
    std::shared_ptr<PlayerTank> player_tank = game->getBoard()->getPlayerTank();
    eq->clear();
    std::shared_ptr<Tank> enemy_tank = helper::placeTank(game->getBoard(), 10, 10, Tank::ArmorTank);
    std::shared_ptr<Bullet> enemy_bullet = helper::fireBullet(game->getBoard(), enemy_tank).value();
    eq->clear();
    std::shared_ptr<Bullet> player_bullet = helper::fireBullet(game->getBoard(), player_tank).value();
    
    WHEN("Tank collides with tank") {
        Event::CollisionMember member1 = Event::PlayerTankCollisionInfo{player_tank};
        Event::CollisionMember member2 = Event::EnemyTankCollisionInfo{enemy_tank};
        eq->clear();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::Collision, member1, member2));
        THEN("They should move back") {
            REQUIRE(eq->size() > 0);
        }
    }
    WHEN("Enemy bullet collides with player tank") {
        unsigned int lives = game->getBoard()->getPlayerTank()->getLives();
        Event::CollisionMember member1 = Event::PlayerTankCollisionInfo{player_tank};
        Event::CollisionMember member2 = Event::EnemyBulletCollisionInfo{enemy_bullet};
        handler->handleEvent(std::make_unique<Event>(Event::EventType::Collision, member1, member2));
        THEN("Player lives -1") {
            REQUIRE(game->getBoard()->getPlayerTank()->getLives() == lives - 1);
        }
    }

    WHEN("Player bullet collides with enemy tank") {
        unsigned int points = game->getStats()->getPoints();
        Event::CollisionMember member1 = Event::FriendlyBulletCollisionInfo{player_bullet};
        Event::CollisionMember member2 = Event::EnemyTankCollisionInfo{enemy_tank};
        unsigned int delta_points = enemy_tank->getPoints();
        eq->clear();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::Collision, member1, member2));
        THEN("Player points +") {
            REQUIRE(game->getStats()->getPoints() == points + delta_points);
        }

        THEN("Enemy tank gets deleted") {
            REQUIRE(eq->size() > 0);
        }
    }

    WHEN("Enemy bullet collides with a friendly bullet") {
        //Bullet* bullet_enemy = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Enemy));
        //Bullet* bullet_friendly = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Friendly));
        // needs Entity* and gets Bullet*
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::EntityEntityCollision, bullet_enemy, bullet_friendly));
        THEN("Both bullets get deleted") {
            // todo
        }
    }

    WHEN("Bullet hits breakable tile") {
        //Bullet* bullet_enemy = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Enemy));
        //Bullet* bullet_friendly = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Friendly));
        // needs Entity* and gets Bullet*
        // handler->handleEvent(std::make_unique<Event>(Event::EventType::EntityGridCollision, bullet_enemy, 10, 10));
        THEN("Tile and bullet are deleted") {
            // REQUIRE_TRUE(game->getBoard()->getGrid()->isTileDeleted(10, 10))
            // todo
        }
    }

    WHEN("Bullet hits unbreakable tile") {
        //Bullet* bullet_enemy = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Enemy));
       // Bullet* bullet_friendly = game->getBoard()->getEntityController()->addEntity(std::make_unique<Bullet>(1, 1, North, 1, Bullet::Friendly));
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


