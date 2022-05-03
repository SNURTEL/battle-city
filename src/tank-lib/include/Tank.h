//
// Created by tomek on 03.05.2022.
//

#ifndef PROI_PROJEKT_TANK_H
#define PROI_PROJEKT_TANK_H

enum Direction {
    North = 0,
    West,
    South,
    East
};

class Tank {
public:
    virtual ~Tank()=default;

    enum TankType{
        BasicTank,
        FastTank,
        PowerTank,
        ArmorTank
    };

    void moveX(int delta_x);

    void moveY(int delta_y);

    void deltaLives(int delta_l);

    [[nodiscard]] TankType getType() const;

    [[nodiscard]] unsigned int getX() const;

    [[nodiscard]] unsigned int getY() const;

    [[nodiscard]] unsigned int getLives() const;

    [[nodiscard]] double getTankSpeed() const;

    [[nodiscard]] double getBulletSpeed() const;

    [[nodiscard]] Direction getFacing() const;

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


class BasicTank: public Tank{
public:
    BasicTank()=delete;
    BasicTank(unsigned int x, unsigned int y, Direction facing=North);
};

class FastTank: public Tank{
public:
    FastTank()=delete;
    FastTank(unsigned int x, unsigned int y, Direction facing=North);
};

class PowerTank: public Tank{
public:
    PowerTank()=delete;
    PowerTank(unsigned int x, unsigned int y, Direction facing=North);
};

class ArmorTank: public Tank{
public:
    ArmorTank()=delete;
    ArmorTank(unsigned int x, unsigned int y, Direction facing=North);
};

#endif //PROI_PROJEKT_TANK_H
