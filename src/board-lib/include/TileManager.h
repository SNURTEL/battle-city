//
// Created by tomek on 23.05.2022.
//

#ifndef PROI_PROJEKT_TILEMANAGER_H
#define PROI_PROJEKT_TILEMANAGER_H
#include <memory>

enum TileType: unsigned int;

class TileManager {
public:
    TileManager(TileManager&) = delete;

    bool operator=(const TileManager&) = delete;

    static bool isTileCollidable(TileType tile);

    static bool isTileDestructible(TileType tile);

    static TileManager* instance();

private:
    TileManager()=default;
};


#endif //PROI_PROJEKT_TILEMANAGER_H
