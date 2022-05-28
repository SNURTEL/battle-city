//
// Created by tomek on 24.05.2022.
//

#include <cmath>

#include "../core-lib/include/EventQueue.h"
#include "../core-lib/include/Event.h"

#include "include/Board.h"
#include "include/Grid.h"
#include "../tank-lib/include/EntityController.h"

#include "../tank-lib/include/Tank.h"
#include "../tank-lib/include/Bullet.h"


void Board::setTankMoving(Tank *target, bool isMoving) {
    entityController_->setTankMoving(target, isMoving);
}

void Board::setTankDirection(Tank *target, Direction direction) {
    snapTankToGrid(target, true, true);

    entityController_->setTankDirection(target, direction);
    eventQueue_->registerEvent(std::make_unique<Event>(Event::TankRotated, target));

}

void Board::snapTankToGrid(Tank *target, bool snap_x, bool snap_y) {
    if (snap_x)
        target->setX(std::round(target->getX()));
    if (snap_y)
        target->setY(std::round(target->getY()));

    eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityMoved, target));
}

void Board::setGrid(std::unique_ptr<Grid> grid) {
    grid_ = std::move(grid);
}

void Board::moveAllEntities() {
    for(std::unique_ptr<Entity> &entity: *(entityController_->getAllEntities())){
        moveEntity(entity.get());
    }
    moveEntity(entityController_->getPlayer());
}

bool Board::moveEntity(Tank *target) {
    target->move();
    if(!validateEntityPosition(target)){
        snapTankToGrid(target);

        std::optional<Entity*> collidingEntity = entityController_->getEntityAtPosition(target->getX(), target->getY());
        if(collidingEntity.has_value()){  // collision with entity
            eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityEntityCollision, dynamic_cast<Entity*>(target), collidingEntity.value()));
        }else{
            eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityTileCollision, dynamic_cast<Entity*>(target), target->getX(), target->getY()));
        }

        return false;
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityMoved, target));
    return true;
}

bool Board::moveEntity(Entity *target) {
    target->move();
    if(!validateEntityPosition(target)){

        std::optional<Entity*> collidingEntity = entityController_->getEntityAtPosition(target->getX(), target->getY());
        if(collidingEntity.has_value()){  // collision with entity
            eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityEntityCollision, target, collidingEntity.value()));
        }else{
            eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityTileCollision, target, target->getX(), target->getY()));
        }
        return false;
    }
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EntityMoved, target));
    return true;
}

bool Board::fireTank(Tank *target) {
    std::optional<std::unique_ptr<Bullet>> newBullet = target->createBullet();

    if(!newBullet.has_value()){
        return false;
    }

    if(!validateEntityPosition(dynamic_cast<Entity*>(newBullet->get()))){
        return false;
    }

    Entity* addedEntity = entityController_->addEntity(std::move(newBullet.value()));
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EntitySpawned, addedEntity));
}

bool Board::spawnTank(Tank *target, unsigned int x, unsigned int y, Tank::TankType type) {  // FIXME DUMB VALIDATION
    std::unique_ptr<Tank> newTank = entityController_->createTank(x, y, type);

    if(!validateEntityPosition(newTank.get())){
        return false;
    }

    Entity* spawnedTank = entityController_->addEntity(std::move(newTank));
    eventQueue_->registerEvent(std::make_unique<Event>(Event::EntitySpawned, spawnedTank));
}

bool Board::validateEntityPosition(Entity *target) {
    if(target->getX() < 0 || target->getY() < 0){
        return false;
    }

    auto min_x = static_cast<unsigned int>(floorf(target->getX()));   // TODO USE DOUBLE
    auto max_x = static_cast<unsigned int>(ceilf(target->getX()+target->getSizeX()));
    auto min_y = static_cast<unsigned int>(floorf(target->getX()));
    auto max_y = static_cast<unsigned int>(ceilf(target->getX()+target->getSizeX()));

    try{
        for(unsigned int i = min_x; i<max_x; i++)
        for(unsigned int j = min_y; j<max_y; j++){
            if(grid_->getTileAtPosition(i, j) != NullTile){
                return false;
            }
        }
    }catch(OutOfGridException&){
        return false;
    }

    if(entityController_->checkEntityCollisions(target)){
        return false;
    }

    return true;
}

void Board::killAllEnemyTanks() {
    for(std::unique_ptr<Entity>& entity: *(entityController_->getAllEntities())){
        Tank* possiblyATank = dynamic_cast<Tank*>(entity.get());
        if(possiblyATank == nullptr){
            continue;
        }
        if(possiblyATank->getType() == Tank::PlayerTank){
            continue;
        }
        entityController_->killTank(possiblyATank);
    }
}

void Board::removeAllEntities() {
    entityController_->clear();
}