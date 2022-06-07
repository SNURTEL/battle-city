//
// Created by tomek on 23.05.2022.
//

#include "include/Grid.h"
#include "../core-lib/include/EventQueue.h"
#include "../core-lib/include/Event.h"

const char *OutOfGridException::what() const noexcept {
    return "Given coords do not lie within map's boundaries";
}

Grid::Grid() {
    eventQueue_ = EventQueue<Event>::instance();
}

TileType Grid::getTileAtPosition(unsigned int x, unsigned int y) {
    if (x > 51 || y > 51) {
        throw OutOfGridException();
    }

    return grid[x][y];
}

void Grid::setTile(unsigned int x, unsigned int y, TileType newTile) {
    if (x > 51 || y > 51) {
        throw OutOfGridException();
    }

    if (newTile == NullTile) {
        deleteTile(x, y);
        return;
    }
    Event::EventType eventType;
    if (grid[x][y] == NullTile) {
        eventType = Event::TilePlaced;
    } else {
        eventType = Event::TileChanged;
    }

    grid[x][y] = newTile;
    eventQueue_->registerEvent(std::make_unique<Event>(eventType, x, y, this));

}

void Grid::deleteTile(unsigned int x, unsigned int y) {
    if (x > 51 || y > 51) {
        throw OutOfGridException();
    }

    if(grid[x][y]==NullTile){
        return;
    }

    grid[x][y] = NullTile;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::TileDeleted, x, y, this));

}

unsigned int Grid::getSizeX() const {
    return size_x;
}

unsigned int Grid::getSizeY() const {
    return size_y;
}

const std::vector<std::pair<unsigned int, unsigned int>> &Grid::getSpawnpoints() const {
    return enemySpawnpoints;
}

const std::queue<Tank::TankType> &Grid::getTankTypes() const {
    return tankTypes;
}

const std::pair<unsigned int, unsigned int> &Grid::getPlayerSpawnpoint() const {
    return playerSpawnpoint;
}

