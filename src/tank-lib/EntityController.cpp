//
// Created by tomek on 03.05.2022.
//

#include <string>
#include <memory>

#include "include/EntityController.h"

#include "include/Tank.h"
#include "include/Bullet.h"

#include "../core-lib/include/EventQueue.h"
#include "../core-lib/include/Event.h"
#include "../core-lib/include/Clock.h"
#include "../bot-lib/include/BotController.h"


const char *EntityDoesNotExistException::what() {
    return "Trying to access a non-existent tank";
}

EntityController::EntityController() {
    eventQueue_ = EventQueue<Event>::instance();
}


std::shared_ptr<Tank>
EntityController::createTank(unsigned int x, unsigned int y, Tank::TankType type, Direction facing) {
    std::shared_ptr<Tank> newTank;
    switch (type) {
        case Tank::BasicTank: {
            newTank = std::make_shared<BasicTank>(x, y, facing);
            break;
        }
        case Tank::FastTank: {
            newTank = std::make_shared<FastTank>(x, y, facing);
            break;
        }
        case Tank::PowerTank: {
            newTank = std::make_shared<PowerTank>(x, y, facing);
            break;
        }
        case Tank::ArmorTank: {
            newTank = std::make_shared<ArmorTank>(x, y, facing);
            break;
        }
        case Tank::PlayerTank: {
            newTank = std::make_shared<PlayerTank>(x, y, facing);
        }
    }
    return std::move(newTank);
}

void EntityController::hitTank(std::shared_ptr<Tank> target, unsigned int damage) {
    if (target->getLives() <= damage) {  // if greater than health
        killTank(target);
    } else {
        target->deltaLives(-static_cast<int>(damage));  // if lower
        eventQueue_->registerEvent(std::make_unique<Event>(Event::TankHit, target));
    }
}

void EntityController::killTank(std::shared_ptr<Tank> tank) {
    if (dynamic_cast<Bot *>(tank.get()) != nullptr) {
        BotController::instance()->deregisterBot();
        dynamic_cast<Bot *>(tank.get())->unsubscribe(Clock::instance());
    }

    auto iter = std::find(entities_.begin(), entities_.end(), tank);

    if (iter == entities_.end()) {
        throw EntityDoesNotExistException();
    }
    if (tank->getType() == Tank::PlayerTank) {
        eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::PlayerKilled, tank));
    } else {
        eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::TankKilled, tank));
    }

    entities_.erase(iter);
}

void EntityController::removeEntity(std::shared_ptr<Entity> entity) {
    auto const &iter = std::find(entities_.begin(), entities_.end(), entity);

    if (iter == entities_.end()) {
        throw EntityDoesNotExistException();
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::EntityRemoved, entity));

    if (dynamic_cast<Bullet *>(entity.get()) != nullptr) {
        dynamic_cast<Bullet *>(entity.get())->unlink();
    }

    auto found = *iter;

    entities_.erase(iter);

}

void EntityController::moveAllEntities() {
    for (std::shared_ptr<Entity> &target: entities_) {
        moveEntity(target);
    }
}

bool EntityController::moveEntity(std::shared_ptr<Entity> target) {
    if (target->move()) {
        eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityMoved, target));
        return true;
    }
    return false;
}

void EntityController::setTankMoving(const std::shared_ptr<Tank> &target, bool isMoving) {
    target->setMoving(isMoving);
}

void EntityController::setTankDirection(const std::shared_ptr<Tank> &target, Direction direction) {
    target->setFacing(direction);
}

std::optional<std::shared_ptr<Entity>>
EntityController::findEntityAtPosition(float x, float y, std::optional<std::shared_ptr<Entity>> ignored) {
    for (auto entity: entities_) {
        if (x >= entity->getX() && entity->getX() + entity->getSizeX() > x) {   //try as a single expression
            if (y >= entity->getY() && entity->getY() + entity->getSizeY() > y) {
                if ((ignored.has_value() && ignored.value() != entity) || !ignored.has_value()) {
                    return entity;
                }
            }
        }
    }
    return std::nullopt;
}

std::shared_ptr<PlayerTank> EntityController::getPlayer() {
    return player_;
}

std::optional<std::shared_ptr<Entity>> EntityController::checkEntityCollisions(const std::shared_ptr<Entity> &target) {
    for (std::shared_ptr<Entity> entity: entities_) {
        if (target->getX() >= entity->getX() + entity->getSizeX() ||
            target->getX() + target->getSizeX() <= entity->getX() ||
            target->getY() >= entity->getY() + entity->getSizeY() ||
            target->getY() + target->getSizeY() <= entity->getY()) {
            continue;  // no collision
        } else if (target != entity) {

            return entity;  // collision detected
        }
    }
    return std::nullopt;  // went through all entities and found no collisions
}

std::shared_ptr<PlayerTank> EntityController::addEntity(const std::shared_ptr<PlayerTank> &playerTank) {
    entities_.push_back(playerTank);
    player_ = playerTank;
    return player_;
}

std::shared_ptr<Tank> EntityController::addEntity(std::shared_ptr<Tank> newTank) {
    entities_.push_back(newTank);
    return newTank;
}

std::shared_ptr<Bullet> EntityController::addEntity(std::shared_ptr<Bullet> newBullet) {
    entities_.push_back(newBullet);
    return newBullet;
}

std::shared_ptr<Entity> EntityController::addEntity(std::shared_ptr<Entity> newEntity) {
    entities_.push_back(newEntity);
    return newEntity;
}

std::vector<std::shared_ptr<Entity>> *EntityController::getAllEntities() {
    return &entities_;
}

void EntityController::clear() {
    for (auto iter = entities_.rbegin(); iter != entities_.rend(); iter++) {
        if (dynamic_cast<Bot *>(iter->get()) != nullptr) {
            dynamic_cast<Bot *>(iter->get())->unsubscribe(Clock::instance());
            BotController::instance()->deregisterBot();
        }

        eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityRemoved, *iter));
        entities_.erase(iter.base());
    }
}