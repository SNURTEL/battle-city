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
#include "../../bot-lib/include/BotController.h"
#include "../../core-lib/include/Clock.h"
#include "../../core-lib/include/Event.h"

//TODO test the constructor lmao
namespace {
    namespace helper {
        class TestGame : public Game {
        public:
            explicit TestGame(unsigned int clockFreq) : Game(clockFreq) {};

            void testSetup() {

                initUI();

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
        BotController *getEmptyBotController() {
            BotController *botController = BotController::instance();
            botController->deregisterAllBots();
            return botController;
        }

        void initSingletons() {
            BotController::initialize(4, 240);
            BotController::instance()->subscribe(Clock::instance());
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
    WHEN("Left arrow clicked") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 71));
        THEN("Player tank should be moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == true);
        }
        THEN("Direction should be WEST") {
            REQUIRE(game->getBoard()->getPlayerTank()->getFacing() == West);
        }
    }
    WHEN("LEFT arrow released") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyReleased, 71));
        THEN("Should stop moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == false);
        }
    }
    /////////////////////////////
    WHEN("RIGHT arrow clicked") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 72));
        THEN("Player tank should be moving") {
            REQUIRE(game->getBoard()->getPlayerTank()->isMoving() == true);
        }
        THEN("Direction should be EAST") {
            REQUIRE(game->getBoard()->getPlayerTank()->getFacing() == East);
        }
    }
    WHEN("RIGHT arrow released") {
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
        unsigned int lives = game->getStats()->getLives();
        Event::CollisionMember member1 = Event::PlayerTankCollisionInfo{player_tank};
        Event::CollisionMember member2 = Event::EnemyBulletCollisionInfo{enemy_bullet};
        handler->handleEvent(std::make_unique<Event>(Event::EventType::Collision, member1, member2));
        THEN("Player lives -1") {
            REQUIRE(game->getStats()->getLives() == lives - 1);
        }
    }

    WHEN("Player bullet collides with enemy tank") {
        Event::CollisionMember member1 = Event::FriendlyBulletCollisionInfo{player_bullet};
        Event::CollisionMember member2 = Event::EnemyTankCollisionInfo{enemy_tank};
        eq->clear();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::Collision, member1, member2));
        THEN("Enemy tank gets deleted (2 events: remove bullet, remove tank)") {
            REQUIRE(eq->size() == 2);
        }
    }

    WHEN("Enemy bullet collides with a friendly bullet") {
        Event::CollisionMember member1 = Event::FriendlyBulletCollisionInfo{player_bullet};
        Event::CollisionMember member2 = Event::EnemyBulletCollisionInfo{enemy_bullet};
        eq->clear();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::Collision, member1, member2));
        THEN("Nothing happens") {
            REQUIRE(eq->size() == 0);
        }
    }

    WHEN("Bullet hits breakable tile") {
        Event::CollisionMember member1 = Event::FriendlyBulletCollisionInfo{player_bullet};
        game->getBoard()->getGrid()->setTile(10, 10, TileType::Bricks);
        Event::CollisionMember member2 = Event::BoardCollisionInfo{10, 10, game->getBoard()->getGrid()};
        eq->clear();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::Collision, member1, member2));
        THEN("Tile and bullet are deleted (two events)") {
            REQUIRE(eq->size() == 1);

        }
    }

    WHEN("Bullet hits unbreakable tile") {
        Event::CollisionMember member1 = Event::FriendlyBulletCollisionInfo{player_bullet};
        game->getBoard()->getGrid()->setTile(10, 10, TileType::Steel);
        Event::CollisionMember member2 = Event::BoardCollisionInfo{10, 10, game->getBoard()->getGrid()};
        eq->clear();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::Collision, member1, member2));
        THEN("Only bullet gets deleted (one event)") {
            REQUIRE(eq->size() == 1);
        }
    }

    WHEN("Tank hits a tile") {
        Event::CollisionMember member1 = Event::PlayerTankCollisionInfo{player_tank};
        Event::CollisionMember member2 = Event::BoardCollisionInfo{10, 10, game->getBoard()->getGrid()};
        eq->clear();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::Collision, member1, member2));
        THEN("Tank should be moved back") {
            REQUIRE(eq->size() == 1);
        }
    }

    WHEN("Player tank dies") {
        eq->clear();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::PlayerKilled, player_tank));
        THEN("game should end (Finished state") {
            REQUIRE(dynamic_cast<FinishedGameState*>(game->getState()) != nullptr);
        }
    }

    WHEN("Enemy tank dies") {
        eq->clear();
        unsigned int points = game->getStats()->getPoints();
        unsigned int delta_points = enemy_tank->getPoints();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::TankKilled, enemy_tank));
        THEN("Player points +") {
            REQUIRE(game->getStats()->getPoints() == points + delta_points);
        }
    }
}

