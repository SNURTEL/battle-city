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

class Entity {
public:

    virtual ~Entity()=default;

    virtual bool move()=0;

    virtual bool moveBack()=0;

    [[nodiscard]] float getX() const;

    [[nodiscard]] float getY() const;

    [[nodiscard]] float getSpeed() const;

    [[nodiscard]] Direction getFacing() const;

    [[nodiscard]] float getSizeX() const;

    [[nodiscard]] float getSizeY() const;

protected:
    Entity()=default;

    float x_;
    float y_;
    float size_x_;
    float size_y_;
    float speed_;
    Direction facing_;

    Entity(float x, float y, float sizeX, float sizeY, float speed, Direction facing);

};


#endif //PROI_PROJEKT_ENTITY_H
