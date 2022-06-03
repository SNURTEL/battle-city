#include "include/ActiveState_dir/Borad_dir/TilesGraphic.h"
#include "../board-lib/include/Grid.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <string>



TilesGraphic::TilesGraphic(WindowView windowView, std::shared_ptr<Grid*> grid)
: AbstractWindow(windowView), grid(grid)
{
    loadTextures();
}

// std::vector<TilesGraphic::RenderObject> TilesGraphic::getRenderTiles()
// {

// }

void TilesGraphic::render()
{

    std::vector<std::pair<int,int>> usedTiles {};

    float tile_height = 10;
    float tile_width = 10;
    float leftOfset = windowView.leftOfset;
    float topOfset = windowView.topOfset;

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
                createTileTexture(tileType1, x_pos, y_pos1, tile_height, tile_width);
                usedTiles.push_back({g, i});
            }

            if (tileType2 == TileType::Bricks && it2 == usedTiles.end())
            {
                createTileTexture(tileType2, x_pos, y_pos2, tile_height, tile_width);
                usedTiles.push_back({g, i});
            }

            if (tileType1 != TileType::Bricks && tileType2 != TileType::Bricks
                && tileType1 != TileType::NullTile && tileType2 != TileType::NullTile
                && it1 == usedTiles.end() && it2 == usedTiles.end())
            {
                createTileTexture(tileType1, x_pos, y_pos1, tile_height,
                                  tile_width, sf::Vector2f(2.f, 2.f));
                usedTiles.push_back({j, i});
                usedTiles.push_back({j, g});
                usedTiles.push_back({j+1, i});
                usedTiles.push_back({j+1, g});
            }
        }
    }
}


void TilesGraphic::createTileTexture(TileType tileType, float x_pos, float y_pos,
                                    float tile_height, float tile_width,
                                    sf::Vector2f scale)
{

    float leftOfset = windowView.leftOfset;
    float topOfset = windowView.topOfset;
    sf::Texture* texture = &textureMap[tileType];
    sf::RectangleShape tile(sf::Vector2f(tile_height, tile_width));
    tile.setPosition(x_pos + leftOfset, y_pos + topOfset);
    tile.setTexture(texture);
    tile.setScale(scale);
    windowView.window->draw(tile);
}


void TilesGraphic::loadTextures()
{

    std::unordered_map<TileType, std::string>::iterator it_path = pathMap.begin();
    // std::unordered_map<TileType, sf::Texture>::iterator it_texture = textureMap.begin();
    TileType type;
    sf::Texture texture;
    std::string path;

    // Not the best solution but iterating over an enum is problematic
    while (it_path != pathMap.end())
    {

        type = it_path->first;
        // texture = it_path->second;
        path = pathMap[type];
        if(type != TileType::NullTile)
            if (!texture.loadFromFile(path))
            {
                // Exceptions
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