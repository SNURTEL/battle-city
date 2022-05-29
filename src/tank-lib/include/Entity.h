//
// Created by tomek on 28.05.2022.
//

#ifndef PROI_PROJEKT_ENTITY_H
#define PROI_PROJEKT_ENTITY_H

/**
 * Represents a direction in which an entity can be pointed at
 * Assume positive x is East, positive y in South
 */
enum Direction: unsigned int{
    North = 0,
    West,
    South,
    East
};

/**
 * Base class for creating objects representing in game
 * Entities are characterized by their position, size, the direction they're pointed at, and their movement speed.
 * Entities can be moved forwards and backwards (methods are required to be overloaded in derived classes)
 */
class Entity {
public:
    virtual ~Entity()=default;

    /**
     * Moves the entity forwards
     * @return Whether the entity actually moved
     */
    virtual bool move()=0;

    /**
     * Moves the entity backwards
     * @return Whether the entity actually moved
     */
    virtual bool moveBack()=0;

    /**
     * Returns entity's X coord
     * @return Entity's X coord
     */
    [[nodiscard]] float getX() const;

    /**
     * Returns entity's Y coord
     * @return Entity's Y coord
     */
    [[nodiscard]] float getY() const;

    /**
     * Returns entity's speed in distance per tick
     * @return Entity's speed
     */
    [[nodiscard]] float getSpeed() const;

    /**
     * Returns the direction in which the entity is faced
     * @return The direction in which the entity is faced
     */
    [[nodiscard]] Direction getFacing() const;

    /**
     * Returns entity's size in X axis
     * @return Entity's X axis size
     */
    [[nodiscard]] float getSizeX() const;

    /**
     * Returns entity's size in Y axis
     * @return Entity's Y axis size
     */
    [[nodiscard]] float getSizeY() const;

protected:
    Entity()=default;

    float x_;
    float y_;
    float size_x_;
    float size_y_;
    float speed_;
    Direction facing_;

    /**
     * Inits class Entity
     * @param x Entity's initial X coord
     * @param y Entity's initial Y coord
     * @param sizeX Entity's X axis size
     * @param sizeY Entity's Y axis size
     * @param speed Entity's speed in distance per tick
     * @param facing The direction in which the entity is pointed at
     */
    Entity(float x, float y, float sizeX, float sizeY, float speed, Direction facing);

};


#endif //PROI_PROJEKT_ENTITY_H
