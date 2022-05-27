//
// Created by tomek on 03.05.2022.
//

#include <string>
#include <memory>

#include "include/TankController.h"

#include "../core-lib/include/EventQueue.h"
#include "../core-lib/include/Event.h"


const char *TankDoesNotExistException::what() {
    return "Trying to access a non-existent tank";
}

TankController::TankController() {
    eventQueue_ = EventQueue<Event>::instance();
}

Tank * TankController::spawnTank(unsigned int x, unsigned int y, Tank::TankType type) {
    // TODO Check collisions before spawning (in Board class maybe?)
    std::unique_ptr<Tank> newTank;
    switch (type) {
        case Tank::BasicTank: {
            newTank = std::make_unique<BasicTank>(x, y);
            break;
        }
        case Tank::FastTank: {
            newTank = std::make_unique<FastTank>(x, y);
            break;
        }
        case Tank::PowerTank: {
            newTank = std::make_unique<PowerTank>(x, y);
            break;
        }
        case Tank::ArmorTank: {
            newTank = std::make_unique<ArmorTank>(x, y);
            break;
        }
        case Tank::PlayerTank: {
            newTank = std::make_unique<PlayerTank>(x, y, 1);    // TODO set lives
        }
    }
    if (type != Tank::PlayerTank) {
        tanks_.push_back(std::move(newTank));
        eventQueue_->registerEvent(std::make_unique<Event>(Event::TankSpawned, tanks_.back().get()));
        return tanks_.back().get();
    }else{
        player_ = std::unique_ptr<PlayerTank>(dynamic_cast<PlayerTank*>(newTank.release()));
        eventQueue_->registerEvent(std::make_unique<Event>(Event::TankSpawned, player_.get()));
        return player_.get();
    }
}


void TankController::hitTank(Tank *target, unsigned int damage) {
    if (target->getLives() <= damage) {
        killTank(target);
    } else {
        target->deltaLives(-static_cast<int>(damage));
        eventQueue_->registerEvent(std::make_unique<Event>(Event::TankHit, target));
    }
}

void TankController::killTank(Tank *target) {
    auto const &iter = std::find_if(tanks_.begin(), tanks_.end(), [&target](std::unique_ptr<Tank> &tank) {
        return tank.get() == target;
    });

    if (iter == tanks_.end()) {
        throw TankDoesNotExistException();
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::TankKilled, target));

    tanks_.erase(iter);
}

void TankController::removeTank(Tank *target) {
    auto const &iter = std::find_if(tanks_.begin(), tanks_.end(), [&target](std::unique_ptr<Tank> &tank) {
        return tank.get() == target;
    });

    if (iter == tanks_.end()) {
        throw TankDoesNotExistException();
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::TankRemoved, target));

    tanks_.erase(iter);
}

void TankController::moveAllTanks() {
    for(std::unique_ptr<Tank> &target: tanks_){
        moveTank(target.get());
    }
    moveTank(player_.get());
}

void TankController::moveTank(Tank *target) {
    if (!target->isMoving())
        return;

    target->move();  // TODO Check collisions before moving (in Board class maybe?)
    eventQueue_->registerEvent(std::make_unique<Event>(Event::TankMoved, target));
}

void TankController::setTankMoving(Tank *target, bool isMoving) {
    target->setMoving(isMoving);
}

void TankController::setTankDirection(Tank *target, Direction direction) {
    target->setFacing(direction);
    eventQueue_->registerEvent(std::make_unique<Event>(Event::TankRotated, target));

}

std::optional<Tank *> TankController::getTankAtPosition(float x, float y) {
    //assume all tanks are 2x2
    for (auto &tank: tanks_) {
        if (x >= tank->getX() && tank->getX() + 2 > x) {   //try as a single expression
            if (x >= tank->getY() && tank->getY() + 2 > y) {
                return tank.get();
            }
        }
    }
    return std::nullopt;
}

PlayerTank *TankController::getPlayer() {
    return player_.get();
}