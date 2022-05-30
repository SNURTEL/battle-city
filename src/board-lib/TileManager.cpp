//
// Created by tomek on 23.05.2022.
//

#include "include/TileManager.h"

#include "include/Grid.h"

bool TileManager::isTileCollidable(TileType tile) {
    return (tile == TileType::Bricks || tile == TileType::Steel);
}

bool TileManager::isTileDestructible(TileType tile) {
    return (tile == TileType::Bricks);
}

TileManager* TileManager::instance(){
    static std::unique_ptr<TileManager> self(new TileManager);
    return self.get();
};
