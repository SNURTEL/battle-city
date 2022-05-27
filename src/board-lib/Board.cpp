//
// Created by tomek on 24.05.2022.
//

#include <cmath>

#include "../core-lib/include/EventQueue.h"
#include "../core-lib/include/Event.h"

#include "include/Board.h"
#include "include/Grid.h"
#include "../tank-lib/include/TankController.h"

TankOverlapException::TankOverlapException(unsigned int x, unsigned int y) : std::exception() {
    what_message = "Tank at (" + std::to_string(x) + ", " + std::to_string(y) + ") overlaps with another object";
}

const char *TankOverlapException::what() {
    return what_message.c_str();
}

void Board::setTankMoving(Tank *target, bool isMoving) {
    tankController_->setTankMoving(target, isMoving);
}

void Board::setTankDirection(Tank *target, Direction direction) {
    snapTankToGrid(target);

    tankController_->setTankDirection(target, direction);
}

void Board::snapTankToGrid(Tank *target, bool snap_x, bool snap_y) {
    if (snap_x)
        target->setX(std::round(target->getX()));
    if (snap_y)
        target->setY(std::round(target->getY()));

    eventQueue_->registerEvent(std::make_unique<Event>(Event::TankMoved, target));
}

void Board::setGrid(std::unique_ptr<Grid> grid) {
    grid_ = std::move(grid);
}

void Board::moveAllTanks() {
    tankController_->moveAllTanks();
}

void Board::moveTank(Tank *target) {
    if (!validateTankMovement(target)) {
        if (target->getFacing() == North || target->getFacing() == South) {
            snapTankToGrid(target, false, true);   // WILL NOT WORK IF TANK'S SPEED IS GREATER THAN 0.5
        } else {
            snapTankToGrid(target, true, false);
        }
    } else {
        target->move();
    }
}

bool Board::spawnTank(Tank *target, unsigned int x, unsigned int y, Tank::TankType type) {
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            if ((tankController_->getTankAtPosition(static_cast<float>(x), static_cast<float>(y))).has_value()
                ||
                grid_->getTileAtPosition(x, y) != NullTile) {  // TODO ALSO CHECK BULLETS
                throw TankOverlapException(x, y);
            }
        }

    tankController_->spawnTank(x, y, type);
    return true;
}

bool Board::validateTankMovement(Tank *target) {
    float x_offset, y_offset = 0;
    switch (target->getFacing()) {
        case North: {
            y_offset = -target->getTankSpeed();
            break;
        };
        case East: {
            x_offset = target->getTankSpeed();
            break;
        };
        case South: {
            y_offset = target->getTankSpeed();
            break;
        };
        case West: {
            x_offset = -target->getTankSpeed();
            break;
        };
    }

    // check extra tiles adjusting for tank's 2x2 size
    auto secondary_x = static_cast<float>(target->getFacing() == North || target->getFacing() == South);
    float secondary_y = roundf(1 - secondary_x);


    if (target->getY() - target->getTankSpeed() < 0) {
        return false;
    }
    try {
        if (grid_->getTileAtPosition(static_cast<unsigned int>(target->getX() + x_offset),
                                     static_cast<unsigned int>(target->getY() + y_offset))
            != TileType::NullTile) {
            return false;
        }
        if (grid_->getTileAtPosition(static_cast<unsigned int>(target->getX() + x_offset + secondary_x),
                                     static_cast<unsigned int>(target->getY() + y_offset + secondary_y))
            != TileType::NullTile) {
            return false;
        }
    } catch (OutOfMapException &e) {
        return false;
    }

    return true;
}
