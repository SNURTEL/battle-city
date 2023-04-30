//
// Created by tomek on 26.05.2022.
//

#ifndef PROI_PROJEKT_GRIDBUILDER_H
#define PROI_PROJEKT_GRIDBUILDER_H

#include <memory>

class Grid;

enum TileType: unsigned int;

/**
 * Used for building an initial grid for different levels. Class does not provide a constructor and all of it's methods are static, making it a primitive singleton
 */
class GridBuilder {
public:
    /**
     * Loads a level from file ./levels/lvl<N>.txt, where N is level number. First line in file is reserved for
     * for enemy tank type sequence, coded as shown:
     *  - 'B' - basic tank
     *  - 'F' - fast tank
     *  - 'P' - power tank
     *  - 'A' - armor tank
     * The rest of the file represents a 52x52 grid
     * Layout code:
     *  - 'B' - bricks
     *  - 'S' - steel
     *  - 'T' - trees
     *  - 'W' - water
     *  - 'E' - eagle location
     *  - '*' - enemy spawn point
     *  - '+' - player spawn point
     *  - any other char - NullTile
     *
     *
     * @param level The level to be loaded. ./levels/lvl<level>.txt will be checked
     * @return A grid constructed with the file. If file was not loaded, the grid will be filled with NullTiles
     */
    static std::unique_ptr<Grid> buildLevel(unsigned int level);

private:
    /**
     * Supporting function for BuildLevel.
     * Places a chunk of tiles of a given type on a grid.
     * New tiles are placed in a rectangle starting at (x1, y1) and ending at (x2, y2) (upper left corner of the grid is considered (0,0))
     * @param targetGrid Output argument!
     * @param type
     * @param x1
     * @param x2
     * @param y1
     * @param y2
     */
    [[deprecated]] static void placeChunk(Grid *targetGrid, TileType type, unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2) {};
    GridBuilder()=default;
};


#endif //PROI_PROJEKT_GRIDBUILDER_H
