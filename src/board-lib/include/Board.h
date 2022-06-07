//
// Created by tomek on 24.05.2022.
//

#ifndef PROI_PROJEKT_BOARD_H
#define PROI_PROJEKT_BOARD_H

#include <memory>

#include "../../core-lib/include/EventQueue.h"
#include "../../tank-lib/include/Tank.h"
#include "../../tank-lib/include/EntityController.h"
#include "Grid.h"

class Event;

class Grid;
class EntityController;

enum Direction: unsigned int;

/**
 * \brief Aggregates and manipulates tank, bullet, and tile objects
 *
 * A mediator between Grid and EntityController classes. Provides access to the board and queues board-related events.
 */
class Board {
public:
    Board();

    /**
     * Sets tank's moving flag to a given value
     * @param target Target tank
     * @param isMoving New flag value
     */
    void setTankMoving(const std::shared_ptr<Tank>& target, bool isMoving);

    /**
     * Changes the direction in which the tank is faced to targetDirection
     * If currently faced in targetDirection, does nothing
     * If rotating by 90 or 270 degrees, attempts to snap to grid
     *
     * Possibly queues Event::TankRotated and Event::EntityMoved
     * @param tank Target tank
     * @param targetDirection New direction
     */
    void setTankDirection(const std::shared_ptr<Tank>& tank, Direction targetDirection);

    /**
     * Attempt to move all entities on the board (tanks will move only if moving flag is set)
     * Detects collisions and queues events if one happens (does not correct colliding entity's position)
     *
     * Possibly queues multiple instances of Event::EntityMoved and Event::EntityEntityCollision or Event::EntityGridCollision
     * */
    void moveAllEntities();

    /**
     * Attempts to move an entity by it's speed per tick value.
     * Detects collisions and queues events if one happens (does not correct colliding entity's position)
     *
     * Possibly queues Event::TankMoved and Event::EntityEntityCollision or Event::EntityGridCollision
     * @param target Target entity
     */
    bool moveEntity(const std::shared_ptr<Entity>& target);

    /**
     * Attempts to create a bullet using the given tanks and adds it to internal EntityController.
     * If the bullet collides with other entity or tile, queues an event (does not correct bullet's position)
     *
     * Possibly queues Event::EntitySpawned and Event::EntityEntityCollision or Event::EntityGridCollision
     * @param target A tank that should fire the bullet
     * @return Whether a collision was detected while spawning the bullet
     */
    bool fireTank(const std::shared_ptr<Tank>& target);

    /**
     * Attempts to spawn a tank at a given location.
     * Detects collisions and queues events if one happens (does not correct colliding tank's position)
     * If passed TankType::PlayerTank as param, spawnPlayer is called instead
     *
     * Possibly queues Event::EntitySpawned or Event::PlayerSpawned and Event::EntityEntityCollision or Event::EntityGridCollision
     * @param x New tank's X coord
     * @param y New tank's Y coord
     * @param type New tank's type
     * @param facing The direction in which the new tank will be faced (defaults to North)
     * @return Whether a collision was detected while spawning the tank
     */
    bool spawnTank(unsigned int x, unsigned int y, Tank::TankType type, Direction facing = North);

    /**
     * Spawns the player at a given location
     * Detects collisions and queues events if one happens (does not correct colliding tank's position)
     *
     * Queues Event::PlayerSpawned and possibly Event::EntityEntityCollision or Event::EntityGridCollision
     * @param x Player's initial x coord
     * @param y Player's initial y coord
     * @param facing The direction in which the player should be faced
     * @return True if no collisions detected, false if overlap found
     */
    bool spawnPlayer(unsigned int x, unsigned int y, Direction facing = North);

    /**
     * Sets board's grid to a given one. Loads enemy spawnpoints and types to BotController
     * This function should only be used after removing all entities from the board in order to prevent overlaps
     *
     * Does not queue events (yet...)
     * @param grid New Grid object wrapped in a unique_ptr
     */
    void setGrid(std::unique_ptr<Grid> grid);

    /**
     * Deletes a tile at the given coords. If tile was a NullTile or was not destructible, does nothing
     *
     * Possibly queues Event::TileDeleted
     * @param x Tile's X coord
     * @param y Tile's Y coord
     */
    void deleteTile(unsigned int x, unsigned int y);

    /**
     * Kills all enemy entities: tanks which are not PlayerTank and bullets which are not friendly
     *
     * Possibly queues multiple instances of Event::TankKilled and Event::EntityRemoved
     */
    void killAllEnemyEntities();

    /**
     * Removes an entity from the board
     *
     * Queues Event::EntityRemoved
     */
    void removeEntity(std::shared_ptr<Entity> entity);

    /**
     * Removes all entities from the board
     *
     * Possibly queues multiple instances of Event::EntityRemoved
     */
    void removeAllEntities();

    /**
     * Returns board's size in X axis
     * @return Board's X axis size
     */
    unsigned int getSizeX();

    /**
     * Returns board's size in Y axis
     * @return Board's Y axis size
     */
    unsigned int getSizeY();

    /**
     * Snaps the tank to the nearest integer coords
     *
     * Queues Event::TankMoved
     * @param target Target tank
     * @param snap_x Whether to snap in X axis (defaults to true)
     * @param snap_y Whether to snap in Y axis (defaults to true)
     */
    bool snapTankToGrid(const std::shared_ptr<Tank>& target, bool snap_x= true, bool snap_y= true);

    /**
     * Removes all entities from the board and loads a new grid from GridBuilder
     *
     * Possibly queues multiple instances of Event::EntityRemoved and a single instance of Event::LevelLoaded
     * @param levelNum
     */
    void loadLevel(unsigned int levelNum);

    /**
     * Returns pointer on the player tank object
     * @return Player Tank pointer
     */
    std::shared_ptr<PlayerTank> getPlayerTank();

    /**
     * Removes entity
     * @param entity pointer
     */
    void removeEntity(Entity* entity);

    /**
     * Returns entity controller pointer
     * @return entity controller pointer
     */
    EntityController* getEntityController();

    /**
     * Hits tank
     * @param target (Tank)
     * @param damage (int)
     */
    void hitTank(std::shared_ptr<Tank> target, unsigned int damage);

    /**
     * Returns grid pointer
     * @return grid pointer
     */
    Grid* getGrid();

protected:
    /**
     * Checks if an entity overlaps with any other entity, tile, or is placed out of grid
     *
     * Does not queue events.
     * @param target Entity to check
     * @return True if no collisions detected, false if position is invalid
     */
    bool validateEntityPosition(const std::shared_ptr<Entity>& target);

    /**
     * Builds an Event::Collision event for a given entity, should be called after detecting a collision
     * This function assumes the collision did happen. Calling the function when there was no collision will result in undefined behavior (usually creating some kind of Entity-Board collision event)
     * @param entity Entity to check
     * @return A collision event wrapped in a unique_ptr
     */
    std::unique_ptr<Event> createCollisionEvent(std::shared_ptr<Entity> entity);

    std::unique_ptr<Grid> grid_;
    std::unique_ptr<EntityController> entityController_;

    EventQueue<Event>* eventQueue_ = EventQueue<Event>::instance();

    BotController* botController;


};

#endif //PROI_PROJEKT_BOARD_H
