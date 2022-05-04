//
// Created by tomek on 03.05.2022.
//

#ifndef PROI_PROJEKT_TANK_H
#define PROI_PROJEKT_TANK_H

/**
 * Represents a direction in which a Tank (or a bullet) can be pointed at
 * Assume positive x is East, positive y in South
 */
enum Direction {
    North = 0,
    West,
    South,
    East
};

/**
 * \brief Base class for tank representation
 *
 * Tanks remember their position, orientation, number of lives,
 * and immutable fields which values are specified in derived classes (ex. points_, speed_).
 *
 * Tanks provide no input validation and should only be manipulated by TankController.
 */
class Tank {
public:
    virtual ~Tank()=default;

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
     * Move tank in x-axis and set adequate facing direction. Will not go below 0.
     * @param delta_x Distance to move
     */
    void moveX(int delta_x);

    /**
     * Move tank in y-axis and set adequate facing direction. Will not go below 0.
     * @param delta_t Distance to move
     */
    void moveY(int delta_y);

    /**
     * Modify the number of lives by delta_l. Will not go below 0.
     * @param delta_l
     */
    void deltaLives(int delta_l);

    /**
     * Returns tank type
     * @return
     */
    [[nodiscard]] TankType getType() const;

    /**
     * Returns tank's x coord
     * @return
     */
    [[nodiscard]] unsigned int getX() const;

    /**
     * Returns tank's y coord
     * @return
     */
    [[nodiscard]] unsigned int getY() const;

    /**
     * Return;s tank's remaining lives (0 is killed)
     * @return
     */
    [[nodiscard]] unsigned int getLives() const;

    /**
     * Returns tank's speed
     * @return
     */
    [[nodiscard]] double getTankSpeed() const;

    /**
     * Returns tank's bullet speed
     * @return
     */
    [[nodiscard]] double getBulletSpeed() const;

    /**
     * Returns the direction in which the tank is currently pointed
     * @return
     */
    [[nodiscard]] Direction getFacing() const;

    /**
     * Returns the number of points that the player gets for killing the tank
     * @return
     */
    [[nodiscard]] unsigned int getPoints() const;

protected:
    Tank()=default;

    TankType type_;

    unsigned int x_;
    unsigned int y_;
    unsigned int lives_;
    double tank_speed_;
    double bullet_speed_;
    Direction facing_;
    unsigned int points_;
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
    PlayerTank(unsigned int x, unsigned int y, unsigned int lives, Direction facing=North);
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
    BasicTank(unsigned int x, unsigned int y, Direction facing=North);
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
    FastTank(unsigned int x, unsigned int y, Direction facing=North);
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
    PowerTank(unsigned int x, unsigned int y, Direction facing=North);
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
    ArmorTank(unsigned int x, unsigned int y, Direction facing=North);
};

#endif //PROI_PROJEKT_TANK_H
