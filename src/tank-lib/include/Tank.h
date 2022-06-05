//
// Created by tomek on 03.05.2022.
//

#ifndef PROI_PROJEKT_TANK_H
#define PROI_PROJEKT_TANK_H

#include <memory>
#include <optional>

#include "../../core-lib/include/SimpleSubscriber.h"
#include "Entity.h"

class SimplePublisher;

class Bullet;

/**
 * \brief Base class for tank representation
 *
 * Tanks are derived from Entity and have some additional tank-specific fields
 * Additionally, tanks inherit from SimpleSubscriber and can subscribe to Bullet instances fired by them (to disable firing when a bullet was already fired)
 *
 * Tanks provide no input validation whatsoever and should only be manipulated by EntityController.
 */
class Tank: public Entity, public SimpleSubscriber{
public:
    /**
     * Represents tank types, should match names of derived classes
     */
    enum TankType{  // is this even necessary?
        PlayerTank,
        BasicTank,
        FastTank,
        PowerTank,
        ArmorTank
    };

    /**
     * Changes the direction in which the tank is faced
     * @param direction Target direction
     */
    void setFacing(Direction direction);

    /**
     * Moves the tank by it's tick movement distance (speed) if the moving_ flag is set; otherwise, returns immediately
     * @return Whether the tank was moved or not
     */
    bool move() override;

    /**
     * Undoes the move() operation; moving_ flag is still required to be set
     * @return Whether the tank was moved or not
     */
    bool moveBack() override;

    /**
     * Modify the number of lives by delta_l. Will not go below 0.
     * @param delta_l Lives number modifier
     */
    void deltaLives(int delta_l);

    /**
     * Returns tank type
     * @return Tank::TankType value representing tank's type
     */
    [[nodiscard]] TankType getType() const;


    /**
     * Returns tank's remaining lives (0 is killed)
     * @return
     */
    [[nodiscard]] unsigned int getLives() const;

    /**
     * Sets tank's moving_ flag
     * @param isMoving New flag value
     */
    void setMoving(bool isMoving);

    /**
     * Checks whether the tank is moving or not
     * @return True is moving_ flag is set
     */
    [[nodiscard]] bool isMoving() const;

    /**
     * Returns the number of points that the player gets for killing the tank
     * @return Point reward value for killing the tank
     */
    [[nodiscard]] unsigned int getPoints() const;

    /**
     * Sets tank's X coord
     * @param x New X coord value
     */
    void setX(float x);

    /**
     * Sets tank's Y coord
     * @param y New Y coord value
     */
    void setY(float y);

    /**
     * Moves the tank by a given distance in direction in which it is faced
     * @param offset Offset value
     */
    void offsetInCurrentDirection(float offset);

    /**
     * Creates a bullet located right in front of the tank and faced in the same direction as the tank.
     * Bullet will not be created if a Bullet instance created by the tank already exists
     * @return A Bullet instance wrapped in a shared_ptr if the bullet was created; otherwise, an std::nullopt
     */
    std::optional<std::shared_ptr<Bullet>> createBullet();

    /**
     * Does nothing. All bullet - tank communication is done just with subscribing and unsubscribing.
     * @param pub
     */
    void notify(SimplePublisher* pub) override;

protected:
    /**
     * Inits class Tank
     * @param type Tank's type
     * @param x Tank's initial X coord
     * @param y Tank's initial Y coord
     * @param speed Tank's speed
     * @param bulletSpeed Tank's bullet's speed
     * @param lives Tank's lives
     * @param direction The direction in which the tank should be faced
     * @param points Point reward value for killing the tank
     */
    Tank(TankType type, float x, float y, float speed, float bulletSpeed, unsigned int lives, Direction direction, unsigned int points);

    TankType type_;

    unsigned int lives_;
    bool moving_;
    unsigned int points_;

    float bulletSpeed_;
};

/**
 * Represents a tank controlled by the player
 */
class PlayerTank: public Tank{
public:
    PlayerTank()=delete;
    /**
     * Init class PlayerTank
     * @param x Initial x coord
     * @param y Initial y coord
     * @param lives Initial number of lives
     * @param facing Initial direction in which the tank is faced (default=North)
     */
    PlayerTank(float x, float y, Direction facing=North);
};

/**
 * Represents a tank worth 100 points
 */
class BasicTank: public Tank{
public:
    BasicTank()=delete;
    /**
    * Init class BasicTank
    * @param x Initial x coord
    * @param y Initial y coord
    * @param facing Initial direction in which the tank is faced (default=North)
    */
    BasicTank(float x, float y, Direction facing=North);
};

/**
 * Represents a tank worth 200 points
 */
class FastTank: public Tank{
public:
    FastTank()=delete;
    /**
    * Init class FastTank
    * @param x Initial x coord
    * @param y Initial y coord
    * @param facing Initial direction in which the tank is faced (default=North)
    */
    FastTank(float x, float y, Direction facing=North);
};

/**
 * Represents a tank worth 300 points
 */
class PowerTank: public Tank{
public:
    PowerTank()=delete;
    /**
    * Init class PowerTank
    * @param x Initial x coord
    * @param y Initial y coord
    * @param facing Initial direction in which the tank is faced (default=North)
    */
    PowerTank(float x, float y, Direction facing=North);
};

/**
 * Represents a tank worth 400 points
 */
class ArmorTank: public Tank{
public:
    ArmorTank()=delete;
    /**
    * Init class ArmorTank
    * @param x Initial x coord
    * @param y Initial y coord
    * @param facing Initial direction in which the tank is faced (default=North)
    */
    ArmorTank(float x, float y, Direction facing=North);
};

#endif //PROI_PROJEKT_TANK_H
