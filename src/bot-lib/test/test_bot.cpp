//
// Created by tomek on 06.06.2022.
//

#include <memory>

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
            BotController *eventQueue = BotController::instance();
            eventQueue->deregisterAllBots();
            return eventQueue;
        }
    }
}

SCENARIO("Creating and deleting a bot") {
    Clock::initialize(60);
    auto botController = helper::getEmptyBotController();

    // Will not work, bots are deregistered in Board
//    WHEN("Initializing a bot object") {
//        std::shared_ptr<helper::TestBot> bot = std::make_shared<helper::TestBot>();
//
//        THEN("Object should be subscribed to Clock and added to BotController") {
//            REQUIRE(bot->getSubscribedSubjects()->size() == 1);
//            REQUIRE(botController->getRegisteredBotsCount() == 1);
//
//            AND_WHEN("Deleting the object") {
//                bot.reset();
//
//                THEN("Registered bot count should decrease") {
//                    REQUIRE(botController->getRegisteredBotsCount() == 0);
//                }
//            }
//        }
//    }
}

SCENARIO("Bot asking the controller for decision") {
    GIVEN("All required components were already initialized; a bot is added to the controller") {
        Clock::initialize(60);
        auto clock = Clock::instance();
        auto eventQueue = helper::getEmptyEventQueue();
        auto botController = helper::getEmptyBotController();

        std::shared_ptr<helper::TestBot> bot = std::make_shared<helper::TestBot>();
        WHEN("A required amount of time passes") {
            for (int i = 0; i < bot->getMaxDecisionCooldown() - 1; ++i) {
                clock->tick();
                REQUIRE(eventQueue->isEmpty());
            }
            clock->tick();
            THEN("Bot should add an event to the queue") {
                REQUIRE_FALSE(eventQueue->isEmpty());
                auto event = eventQueue->pop();

                REQUIRE(event->type == Event::BotDecisionRequest);
                REQUIRE((event->info.botInfo.bot) == std::static_pointer_cast<Bot>(bot));

                REQUIRE(eventQueue->isEmpty());

                AND_WHEN("The samee amount of time passes again") {
                    for (int i = 0; i < bot->getMaxDecisionCooldown() - 1; ++i) {
                        clock->tick();
                        REQUIRE(eventQueue->isEmpty());
                    }
                    clock->tick();
                    THEN("Bot should add an event to the queue") {
                        REQUIRE_FALSE(eventQueue->isEmpty());
                        event = eventQueue->pop();

                        REQUIRE(event->type == Event::BotDecisionRequest);
                        REQUIRE((event->info.botInfo.bot) == std::static_pointer_cast<Bot>(bot));

                        REQUIRE(eventQueue->isEmpty());
                    }
                }
            }
        }
    }
}
