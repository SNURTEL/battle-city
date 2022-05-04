//
// Created by tomek on 03.05.2022.
//

#include "include/Tank.h"

void Tank::moveX(int delta_x) {
    x_ += delta_x;
    if (delta_x > 0) {
        facing_ = East;
    } else if (delta_x < 0) {
        facing_ = West;
    }
}

void Tank::moveY(int delta_y) {
    y_ += delta_y;
    if (delta_y > 0) {
        facing_ = South;
    } else if (delta_y < 0) {
        facing_ = North;
    }
}

void Tank::deltaLives(int delta_l) {
    lives_ += delta_l;
}

unsigned int Tank::getX() const {
    return x_;
}

unsigned int Tank::getY() const {
    return y_;
}

unsigned int Tank::getLives() const {
    return lives_;
}

double Tank::getTankSpeed() const {
    return tank_speed_;
}

Direction Tank::getFacing() const {
    return facing_;
}

unsigned int Tank::getPoints() const {
    return points_;
}

double Tank::getBulletSpeed() const {
    return bullet_speed_;
}

Tank::TankType Tank::getType() const {
    return type_;
}

// ##############################

PlayerTank::PlayerTank(unsigned int x, unsigned int y, unsigned int lives, Direction facing) {
    type_=Tank::PlayerTank;

    x_ = x;
    y_ = y;
    facing_=facing;

    lives_=lives;
    tank_speed_=1;
    bullet_speed_=1;
    points_=100;
}

BasicTank::BasicTank(unsigned int x, unsigned int y, Direction facing) : Tank() {
    type_=Tank::BasicTank;

    x_ = x;
    y_ = y;
    facing_=facing;

    lives_=1;
    tank_speed_=1;
    bullet_speed_=1;
    points_=100;
}

FastTank::FastTank(unsigned int x, unsigned int y, Direction facing) : Tank() {
    type_=Tank::FastTank;

    x_ = x;
    y_ = y;
    facing_=facing;

    lives_=1;
    tank_speed_=3;
    bullet_speed_=2;
    points_=200;
}

PowerTank::PowerTank(unsigned int x, unsigned int y, Direction facing) : Tank() {
    type_=Tank::PowerTank;

    x_ = x;
    y_ = y;
    facing_=facing;

    lives_=1;
    tank_speed_=2;
    bullet_speed_=3;
    points_=300;
}

ArmorTank::ArmorTank(unsigned int x, unsigned int y, Direction facing) : Tank() {
    type_=Tank::ArmorTank;

    x_ = x;
    y_ = y;
    facing_=facing;

    lives_=4;
    tank_speed_=2;
    bullet_speed_=2;
    points_=400;
}