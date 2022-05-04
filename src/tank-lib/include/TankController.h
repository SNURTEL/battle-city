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
 * Exception thrown when two tanks are overlapping as a result of an error
 */
class TankOverlapException : public std::exception{
public:
    TankOverlapException()=delete;

    /**
     * Inits class TankOverlapException
     * @param x First tank's x coord
     * @param y First tank's y coord
     */
    TankOverlapException(unsigned int x, unsigned int y);

    const char* what();

private:
    std::string what_message;
};

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
     * Event::TankSpawned is added to event queue
     * @param x Tank's initial x coord
     * @param y Tank's initial y coord
     * @param type Tank's type
     */
    void spawnTank(unsigned int x, unsigned int y, Tank::TankType type);

    /**
     * Deals damage to a tank (and kills it if health drops to 0)
     * Event::TankHit (or Event::TankKilled) is added to event queue
     * @param target Target tank
     * @param damage Damage value
     */
    void hitTank(Tank * target, unsigned int damage = 1);

    /**
     * Kills a tank immediately
     * Event::TankKilled is added to event queue
     * @param target Target tank
     */
    void killTank(Tank * target);

    /**
     * Removes and deletes a tank from pool without killing it
     * Event::TankRemoved is added to event queue
     * @param target
     */
    void removeTank(Tank * target);

    /**
     * Attempts to move a tank by it's speed per tick value. Stops at obstacles
     * Event::TankMoved is added to event queue
     * @param target Target tank
     * @param direction Direction in which to move the tank
     */
    void moveTank(Tank * target, Direction direction);

    // const?
    //assumes tanks are 13x13 in size
    /**
     * Checks if a given point is located inside any tank's bounding box. If so, returns it
     * @param x
     * @param y
     * @return
     */
    std::optional<Tank*> getTankAtPosition(unsigned int x, unsigned int y);

protected:
    EventQueue<Event> *eventQueue_;
    std::vector<std::unique_ptr<Tank>> tanks_;
};


#endif //PROI_PROJEKT_TANKCONTROLLER_H
