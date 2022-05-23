//
// Created by tomek on 23.05.2022.
//

#ifndef PROI_PROJEKT_GRID_H
#define PROI_PROJEKT_GRID_H

#include <exception>

enum TileType: unsigned int{
    Bricks=0,
    Steel,
    Water,
    Trees,
    NullTile
};
class OutOfMapException : public std::exception {
    [[nodiscard]] const char * what () const noexcept override;
};

class Grid {
public:
    TileType getTileAtPosition(unsigned int x, unsigned int y);

    void setTile(unsigned int x, unsigned int y, TileType newTile);

private:
    TileType grid[52][52] = {TileType::NullTile};
};


#endif //PROI_PROJEKT_GRID_H
