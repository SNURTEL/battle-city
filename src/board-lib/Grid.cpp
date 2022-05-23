//
// Created by tomek on 23.05.2022.
//

#include "include/Grid.h"

const char *OutOfMapException::what() const noexcept {
    return "Given coords do not lie within map's boundaries";
}

TileType Grid::getTileAtPosition(unsigned int x, unsigned int y) {
    if(x>51 || y>51){
        throw OutOfMapException();
    }

    return grid[x][y];
}

void Grid::setTile(unsigned int x, unsigned int y, TileType newTile) {
    if(x>51 || y>51){
        throw OutOfMapException();
    }

    grid[x][y] = newTile;
}