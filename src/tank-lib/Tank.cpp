//
// Created by tomek on 03.05.2022.
//

#include "include/Tank.h"

//void Tank::moveX(int delta_x) {
//    if (delta_x > 0) {
//        facing_ = East;
//        x_ += delta_x;
//
//    } else if (delta_x < 0) {
//        facing_ = West;
//        if(static_cast<int>(x_) + delta_x < 0){
//            x_ = 0;
//        }else{
//            x_ += delta_x;
//        }
//    }
//}
//
//void Tank::moveY(int delta_y) {
//    if (delta_y > 0) {
//        facing_ = South;
//        y_ += delta_y;
//    } else if (delta_y < 0) {
//        facing_ = North;
//        if(static_cast<int>(y_) + delta_y < 0){
//            y_ = 0;
//        }else{
//            y_ += delta_y;
//        }//    }
//}

void Tank::setFacing(Direction direction) {
    facing_ = direction;
}

void Tank::move() {
    if(!isMoving()){
        return;
    }
    offsetInCurrentDirection(tank_speed_);
}

void Tank::deltaLives(int delta_l) {
    if (static_cast<int>(lives_) + delta_l < 0) {
        lives_ = 0;
    } else {
        lives_ += delta_l;
    }
}

float Tank::getX() const {
    return x_;
}

float Tank::getY() const {
    return y_;
}

unsigned int Tank::getLives() const {
    return lives_;
}

float Tank::getTankSpeed() const {
    return tank_speed_;
}

void Tank::setMoving(bool movingFlag) {
    moving_ = movingFlag;
}

bool Tank::isMoving() const {
    return moving_;
}

Direction Tank::getFacing() const {
    return facing_;
}

unsigned int Tank::getPoints() const {
    return points_;
}

float Tank::getBulletSpeed() const {
    return bullet_speed_;
}

Tank::TankType Tank::getType() const {
    return type_;
}

void Tank::setX(float x) {
    x_ = x;
}

void Tank::setY(float y) {
    y_ = y;
}

void Tank::offsetInCurrentDirection(float offset) {
    switch (facing_) {
        case North:
            y_ -= offset;
            break;
        case East:
            x_ += offset;
            break;
        case South:
            y_ += offset;
            break;
        case West:
            x_ -= offset;
            break;
    }
}

// ##############################

PlayerTank::PlayerTank(float x, float y, unsigned int lives, Direction facing) {
    type_ = Tank::PlayerTank;

    x_ = x;
    y_ = y;
    facing_ = facing;

    lives_ = lives;
    tank_speed_ = 1;
    bullet_speed_ = 1;
    points_ = 100;
}

BasicTank::BasicTank(float x, float y, Direction facing) : Tank() {
    type_ = Tank::BasicTank;

    x_ = x;
    y_ = y;
    facing_ = facing;

    lives_ = 1;
    tank_speed_ = 1;
    bullet_speed_ = 1;
    points_ = 100;
}

FastTank::FastTank(float x, float y, Direction facing) : Tank() {
    type_ = Tank::FastTank;

    x_ = x;
    y_ = y;
    facing_ = facing;

    lives_ = 1;
    tank_speed_ = 3;
    bullet_speed_ = 2;
    points_ = 200;
}

PowerTank::PowerTank(float x, float y, Direction facing) : Tank() {
    type_ = Tank::PowerTank;

    x_ = x;
    y_ = y;
    facing_ = facing;

    lives_ = 1;
    tank_speed_ = 2;
    bullet_speed_ = 3;
    points_ = 300;
}

ArmorTank::ArmorTank(float x, float y, Direction facing) : Tank() {
    type_ = Tank::ArmorTank;

    x_ = x;
    y_ = y;
    facing_ = facing;

    lives_ = 4;
    tank_speed_ = 2;
    bullet_speed_ = 2;
    points_ = 400;
}