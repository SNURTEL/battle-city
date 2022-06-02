//
// Created by tomek on 24.05.2022.
//

#include <cmath>

#include "../core-lib/include/EventQueue.h"
#include "../core-lib/include/Event.h"

#include "include/Board.h"
#include "include/Grid.h"
#include "include/GridBuilder.h"
#include "include/TileManager.h"
#include "../tank-lib/include/EntityController.h"

#include "../tank-lib/include/Tank.h"
#include "../tank-lib/include/Bullet.h"


Board::Board() : entityController_(std::make_unique<EntityController>()), grid_(std::make_unique<Grid>()) {}

void Board::setTankMoving(Tank *target, bool isMoving) {
    entityController_->setTankMoving(target, isMoving);
}

void Board::setTankDirection(Tank *tank, Direction targetDirection) {
    Direction initialDirection = tank->getFacing();

    if (initialDirection == targetDirection) {
        return;
    }

    if (!(abs(static_cast<int>(initialDirection - targetDirection)) == 2) &&
        !snapTankToGrid(tank, true, true)) {  // rotated by 180 deg
        return;
    }

    entityController_->setTankDirection(tank, targetDirection);
    eventQueue_->registerEvent(std::make_unique<Event>(Event::TankRotated, tank));


}

bool Board::snapTankToGrid(Tank *target, bool snap_x, bool snap_y) {
    float initial_x = target->getX();
    float initial_y = target->getY();

    if (snap_x)
        target->setX(std::round(target->getX()));
    if (snap_y)
        target->setY(std::round(target->getY()));

    if (!validateEntityPosition(target)) {
        target->setX(initial_x);
        target->setY(initial_y);

        return false;
    }

    eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityMoved, target));

    return true;
}

void Board::setGrid(std::unique_ptr<Grid> grid) {
    grid_ = std::move(grid);
}

void Board::deleteTile(unsigned int x, unsigned int y) {
    if (!TileManager::isTileDestructible(grid_->getTileAtPosition(x, y))) {
        return;
    }
    grid_->deleteTile(x, y);
}

void Board::moveAllEntities() {
    for (std::unique_ptr<Entity> &entity: *(entityController_->getAllEntities())) {
        moveEntity(entity.get());
    }
}

bool Board::moveEntity(Entity *target) {
    if (!target->move()) {
        return false;
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityMoved, target));
    if (!validateEntityPosition(target)) {
        eventQueue_->registerEvent(createCollisionEvent(target));
        return false;
    }
    return true;
}

bool Board::fireTank(Tank *target) {
    std::optional<std::unique_ptr<Bullet>> newBullet = target->createBullet();

    if (!newBullet.has_value()) {
        return false;
    }

    if (!validateEntityPosition(dynamic_cast<Entity *>(newBullet->get()))) {
        eventQueue_->registerEvent(createCollisionEvent(target));
    }

    Entity *addedEntity = entityController_->addEntity(std::move(newBullet.value()));
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EntitySpawned, addedEntity));
    return true;
}

bool Board::spawnTank(unsigned int x, unsigned int y, Tank::TankType type, Direction facing) {
    std::unique_ptr<Tank> newTank = entityController_->createTank(x, y, type, facing);

    Entity *spawnedTank = entityController_->addEntity(std::move(newTank));
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EntitySpawned, spawnedTank));

    if (!validateEntityPosition(spawnedTank)) {
        eventQueue_->registerEvent(createCollisionEvent(spawnedTank));
        return false;
    }
    return true;
}

bool Board::validateEntityPosition(Entity *target) {
    if (target->getX() < 0 || target->getY() < 0) {
        return false;
    }

    auto min_x = static_cast<unsigned int>(floorf(target->getX()));   // TODO USE DOUBLE
    auto max_x = static_cast<unsigned int>(ceilf(target->getX() + target->getSizeX() - 1));
    auto min_y = static_cast<unsigned int>(floorf(target->getY()));
    auto max_y = static_cast<unsigned int>(ceilf(target->getY() + target->getSizeY() - 1));

    try {
        for (unsigned int i = min_x; i <= max_x; i++)
            for (unsigned int j = min_y; j <= max_y; j++) {
                TileType tile = grid_->getTileAtPosition(i, j);
                if (tile != NullTile && TileManager::isTileCollidable(tile)) {
                    return false;
                }
            }
    } catch (OutOfGridException &) {
        return false;
    }

    if (entityController_->checkEntityCollisions(target)) {
        return false;
    }

    return true;
}

void Board::killAllEnemyEntities() {
    std::vector<std::unique_ptr<Entity>>* entityVector = entityController_->getAllEntities();
    for (auto iter = entityVector->rbegin(); iter != entityVector->rend(); iter++) {
        Entity* entity = iter->get();
        Tank *possiblyATank = dynamic_cast<Tank *>(entity);
        if (possiblyATank == nullptr) {  // bullet
            auto *possiblyABullet = dynamic_cast<Bullet *>(entity);
            if (possiblyABullet != nullptr && possiblyABullet->isFriendly()) {
                continue;
            }
            entityController_->removeEntity(entity);
            continue;

        } else if (possiblyATank->getType() == Tank::PlayerTank) {
            continue;
        }

        entityController_->killTank(possiblyATank);
    }
}

void Board::removeAllEntities() {
    entityController_->clear();
}

unsigned int Board::getSizeX() {
    return grid_->getSizeX();
}

unsigned int Board::getSizeY() {
    return grid_->getSizeY();
}

std::unique_ptr<Event> Board::createCollisionEvent(Entity *entity) {
    std::unique_ptr<Event> collisionEvent;
    std::optional<Entity *> collidingEntity = entityController_->findEntityAtPosition(entity->getX(), entity->getY(),
                                                                                      entity);
    if (collidingEntity.has_value()) {  // collision with entity
        collisionEvent = std::make_unique<Event>(Event::EntityEntityCollision, entity, collidingEntity.value());
    } else {
        collisionEvent = std::make_unique<Event>(Event::EntityGridCollision, entity, entity->getX(), entity->getY());
    }
    return std::move(collisionEvent);
}

void Board::loadLevel(unsigned int levelNum) {
    removeAllEntities();
    setGrid(std::move(GridBuilder::BuildLevel(levelNum)));  // TODO init player tank
}