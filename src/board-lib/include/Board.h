//
// Created by tomek on 24.05.2022.
//

#ifndef PROI_PROJEKT_BOARD_H
#define PROI_PROJEKT_BOARD_H

#include <execution>
#include <memory>

#include "../../core-lib/include/EventQueue.h"
#include "../../tank-lib/include/Tank.h"
class Event;


class Grid;
class EntityController;


enum Direction: unsigned int;

// TODO CONSTRUCTOR WITH MEMBER CONTROLLERS
class Board {
public:
    /**
     * Sets tank's moving flag to a given value
     * @param target
     * @param isMoving
     */
    void setTankMoving(Tank* target, bool isMoving);

    /**
     * Changes the direction in which the tank is faced
     *
     * Queues Event::TankRotated
     * @param target
     * @param direction
     */
    void setTankDirection(Tank* target, Direction direction);

    /**
     * Attempt to move all tanks on the board (will move only if moving flag is set)
     *
     * Possibly queues multiple instances of Event::TankMoved
     * */
    void moveAllEntities();

    bool moveEntity(Tank *target);

    /**
     * Attempts to move a tank by it's speed per tick value. Stops at obstacles
     *
     * Possibly queues Event::TankMoved
     * @param target
     */
    bool moveEntity(Entity *target);


    bool fireTank(Tank* target);

    /**
     * Attempts to spawn a tank at a given location. Fails if spawned tank would overlap with another entity
     *
     * Possibly queues Event::TankSpawned
     *
     * @param target
     * @param x
     * @param y
     * @param type
     * @return True if successful, false if spawned tank would overlap with another entity
     */
    bool spawnTank(Tank* target, unsigned int x, unsigned int y, Tank::TankType type);

    /**
     * Sets board's grid to a given one
     * This function should only be used after removing all entities from the board in order to prevent overlaps
     * @param grid
     */
    void setGrid(std::unique_ptr<Grid> grid);

    void killAllEnemyTanks();

    void removeAllEntities();

private:
    /**
     * Snaps the tank to the nearest integer coords
     *
     * Queues Event::TankMoved
     * @param target
     * @param snap_x
     * @param snap_y
     */
    void snapTankToGrid(Tank* target, bool snap_x=true, bool snap_y=true);

//    /**
//     * Checks if a tank can move in the direction it is faced without collisions
//     * @param target
//     * @return
//     */
//    bool validateTankMovement(Tank* target);

    bool validateEntityPosition(Entity* target);

    std::unique_ptr<Grid> grid_;
    EntityController* entityController_;

    EventQueue<Event>* eventQueue_ = EventQueue<Event>::instance();

};


#endif //PROI_PROJEKT_BOARD_H
