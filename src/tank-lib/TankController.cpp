//
// Created by tomek on 03.05.2022.
//

#include <string>
#include <memory>

#include "include/TankController.h"

#include "../core-lib/include/EventQueue.h"
#include "../core-lib/include/Event.h"


TankOverlapException::TankOverlapException(unsigned int x, unsigned int y) : std::exception() {
    what_message = "Tank at (" + std::to_string(x) + ", " + std::to_string(y) + ") overlaps with another object";
}

const char *TankOverlapException::what() {
    return what_message.c_str();
}

const char *TankDoesNotExistException::what() {
    return "Trying to access a non-existent tank";
}

TankController::TankController() {
    eventQueue_ = EventQueue<Event>::instance();
}

void TankController::spawnTank(unsigned int x, unsigned int y, Tank::TankType type) {
    // TODO if overlaps, throw exception
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
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::TankSpawned, newTank.get()));
    tanks_.push_back(std::move(newTank));
}

void TankController::hitTank(Tank *target, unsigned int damage) {
    if(target->getLives() <= damage){
        killTank(target);
    } else{
        target->deltaLives(-static_cast<int>(damage));
        eventQueue_->registerEvent(std::make_unique<Event>(Event::TankHit, target));
    }
}

void TankController::killTank(Tank *target) {
    auto const& iter = std::find_if(tanks_.begin(), tanks_.end(), [&target](std::unique_ptr<Tank>& tank) {
        return tank.get() == target;
    });

    if(iter == tanks_.end()){
        throw TankDoesNotExistException();
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::TankKilled, target));

    tanks_.erase(iter);
}

void TankController::removeTank(Tank *target) {
    auto const& iter = std::find_if(tanks_.begin(), tanks_.end(), [&target](std::unique_ptr<Tank>& tank) {
        return tank.get() == target;
    });

    if(iter == tanks_.end()){
        throw TankDoesNotExistException();
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::TankRemoved, target));

    tanks_.erase(iter);
}

void TankController::moveTank(Tank * target, Direction direction) {
    //TODO stop before obstacle if collision
    switch (direction) {
        case North:{
            target->moveY(-1);
            break;
        }
        case West:{
            target->moveX(-1);
            break;
        }
        case South:{
            target->moveY(1);
            break;
        }
        case East:
            target->moveY(1);
            break;
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::TankMoved, target));
}

std::optional<Tank *> TankController::getTankAtPosition(unsigned int x, unsigned int y) {
    //assume all tanks are 13x13
    for(auto& tank: tanks_){
        if(x >= tank->getX() &&  tank->getX() + 13 > x){   //try as a single expression
            if(x >= tank->getY() && tank->getY() + 13 > y){
                return tank.get();
            }
        }
    }
    return std::nullopt;
}