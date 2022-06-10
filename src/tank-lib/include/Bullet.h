//
// Created by tomek on 28.05.2022.
//

#ifndef PROI_PROJEKT_BULLET_H
#define PROI_PROJEKT_BULLET_H

#include "../../core-lib/include/SimplePublisher.h"

#include "Entity.h"

/**
 * An Entity derived class representing a tank bullet
 * In addition to Entity's attrs, Bullets can be characterized by their friendliness.
 * Bullet's are SimplePublishers and can be subscribed to by Tank objects.
 */
class Bullet : public Entity, public SimplePublisher {
public:
    /**
     * Enum type indicating whether a bullet was fired by the player or not
     */
    enum BulletType : unsigned int {
        Enemy=0,
        Friendly=1
    };

    Bullet() = delete;

    /**
     * Inits class bullet
     * @param x Bullet's initial X coord
     * @param y Bullet's initial Y coord
     * @param direction The direction in which the bullet is pointed at
     * @param speed Bullet's speed in distance per tick
     * @param type Bullets friendliness
     */
    Bullet(float x, float y, Direction direction, float speed, BulletType type);

    /**
     * Moves the bullet forwards unconditionally
     * @return Always true
     */
    bool move() override;

    /**
     * Moves the bullet backwards unconditionally
     * @return Always true
     */
    bool moveBack() override;

    /**
     * Returns whether the bullet was fired by the player
     * @return Bullet's friendliness
     */
    bool isFriendly();

    /**
     * Explicitly unsubscribes the bullet from the  tank
     */
    void unlink();

protected:
    /**
     * Offsets the bullet in the direction it is faced
     * @param offset Offset value
     */
    void offsetInCurrentDirection(float offset);

    BulletType type_;
};


#endif //PROI_PROJEKT_BULLET_H
