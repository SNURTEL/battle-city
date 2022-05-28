//
// Created by tomek on 28.05.2022.
//

#ifndef PROI_PROJEKT_BULLET_H
#define PROI_PROJEKT_BULLET_H

#include "../../core-lib/include/SimplePublisher.h"

#include "Entity.h"

class Bullet : public Entity, public SimplePublisher {
public:
    enum BulletType : unsigned int {
        Enemy=0,
        Friendly=1
    };

    Bullet() = delete;

    Bullet(float x, float y, Direction direction, float speed, BulletType type);

    bool move() override;

    bool moveBack() override;

    bool isFriendly();

protected:
    void offsetInCurrentDirection(float offset);

    BulletType type_;
};


#endif //PROI_PROJEKT_BULLET_H
