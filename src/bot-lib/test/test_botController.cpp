//
// Created by tomek on 06.06.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/Bot.h"
#include "../include/BotController.h"
#include "../../core-lib/include/Clock.h"
#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

namespace {
    namespace helper {
        class TestBot : public Bot {
        public:
            TestBot() : Bot(1, 1, 1, 1, 1, North) {}

            bool move() override {}

            bool moveBack() override {}

            std::vector<SimplePublisher *> *getSubscribedSubjects() {
                return &subscribedSubjects_;
            }

            unsigned int getDecisionCooldown() {
                return decisionCooldown;
            }

            unsigned int getMaxDecisionCooldown() {
                return maxDecisionCooldown;
            }
        };

        EventQueue<Event> *getEmptyEventQueue() {
            EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
            eventQueue->clear();
            return eventQueue;
        }

        BotController *getEmptyBotController() {
            BotController::initialize(4, 240);
            BotController *botController = BotController::instance();
            botController->deregisterAllBots();
            return botController;
        }
    }
}

SCENARIO("Initializing the controller object") {
    WHEN("The controller object is created") {
        BotController *controller = helper::getEmptyBotController();

        THEN("Object should be subscribed to Clock") {
            // ??? tested in test_bot
        }
    }
}


SCENARIO("Requesting to spawn tanks") {
    GIVEN("Some objects idk it's 4:28 AM") {
        Clock::initialize(60);
        auto clock = Clock::instance();
        auto eventQueue = helper::getEmptyEventQueue();
        auto botController = helper::getEmptyBotController();

        WHEN("A certain amount of time passes") {
            botController->setSpawnpoints({{1, 2}});

            for (int i = 0; i < 239; ++i) {
                clock->tick();
                REQUIRE(eventQueue->isEmpty());
            }
            clock->tick();

            THEN("Bot spawn decision should be added to the queue") {
                REQUIRE_FALSE(eventQueue->isEmpty());
                auto event = eventQueue->pop();
                REQUIRE(event->type == Event::BotSpawnDecision);
                REQUIRE(event->info.spawnDecisionInfo.x == 1);
                REQUIRE(event->info.spawnDecisionInfo.y == 2);

                REQUIRE(eventQueue->isEmpty());

                AND_WHEN("The maximum number of registered bots is reached") {
                    std::shared_ptr<helper::TestBot> b1 = std::make_shared<helper::TestBot>();
                    std::shared_ptr<helper::TestBot> b2 = std::make_shared<helper::TestBot>();
                    std::shared_ptr<helper::TestBot> b3 = std::make_shared<helper::TestBot>();
                    std::shared_ptr<helper::TestBot> b4 = std::make_shared<helper::TestBot>();

                    REQUIRE(botController->getRegisteredBotsCount() == 4);

                    THEN("No more spawn decisions should be generated") {
                        for (int i = 0; i < 500; ++i) {
                            clock->tick();
                        }
                        while (!eventQueue->isEmpty()) {
                            REQUIRE(eventQueue->pop()->type != Event::BotSpawnDecision);
                        }
                    }
                }
            }
        }WHEN("No spawnpoints are provided") {
            BotController::initialize(4, 5);

            for (int i = 0; i < 4; ++i) {
                clock->tick();
                REQUIRE(eventQueue->isEmpty());
            }

            THEN("An exception should be thrown") {
                REQUIRE_THROWS_AS(clock->tick(), NoSpawnpointException);
            }
        }
    }
}

SCENARIO("Making random bot decisions") {
    GIVEN("Some objects") {
        Clock::initialize(60);
        auto clock = Clock::instance();
        auto eventQueue = helper::getEmptyEventQueue();
        auto botController = helper::getEmptyBotController();

        std::shared_ptr<helper::TestBot> bot = std::make_shared<helper::TestBot>();

        WHEN("Making random decisions for the bot") {
            THEN("Nothing should break lol") {
                std::unique_ptr<Event> event;
                for (int i = 0; i < 80; ++i) {
                    botController->makeBotDecision(bot);
                    event = eventQueue->pop();

                    switch (event->type) {
                        case Event::BotMoveDecision:{
                            REQUIRE(event->info.moveDecisionInfo.bot == bot);
                            REQUIRE(event->info.moveDecisionInfo.flag == true);
                            break;
                        }
                        case Event::BotRotateDecision:{
                            REQUIRE(event->info.rotateDecisionInfo.bot == bot);
                            REQUIRE(event->info.rotateDecisionInfo.direction % 2 != bot->getFacing() % 2);
                            break;
                        }
                        case Event::BotFireDecision:{
                            REQUIRE(event->info.fireDecisionInfo.bot == bot);
                            break;
                        }
                        default:
                            FAIL("what the\nwhat the hell dude :(");  // invalid event type
                    }

                }
            }
        }
    }
}