//
// Created by tomek on 03.05.2022.
//

#ifndef PROI_PROJEKT_TANKCONTROLLER_H
#define PROI_PROJEKT_TANKCONTROLLER_H

#include <vector>
#include <memory>
#include <optional>

#include "Tank.h"

class Event;

template<class E>
class EventQueue;

/**
 * Exception thrown when trying to access a non-existent tank
 */
class TankDoesNotExistException : public std::exception{
public:
    const char* what();
};

/**
 * Aggregates and manipulates Tank objects
 */
class TankController {
public:
    /**
     * Inits class TankController
     */
    TankController();

    /**
     * Attempts to spawn a tank at a given location.
     *
     * Queues Event::TankSpawned
     * @param x Tank's initial x coord
     * @param y Tank's initial y coord
     * @param type Tank's type
     */
    Tank * spawnTank(unsigned int x, unsigned int y, Tank::TankType type);

    /**
     * Deals damage to a tank (and kills it if health drops to 0)
     *
     * Queues Event::TankHit
     * @param target Target tank
     * @param damage Damage value
     */
    void hitTank(Tank * target, unsigned int damage = 1);

    /**
     * Kills a tank immediately
     *
     * Queues Event::TankKilled
     * @param target Target tank
     */
    void killTank(Tank * target);

    /**
     * Removes and deletes a tank from pool without killing it
     *
     * Queues Event::TankRemoved
     * @param target
     */
    void removeTank(Tank * target);

    /**
     * Attempt to move all tanks on the board (will move only if moving flag is set)
     *
     * Possibly queues multiple instances of Event::TankMoved
     */
    void moveAllTanks();

    /**
     * Attempts to move a tank by it's speed per tick value. Stops at obstacles
     *
     * Possibly queues Event::TankMoved
     * @param target Target tank
     * @param direction Direction in which to move the tank
     */
    void moveTank(Tank *target);

    /**
     * Sets tank's moving_ flag to a given value
     * @param target
     * @param isMoving
     */
    void setTankMoving(Tank* target, bool isMoving);

    /**
     * Sets the direction in which the tank is faced
     * @param target
     * @param direction
     */
    void setTankDirection(Tank* target, Direction direction);

    //assumes tanks are 2x2 in size
    /**
     * Checks if a given point is located inside any tank's bounding box. If so, returns it
     *
     * Queues Event::TankRotated
     * @param x
     * @param y
     * @return
     */
    std::optional<Tank*> getTankAtPosition(float x, float y);

    /**
     * Returns a pointer to the player-controlled tank
     * @return
     */
    PlayerTank* getPlayer();

protected:
    EventQueue<Event> *eventQueue_;
    std::vector<std::unique_ptr<Tank>> tanks_;
    std::unique_ptr<PlayerTank> player_;
};


#endif //PROI_PROJEKT_TANKCONTROLLER_H
