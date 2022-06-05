//
// Created by tomek on 03.05.2022.
//

#ifndef PROI_PROJEKT_ENTITYCONTROLLER_H
#define PROI_PROJEKT_ENTITYCONTROLLER_H

#include <vector>
#include <memory>
#include <optional>

#include "Tank.h"

class Event;

template<class E>
class EventQueue;

/**
 * Exception thrown when trying to remove an entity that's not located in a controller
 */
class EntityDoesNotExistException : public std::exception {
public:
    const char *what();
};

/**
 * \brief Aggregates and manipulates Entity objects
 *
 * Not meant to be used as a standalone class - not all methods queue appropriate events; collision detection must be controlled externally
 */
class EntityController {
public:
    /**
     * Inits class EntityController
     */
    EntityController();

    /**
     * Creates a tank using given params and return a shared_ptr with the created tank
     * @param x Tank's initial X coord
     * @param y Tank's initial Y coord
     * @param type Tank's type
     * @param facing The direction in which the tank should be faced (defaults to North)
     * @return A new tank wrapped in a shared_ptr
     */
    std::shared_ptr<Tank> createTank(unsigned int x, unsigned int y, Tank::TankType type, Direction facing = North);

    /**
     * Deals damage to a tank (and kills it if health drops to 0)
     *
     * Queues Event::TankHit or Event::TankKilled
     * @param target Target tank
     * @param damage Damage value
     */
    void hitTank(std::shared_ptr<Tank> target, unsigned int damage = 1);

    /**
     * Kills a tank immediately. If killed the player, queues a different event type
     *
     * Queues Event::TankKilled or Event::PlayerKilled
     * @param tank Target tank
     */
    void killTank(std::shared_ptr<Tank> tank);

    /**
     * Removes and deletes an entity from pool without killing it
     *
     * Queues Event::EntityRemoved
     * @param entity
     */
    void removeEntity(std::shared_ptr<Entity> entity);

    /**
     * Attempt to move all entities on the board (tanks will move only if moving flag is set, bullets will move unconditionally)
     * Does not detect collisions
     *
     * Possibly queues multiple instances of Event::EntityMoved
     */
    void moveAllEntities();

    /**
     * Attempts to move an entity by it's speed per tick value.
     * Does not detect collisions
     *
     * Possibly queues Event::EntityMoved
     * @param target Target entity
     * @param direction Direction in which to move the entity
     */
    void moveEntity(std::shared_ptr<Entity> target);

    /**
     * Sets tank's moving_ flag to a given value
     * @param target Target tank
     * @param isMoving New flag value
     */
    void setTankMoving(const std::shared_ptr<Tank>& target, bool isMoving);

    /**
     * Sets the direction in which a tank is faced
     *
     * Does not queue any events, nor does it provide collision detection
     * @param target Target tank
     * @param direction New direction
     */
    void setTankDirection(const std::shared_ptr<Tank>& target, Direction direction);

    /**
     * Checks if a given point is located inside any entity's bounding box. If so, returns a pointer to the entity
     * If multiple entities are overlapping at the given point, will return the one which was added to the controller the earliest
     *
     * @param x X coord to check
     * @param y Y coord to check
     * @return If an entity was found, a pointer to the entity; if not, an std::nullopt
     */
    std::optional<std::shared_ptr<Entity>>
    findEntityAtPosition(float x, float y, std::optional<std::shared_ptr<Entity>> ignored = std::nullopt);

    /**
     * Returns a pointer to the player-controlled tank
     * @return A pointer to the player-controlled tank
     */
    std::shared_ptr<PlayerTank> getPlayer();

    /**
     * Adds a PlayerTank type entity to the controller and saves a reference to it in the player_ field.
     * Does not queue events
     * @param playerTank A new PlayerTank instance wrapped in a shared_ptr
     * @return A pointer to the new PlayerTank (old pointers are now invalid)
     */
    std::shared_ptr<PlayerTank> addEntity(const std::shared_ptr<PlayerTank>& playerTank);

    /**
     * Adds a Tank (but not PlayerTank) type entity to the controller.
     * Does not queue events
     * @param newTank A new Tank instance wrapped in a shared_ptr
     * @return A pointer to the new Tank (old pointers are now invalid)
     */
    std::shared_ptr<Tank> addEntity(std::shared_ptr<Tank> newTank);

    /**
     * Adds a Bullet type entity to the controller.
    * Does not queue events
    * @param newBullet A new Bullet instance wrapped in a shared_ptr
    * @return A pointer to the new Bullet (old pointers are now invalid)
    */
    std::shared_ptr<Bullet> addEntity(std::shared_ptr<Bullet> newBullet);

    /**
     * Adds a Entity (but not Tank or Bullet) type entity to the controller.
    * Does not queue events
    * @param newEntity A new Entity instance wrapped in a shared_ptr
    * @return A pointer to the new Entity (old pointers are now invalid)
    */
    std::shared_ptr<Entity> addEntity(std::shared_ptr<Entity> newEntity);

    /**
     * Checks if an entity overlaps with any other entity
     * Does not queue events
     * @param target An entity to check
     * @return Whether a given entity overlaps with any other entity
     */
    bool checkEntityCollisions(const std::shared_ptr<Entity>& target);

    /**
     * Provides access to entity vector
     * VECTOR SHOULD NOT BE MODIFIED OUTSIDE THE CONTROLLER CLASS
     *
     * @return A pointer to the internal vector containing Entity instances
     */
    std::vector<std::shared_ptr<Entity>> *getAllEntities();  //TODO const reference lol

    /**
     * Erases all entities from the controller
     *
     * Possibly queues multiple instances of Event::EntityRemoved
     */
    void clear();

protected:

    EventQueue<Event> *eventQueue_;
    std::vector<std::shared_ptr<Entity>> entities_;

    std::shared_ptr<PlayerTank> player_;
};


#endif //PROI_PROJEKT_ENTITYCONTROLLER_H
