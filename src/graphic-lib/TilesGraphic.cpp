#include "include/ActiveState_dir/Borad_dir/TilesGraphic.h"
#include "../board-lib/include/Grid.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <string>
#include "include/TextureLoadingError.h"
#include <sstream>


TilesGraphic::TilesGraphic(const WindowView& windowView, std::shared_ptr<Grid*> grid,
                          std::shared_ptr<std::queue<sf::Sprite>> specialQueue)
: AbstractWindow(windowView), grid(grid), specialQueue(specialQueue)
{
    loadTextures();
}


void TilesGraphic::render()
{

    std::vector<std::pair<int,int>> usedTiles {};

    float tile_height = 10;
    float tile_width = 10;
    float leftOfset = windowView.leftOffset;
    float topOfset = windowView.topOffset;

    for (int i = 0; i < 51; i++)
    {
    int g = i + 1;
        for (int j = 0; j < 52; j++)
        {
            float x_pos = j * tile_height;
            float y_pos2 = g * tile_height;
            float y_pos1 = i * tile_width;

            TileType tileType1 = (*grid)->getTileAtPosition(j, i);
            TileType tileType2 = (*grid)->getTileAtPosition(j, g);

            std::vector<std::pair<int,int>>::iterator it1 = std::find(usedTiles.begin(), usedTiles.end(), std::pair<int,int>{j, i});
            std::vector<std::pair<int,int>>::iterator it2 = std::find(usedTiles.begin(), usedTiles.end(), std::pair<int,int>{j, g});

            if (tileType1 == TileType::Bricks && it1 == usedTiles.end())
            {
                sf::Sprite tile;
                setTileTexture(tile, tileType1, x_pos, y_pos1, tile_height, tile_width);
                windowView.window->draw(tile);
                usedTiles.push_back({g, i});
            }

            if (tileType2 == TileType::Bricks && it2 == usedTiles.end())
            {
                sf::Sprite tile;
                setTileTexture(tile, tileType2, x_pos, y_pos2, tile_height, tile_width);
                windowView.window->draw(tile);
                usedTiles.push_back({g, i});
            }

            if (tileType1 != TileType::Bricks && tileType2 != TileType::Bricks
                && tileType1 != TileType::NullTile && tileType2 != TileType::NullTile
                && it1 == usedTiles.end() && it2 == usedTiles.end())
            {
                sf::Sprite tile;
                setTileTexture(tile, tileType1, x_pos, y_pos1, tile_height,
                               tile_width, sf::Vector2f(2.f, 2.f));
                if (tileType1 != TileType::Trees)
                    windowView.window->draw(tile);
                else
                    specialQueue->push(tile);
                usedTiles.push_back({j, i});
                usedTiles.push_back({j, g});
                usedTiles.push_back({j+1, i});
                usedTiles.push_back({j+1, g});
            }
        }
    }
}


void TilesGraphic::setTileTexture(sf::Sprite& tile, TileType tileType, float x_pos, float y_pos,
                                         float tile_height, float tile_width,
                                         sf::Vector2f scale)
{
    float leftOfset = windowView.leftOffset;
    float topOfset = windowView.topOffset;
    sf::Texture* texture = &textureMap[tileType];
    tile.setTexture(*texture);
    tile.setPosition(x_pos + leftOfset, y_pos + topOfset);
    tile.setScale(scale);
}


void TilesGraphic::loadTextures()
{

    std::unordered_map<TileType, std::string>::iterator it_path = pathMap.begin();
    TileType type;
    sf::Texture texture;
    std::string path;

    while (it_path != pathMap.end())
    {

        type = it_path->first;
        path = pathMap[type];
        if(type != TileType::NullTile)
            if (!texture.loadFromFile(path))
            {
                std::stringstream ss;
                ss << "Failed to load tile texture: " << path;
                throw TextureLoadingError(ss.str());
            }
            else
            {
                textureMap[type] = texture;
            }
        it_path ++;
    }

}


Grid** TilesGraphic::getGrid()
{return grid.get();};