//
// Created by tomek on 23.05.2022.
//

#ifndef PROI_PROJEKT_TILEMANAGER_H
#define PROI_PROJEKT_TILEMANAGER_H
#include <memory>

enum TileType: unsigned int;

/**
 * Allows to access attrs of tiles represented by TileType.
 * The class does not provide a constructor and all of it's methods are static, making it a primitive singleton
 */
class TileManager {
public:
    TileManager(TileManager&) = delete;

    bool operator=(const TileManager&) = delete;

    /**
     * Checks if tiles of a given type can collide with tanks and bullets
     * @param tile
     * @return
     */
    static bool isTileCollidable(TileType tile);

    /**
     * Checks if tiles of a given type can be destroyed
     * @param tile
     * @return
     */
    static bool isTileDestructible(TileType tile);

    /**
     * Returns TileManger's sole instance
     * @return
     */
    static TileManager* instance();

private:
    TileManager()=default;
};


#endif //PROI_PROJEKT_TILEMANAGER_H
