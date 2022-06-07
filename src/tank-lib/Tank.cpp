//
// Created by tomek on 03.05.2022.
//

#include "../core-lib/include/SimpleSubscriber.h"
#include "../core-lib/include/SimplePublisher.h"

#include "include/Tank.h"
#include "include/Bullet.h"


void Tank::setFacing(Direction direction) {
    facing_ = direction;
}

bool Tank::move() {
    if (!isMoving()) {
        return false;
    }
    offsetInCurrentDirection(speed_);
    return true;
}

bool Tank::moveBack() {
    if (!isMoving()) {
        return false;
    }
    offsetInCurrentDirection(-speed_);
    return true;
}

void Tank::deltaLives(int delta_l) {
    if (static_cast<int>(lives_) + delta_l < 0) {
        lives_ = 0;
    } else {
        lives_ += delta_l;
    }
}

unsigned int Tank::getLives() const {
    return lives_;
}


void Tank::setMoving(bool movingFlag) {
    moving_ = movingFlag;
}

bool Tank::isMoving() const {
    return moving_;
}

unsigned int Tank::getPoints() const {
    return points_;
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

std::optional<std::shared_ptr<Bullet>> Tank::createBullet() {
    if (!subscribedSubjects_.empty()) {
        return std::nullopt;
    }

    float bulletSizeX = 0.4;
    float bulletSizeY = 0.4;

    auto bulletType = static_cast<Bullet::BulletType>(type_ == TankType::PlayerTank);

    std::shared_ptr<Bullet> bullet;

    switch (facing_) {
        case North: {
            bullet = std::make_shared<Bullet>(x_ + (size_x_ - bulletSizeX) / 2, y_ - bulletSizeY, facing_, bulletSpeed_,
                                              bulletType);
            break;
        }
        case East: {
            bullet = std::make_shared<Bullet>(x_ + size_x_, y_ + (size_y_ - bulletSizeY) / 2, facing_, bulletSpeed_,
                                              bulletType);
            break;
        }
        case South: {
            bullet = std::make_shared<Bullet>(x_ + (size_x_ - bulletSizeX) / 2, y_ + size_y_, facing_, bulletSpeed_,
                                              bulletType);
            break;
        }
        case West: {
            bullet = std::make_shared<Bullet>(x_ - bulletSizeX, y_ + (size_y_ - bulletSizeY) / 2, facing_, bulletSpeed_,
                                              bulletType);
            break;
        }
    }

    subscribe(bullet.get());

    return std::move(bullet);
}

void Tank::notify(SimplePublisher *pub) {}

Tank::Tank(TankType type, float x, float y, float speed, float bulletSpeed, unsigned int lives, Direction direction,
           unsigned int points)
        : Entity(x, y, 4, 4, speed, direction),
          bulletSpeed_(bulletSpeed), lives_(lives), type_(type), points_(points), moving_(false) {}

// ##############################

PlayerTank::PlayerTank(float x, float y, Direction facing) : Entity(x, y, 4, 4, 0.2, facing),
                                                             Tank(Tank::PlayerTank, x, y, 0.2, 0.4, 999, facing, 0) {}

BasicTank::BasicTank(float x, float y, Direction facing) : Entity(x, y, 4, 4, 0.1, facing),
                                                           Tank(Tank::BasicTank, x, y, 0.1, 0.3, 1, facing, 100),
                                                           Bot(x, y, 4, 4, 0.1, facing) {}

FastTank::FastTank(float x, float y, Direction facing) : Entity(x, y, 4, 4, 0.3, facing),
                                                         Tank(Tank::FastTank, x, y, 0.3, 0.5, 1, facing, 200),
                                                         Bot(x, y, 4, 4, 0.3, facing) {}

PowerTank::PowerTank(float x, float y, Direction facing) : Entity(x, y, 4, 4, 0.2, facing),
                                                           Tank(Tank::PowerTank, x, y, 0.2, 0.7, 1, facing, 300),
                                                           Bot(x, y, 4, 4, 0.2, facing) {}

ArmorTank::ArmorTank(float x, float y, Direction facing) : Entity(x, y, 4, 4, 0.2, facing),
                                                           Tank(Tank::ArmorTank, x, y, 0.2, 0.5, 4, facing, 400),
                                                           Bot(x, y, 4, 4, 0.2, facing) {}
