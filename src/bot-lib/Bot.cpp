//
// Created by tomek on 06.06.2022.
//

#include "include/Bot.h"
#include "include/BotController.h"
#include "../core-lib/include/EventQueue.h"
#include "../core-lib/include/Event.h"
#include "../core-lib/include/Clock.h"

Bot::Bot(float x, float y, float sizeX, float sizeY, float speed, Direction facing) :
        Entity(x, y, sizeX, sizeY, speed, facing),
        botController(BotController::instance()),
        maxDecisionCooldown(6),
        decisionCooldown(maxDecisionCooldown) {
    botController->registerBot();
    subscribe(Clock::instance());
}

Bot::~Bot() {
//    botController->deregisterBot();
}

void Bot::notify(SimplePublisher *pub) {
    decisionCooldown = std::min(decisionCooldown, decisionCooldown - 1);  // prevent overflow
    if (decisionCooldown == 0) {
        requestDecision();
        decisionCooldown = maxDecisionCooldown;
    }
}

void Bot::requestDecision() {
    EventQueue<Event>::instance()->registerEvent(std::make_unique<Event>(Event::BotDecisionRequest,
                                                                         shared_from_this()));
}