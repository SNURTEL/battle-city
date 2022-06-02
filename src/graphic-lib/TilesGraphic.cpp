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

    float tile_height = 10;
    float tile_width = 10;
    float leftOfset = windowView.leftOfset;
    float topOfset = windowView.topOfset;

    for (int i = 0; i < 52; i++)
    {
        for (int j = 0; j < 52; j++)
        {
            float x_pos = j * tile_height;
            float y_pos = i * tile_width;
            TileType tileType = (*grid)->getTileAtPosition(j, i);
            if(tileType != TileType::NullTile)
            {
                sf::Texture* texture = &textureMap[tileType];
                sf::RectangleShape tile(sf::Vector2f(tile_height, tile_width));
                tile.setPosition(x_pos + leftOfset, y_pos + topOfset);
                tile.setTexture(texture);
                windowView.window->draw(tile);
            }
        }
    }
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