//
// Created by tomek on 06.06.2022.
//

#include <algorithm>
#include <random>

#include "include/BotController.h"
#include "include/Bot.h"
#include "../core-lib/include/Clock.h"
#include "../core-lib/include/SingletonExceptions.h"

const char *NoSpawnpointException::what() const noexcept {
    return "At least one spawnpoint is needed to spawn a bot";
}

BotController::BotController(unsigned int n_maxRegisteredBots, unsigned int n_spawnCooldown) : maxSpawnCooldown(
        n_spawnCooldown),
                                                                                               spawnCooldown(
                                                                                                       maxSpawnCooldown),
                                                                                               maxRegisteredBots_(
                                                                                                       n_maxRegisteredBots),
                                                                                               registeredBots_(0),
                                                                                               eventQueue_(
                                                                                                       EventQueue<Event>::instance()) {
};

void BotController::makeBotDecision(const std::shared_ptr<Bot>& bot) {
    enum action : unsigned int {
        MoveForward = 0,
        RotateLeft,
        RotateRight,
        Fire
    };

    std::default_random_engine generator;
    std::vector<int> distribution {4, 1, 1, 2};
    auto pick = std::rand() % 8;
    for (int i = 0; i < distribution.size(); i++) {
        pick -= distribution[i];
        if (pick < 0) {
            pick = i;
            break;
        }
    }

    // auto pick = static_cast<action>(distribution(generator));

    switch (pick) {
        case MoveForward:{
            eventQueue_->registerEvent(std::make_unique<Event>(Event::BotMoveDecision, bot, true));
            break;
        }
        case RotateLeft:{
            eventQueue_->registerEvent(std::make_unique<Event>(Event::BotRotateDecision, bot, static_cast<int>((bot->getFacing()-1)%4)));
            break;
        }
        case RotateRight:{
            eventQueue_->registerEvent(std::make_unique<Event>(Event::BotRotateDecision, bot, static_cast<int>((bot->getFacing()+1)%4)));
            break;
        }
        case Fire:{
            eventQueue_->registerEvent(std::make_unique<Event>(Event::BotFireDecision, bot));
            break;
        }
        default: {};
    }


    if (rotateRoll == 0) {
        eventQueue_->registerEvent(
                std::make_unique<Event>(Event::BotRotateDecision, bot, static_cast<int>((bot->getFacing() - 1) % 4)));

    } else if (rotateRoll == 1) {
        eventQueue_->registerEvent(
                std::make_unique<Event>(Event::BotRotateDecision, bot, static_cast<int>((bot->getFacing() + 1) % 4)));

    }



}

void BotController::registerBot() {
    registeredBots_++;
}

void BotController::deregisterBot() {
    if (registeredBots_ == maxRegisteredBots_) {
        spawnCooldown = maxSpawnCooldown;
    }
    registeredBots_--;
}

void BotController::deregisterAllBots() {
    registeredBots_ = 0;
}

unsigned int BotController::getRegisteredBotsCount() const {
    return registeredBots_;
}

void BotController::notify(SimplePublisher *pub) {
    if (!counting)
        return;
    spawnCooldown = std::min(spawnCooldown, spawnCooldown - 1); // prevent overflow
    if (registeredBots_ < maxRegisteredBots_ && spawnCooldown == 0) {
        requestSpawnBot();
        spawnCooldown = maxSpawnCooldown;
    }
}

std::unique_ptr<BotController> BotController::self_ = nullptr;

BotController *BotController::instance() {
    if (self_ == nullptr) {
        throw SingletonNotInitializedException();
    }
    return self_.get();
}

void BotController::initialize(unsigned int maxRegisteredBots, unsigned int spawnCooldown) {
    self_ = std::unique_ptr<BotController>(new BotController(maxRegisteredBots, spawnCooldown));
}

void BotController::setSpawnpoints(const std::vector<std::pair<unsigned int, unsigned int>> &spawnpoints) {
    spawnpoints_ = spawnpoints;
}

void BotController::requestSpawnBot() {
    if (spawnpoints_.empty()) {
        throw NoSpawnpointException();
    }

    if (types_.empty()) {
        return;
    }

    Tank::TankType type = types_.front();
    types_.pop();

    std::vector<std::pair<unsigned int, unsigned int>> out;
    std::sample(
            spawnpoints_.begin(),
            spawnpoints_.end(),
            std::back_inserter(out),
            1,
            std::mt19937{std::random_device{}()}
    );
    std::pair<unsigned int, unsigned int> spawnpoint = out.front();

    EventQueue<Event>::instance()->registerEvent(std::make_unique<Event>(Event::BotSpawnDecision,
                                                                         spawnpoint.first,
                                                                         spawnpoint.second,
                                                                         type,
                                                                         South));
}

void BotController::setTypes(const std::queue<Tank::TankType> &types) {
    types_ = types;
}


void BotController::setCounting(bool nCounting) {
    counting = nCounting;
}