//
// Created by tomek on 03.05.2022.
//

#include <string>
#include <memory>

#include "include/EntityController.h"

#include "../core-lib/include/EventQueue.h"
#include "../core-lib/include/Event.h"


const char *TankDoesNotExistException::what() {
    return "Trying to access a non-existent tank";
}

EntityController::EntityController() {
    eventQueue_ = EventQueue<Event>::instance();
}


std::unique_ptr<Tank> EntityController::createTank(unsigned int x, unsigned int y, Tank::TankType type) {
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
    return std::move(newTank);
}

void EntityController::hitTank(Tank *target, unsigned int damage) {
    if (target->getLives() <= damage) {
        killTank(target);
    } else {
        target->deltaLives(-static_cast<int>(damage));
        eventQueue_->registerEvent(std::make_unique<Event>(Event::TankHit, target));
    }
}

void EntityController::killTank(Tank *target) {
    auto const &iter = std::find_if(entities_.begin(), entities_.end(), [&target](std::unique_ptr<Entity> &tank) {
        return tank.get() == target;
    });

    if (iter == entities_.end()) {
        throw TankDoesNotExistException();
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::TankKilled, target));  // TODO MOVE ALL EVENTS TO BOARD

    entities_.erase(iter);
}

void EntityController::removeEntity(Entity *target) {
    auto const &iter = std::find_if(entities_.begin(), entities_.end(), [&target](std::unique_ptr<Entity> &entity) {
        return entity.get() == target;
    });

    if (iter == entities_.end()) {
        throw TankDoesNotExistException();
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::EntityRemoved, target));

    entities_.erase(iter);
}

void EntityController::moveAllEntities() {
    for (std::unique_ptr<Entity> &target: entities_) {
        moveEntity(target.get());
    }
}

void EntityController::moveEntity(Entity *target) {
    if (target->move()) {  // TODO Check collisions before moving (in Board class maybe?)
        eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityMoved, target));
    }
}

void EntityController::setTankMoving(Tank *target, bool isMoving) {
    target->setMoving(isMoving);
}

void EntityController::setTankDirection(Tank *target, Direction direction) {
    target->setFacing(direction);
}

std::optional<Entity *> EntityController::getEntityAtPosition(float x, float y) {
    for (auto &entity: entities_) {
        if (x >= entity->getX() && entity->getX() + entity->getSizeX() > x) {   //try as a single expression
            if (x >= entity->getY() && entity->getY() + entity->getSizeY() > y) {
                return entity.get();
            }
        }
    }
    return std::nullopt;
}

PlayerTank *EntityController::getPlayer() {
    return player_;
}

bool EntityController::checkEntityCollisions(Entity *target) {
    for (std::unique_ptr<Entity> &entity_unq_ptr: entities_) {
        Entity *entity = entity_unq_ptr.get();
        if (target->getX() >= entity->getX() + entity->getSizeX() ||
            target->getX() + target->getSizeX() <= entity->getX() ||
            target->getY() >= entity->getY() + entity->getSizeY() ||
            target->getY() + target->getSizeY() <= entity->getY()) {
            continue;  // no collision
        } else if (target != entity) {
            return true;  // collision detected
        }
    }
    return false;  // went through all entities and found no collisions
}

PlayerTank *EntityController::addEntity(std::unique_ptr<PlayerTank> playerTank) {
    entities_.push_back(std::move(playerTank));
    player_ = dynamic_cast<PlayerTank*>(entities_.back().get());
    return player_;
}

Entity* EntityController::addEntity(std::unique_ptr<Entity> newEntity) {
    entities_.push_back(std::move(newEntity));
    return entities_.back().get();
}

std::vector<std::unique_ptr<Entity>> *EntityController::getAllEntities() {
    return &entities_;
}

void EntityController::clear() {
    for(auto iter = entities_.rbegin(); iter != entities_.rend(); iter++){
        eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityRemoved, iter->get()));
        entities_.erase(iter.base());
    }
}