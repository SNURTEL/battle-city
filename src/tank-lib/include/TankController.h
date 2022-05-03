//
// Created by tomek on 03.05.2022.
//

#ifndef PROI_PROJEKT_TANKCONTROLLER_H
#define PROI_PROJEKT_TANKCONTROLLER_H

#include <vector>
#include <memory>
#include <optional>

#include "Tank.h"

class Event;

template<class E>
class EventQueue;

class TankOverlapException : public std::exception{
public:
    TankOverlapException()=delete;

    TankOverlapException(unsigned int x, unsigned int y);

    const char* what();

private:
    std::string what_message;
};

class TankDoesNotExistException : public std::exception{
public:
    const char* what();
};

class TankController {
public:
    TankController();

    void spawnTank(unsigned int x, unsigned int y, Tank::TankType type);

    void hitTank(Tank * target, unsigned int damage = 1);

    void killTank(Tank * target);

    void removeTank(Tank * target);

    void moveTank(Tank * target, Direction direction);

    // const?
    std::optional<Tank*> getTankAtPosition(unsigned int x, unsigned int y);

private:
    EventQueue<Event> *eventQueue_;
    std::vector<std::unique_ptr<Tank>> tanks_;
};


#endif //PROI_PROJEKT_TANKCONTROLLER_H
