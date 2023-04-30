//
// Created by tomek on 28.05.2022.
//

#include "../core-lib/include/SimpleSubscriber.h"
#include "../core-lib/include/SimplePublisher.h"

#include "include/Bullet.h"

Bullet::Bullet(float x, float y, Direction direction, float speed, BulletType type) : Entity(x, y, 0.4, 0.4, speed, direction), type_(type) {}

bool Bullet::move() {
    offsetInCurrentDirection(speed_);
    return true;
};

bool Bullet::moveBack() {
    offsetInCurrentDirection(-speed_);
};

void Bullet::offsetInCurrentDirection(float offset) {
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

bool Bullet::isFriendly() {
    return static_cast<bool>(type_);
}

void Bullet::unlink() {
    if(subscribers_.empty()){
        return;
    }
    subscribers_.front()->unsubscribe(this);
}