SCENARIO("Bot events") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    std::unique_ptr<helper::TestGame> game = std::make_unique<helper::TestGame>(60);
    game->testSetup();
    game->testStart();
    ActiveGameState* state = dynamic_cast<ActiveGameState*>(game->getState());
    ActiveEventHandler* handler = dynamic_cast<ActiveEventHandler*>(state->getEventHandler());
    std::shared_ptr<PlayerTank> player_tank = game->getBoard()->getPlayerTank();
    eq->clear();
    std::shared_ptr<Tank> enemy_tank = helper::placeTank(game->getBoard(), 10, 10, Tank::ArmorTank);
    std::shared_ptr<Bullet> enemy_bullet = helper::fireBullet(game->getBoard(), enemy_tank).value();
    eq->clear();
    std::shared_ptr<Bullet> player_bullet = helper::fireBullet(game->getBoard(), player_tank).value();
    eq->clear();
    helper::initSingletons();
    BotController* bot_controller = helper::getEmptyBotController();

    WHEN("Bot spawn decision") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::BotSpawnDecision, 5, 5, Tank::ArmorTank));
        handler->handleEvent(std::make_unique<Event>(Event::EventType::BotSpawnDecision, 5, 7, Tank::ArmorTank));
        THEN("Bot should be registered") {
            REQUIRE(bot_controller->getRegisteredBotsCount() == 2);
        }
    }
    WHEN("Bot gets killed") {
//        bot_controller = helper::getEmptyBotController();
//        std::shared_ptr<Tank> testPlayerTank = helper::placeTank(game->getBoard(), 40, 40, Tank::BasicTank);
//        std::unique_ptr<Event::CollisionMember> member1 = std::make_unique<Event::CollisionMember>(Event::FriendlyBulletCollisionInfo{player_bullet});
//        std::unique_ptr<Event::CollisionMember> member2 = std::make_unique<Event::CollisionMember>(Event::EnemyTankCollisionInfo{testPlayerTank});
//        eq->clear();
////        testPlayerTank.reset();
//        auto collisionEvent = std::make_unique<Event>(Event::EventType::Collision, *(member1.get()), *(member2.get()));
//
//        eq->clear();  // ensure no additional testPlayerTank shared_ptrs exist
//        member1.reset();
//        member2.reset();
//
//        handler->handleEvent(std::move(collisionEvent));
//
//        eq->clear();  // ensure no additional testPlayerTank shared_ptrs exist
//
//        THEN("Bot should be registered") {
//
//            REQUIRE(bot_controller->getRegisteredBotsCount() == 0);
//        }

//         bot_controller = helper::getEmptyBotController();
// //        for(int i = 0; i<1; i++){
//             std::shared_ptr<Tank> testPlayerTank = helper::placeTank(game->getBoard(), 40, 40, Tank::PlayerTank);
//             std::shared_ptr<Tank> testEnemyTank = helper::placeTank(game->getBoard(), 40, 40, Tank::BasicTank);
//             std::shared_ptr<Bullet> testBullet = helper::fireBullet(game->getBoard(), testPlayerTank).value();
//             eq->registerEvent(std::make_unique<Event>(Event::Collision, Event::FriendlyBulletCollisionInfo{testBullet}, Event::EnemyTankCollisionInfo{testEnemyTank}));
// //        }
//         handler->handleEvent(eq->pop());
//         auto aaa = testEnemyTank.use_count();
//         eq->pop();
//         eq->pop();
//         eq->pop();
//         aaa = testEnemyTank.use_count();

// //        game->getBoard()->removeEntity(testBullet);

//         REQUIRE(bot_controller->getRegisteredBotsCount() == 0);


    }

    WHEN("Bot Move decision (true)") {
        bot_controller = helper::getEmptyBotController();
        bot_controller->registerBot();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::BotMoveDecision, std::dynamic_pointer_cast<Bot>(enemy_tank), true));
        THEN("Bot should be moving") {

            REQUIRE(enemy_tank->isMoving() == true);
        }
    }

    WHEN("Bot Move decision (false)") {
        bot_controller = helper::getEmptyBotController();
        bot_controller->registerBot();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::BotMoveDecision, std::dynamic_pointer_cast<Bot>(enemy_tank), false));
        THEN("Bot should be not moving") {
            REQUIRE(enemy_tank->isMoving() == false);
        }
    }

    WHEN("Bot Rotate decision (to South)")
    {
        REQUIRE(enemy_tank->getFacing() == North);
        bot_controller = helper::getEmptyBotController();
        bot_controller->registerBot();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::BotRotateDecision, std::dynamic_pointer_cast<Bot>(enemy_tank), 2));
        THEN("Facing should be South") {
            REQUIRE(enemy_tank->getFacing() == South);
        }
    }

    WHEN("Bot Fire decision") {
        bot_controller = helper::getEmptyBotController();
        bot_controller->registerBot();
        handler->handleEvent(std::make_unique<Event>(Event::EventType::BotFireDecision, std::dynamic_pointer_cast<Bot>(enemy_tank)));
        THEN("Bot should create a bullet (cant create another)") {
           REQUIRE_FALSE(enemy_tank->createBullet().has_value());
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

SCENARIO("State is finished") {
    EventQueue<Event> * eq = helper::getEmptyEventQueue();
    std::unique_ptr<helper::TestGame> game = std::make_unique<helper::TestGame>(60);
    game->testSetup();
    game->setFinishedState();
    FinishedGameState* state = dynamic_cast<FinishedGameState*>(game->getState());
    FinishedEventHandler* handler = dynamic_cast<FinishedEventHandler*>(state->getEventHandler());

    WHEN("Enter is clicked") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 58));\
        THEN("Game should quit") {
            REQUIRE_FALSE(game->testIsRunning());
        }
    }

    WHEN("Esc is clicked") {
        handler->handleEvent(std::make_unique<Event>(Event::EventType::KeyPressed, 36));
        THEN("Game should quit") {
            REQUIRE_FALSE(game->testIsRunning());
        }
    }
}

