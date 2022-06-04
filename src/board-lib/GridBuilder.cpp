//
// Created by tomek on 26.05.2022.
//

#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <unistd.h>

#include "include/GridBuilder.h"
#include "include/Grid.h"

std::unique_ptr<Grid> GridBuilder::buildLevel(unsigned int level) {
    static std::map<char, TileType> charToTileMapping {
        {'-', NullTile},
        {'B', Bricks},
        {'S', Steel},
        {'T', Trees},
        {'W', Water},
        {'E', NullTile}  // TODO add eagle
    };
    char cwd[80];
    getcwd(cwd, 80);
    std::string filename = "../bin/levels/lvl" + std::to_string(level) + ".txt";  // FIXME THIS **WILL** CAUSE ERRORS AND SHOULD BE FIXED AS SOON AS POSSIBLE
    std::ifstream file(filename);
    std::string line;

    auto newGrid = std::make_unique<Grid>();
    unsigned int maxWidth = newGrid->getSizeX();

    unsigned int lineIdx = 0;
    while (std::getline(file, line))
    {
        unsigned int lineLength = line.length();
        unsigned int endIndex =  std::min(maxWidth, lineLength);
        for(int i=0; i<endIndex; ++i){  // FIXME I am pretty sure this can be done with an iterator
            newGrid->grid[i][lineIdx] = charToTileMapping[line[i]];
            // i, lineIdx, charToTileMapping[line[i]]
        }
        lineIdx++;
    }


    return std::move(newGrid);
}