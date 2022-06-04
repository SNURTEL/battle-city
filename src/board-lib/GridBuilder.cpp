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
#include "../tank-lib/include/Tank.h"

std::unique_ptr<Grid> GridBuilder::buildLevel(unsigned int level) {
    static std::map<char, TileType> charToTileMapping{
            {'-', NullTile},
            {'B', Bricks},
            {'S', Steel},
            {'T', Trees},
            {'W', Water},
            {'E', NullTile},
            {'*', NullTile}
    };

    static std::map<char, Tank::TankType> charToTankMapping{
            {'B', Tank::BasicTank},
            {'F', Tank::FastTank},
            {'P', Tank::PowerTank},
            {'A', Tank::ArmorTank}

    };

    std::string filename = "./levels/lvl" + std::to_string(level) +
                           ".txt";  // FIXME THIS **WILL** CAUSE ERRORS AND SHOULD BE FIXED AS SOON AS POSSIBLE
    std::ifstream file(filename);
    std::string line;

    auto newGrid = std::make_unique<Grid>();
    unsigned int maxWidth = newGrid->getSizeX();

    if(std::getline(file, line)){
        for(char c: line){
            if(charToTankMapping.count(c) == 0){
                continue;
            }
            newGrid->tankTypes.push(charToTankMapping[c]);
        }
    }

    unsigned int lineIdx = 0;
    while (std::getline(file, line)) {
        unsigned int lineLength = line.length();
        unsigned int endIndex = std::min(maxWidth, lineLength);
        for (int i = 0; i < endIndex; ++i) {  // FIXME I am pretty sure this can be done with an iterator
            switch (line[i]) {
                case 'E': { // TODO add eagle
                    break;
                }
                case '*':{
                    newGrid->spawnpoints.emplace_back(std::make_pair(i, lineIdx));
                    break;
                }
                default:{
                }
            }
            if(charToTileMapping.count(line[i]) == 0){
                continue;
            }
            newGrid->grid[i][lineIdx] = charToTileMapping[line[i]];
            // i, lineIdx, charToTileMapping[line[i]]
        }
        lineIdx++;
    }


    return std::move(newGrid);
}