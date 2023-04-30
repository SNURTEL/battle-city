//
// Created by tomek on 28.05.2022.
//

#include "include/Entity.h"


float Entity::getX() const {
    return x_;
}

float Entity::getY() const {
    return y_;
}

float Entity::getSpeed() const {
    return speed_;
}

Direction Entity::getFacing() const {
    return facing_;
}

float Entity::getSizeX() const {
    return size_x_;
}

float Entity::getSizeY() const {
    return size_y_;
}

Entity::Entity(float x, float y, float sizeX, float sizeY, float speed, Direction facing) : x_(x), y_(y),
                                                                                            size_x_(sizeX),
                                                                                            size_y_(sizeY),
                                                                                            speed_(speed),
                                                                                            facing_(facing) {}
