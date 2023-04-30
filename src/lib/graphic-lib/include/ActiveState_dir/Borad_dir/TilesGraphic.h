// Created by Michał


#ifndef PROI_PROJEKT_TILESGRAPHIC_H
#define PROI_PROJEKT_TILESGRAPHIC_H

#include "../../AbstractWindow.h"
#include "../../../../board-lib/include/Grid.h"
#include <unordered_map>
#include <queue>


class Grid;

/**
 * @brief Class responsible for board updates and render
 *
 * Fetchs tiles positions and redners its on the screen
 * Defines enum TextureType for matching textures with tile type
 * Defines struct RenderObject for storing matched texture together with tile coords
 * Stores paths to textures in a std::map<TextureType, std::string>
 */
class TilesGraphic : public AbstractWindow
{
protected:

    /// @brief Sturct that links object position with its texture
    struct RenderObject
    {
        sf::Vector2f coords;
        TileType textureType;
    };


    std::shared_ptr<std::queue<sf::Sprite>> specialQueue;


    std::shared_ptr<Grid*> grid;


    /// @brief Maps texture types to actual paths to textures
    std::unordered_map<TileType, std::string> pathMap{
        {TileType::Bricks, "../../src/lib/graphic-lib/tilesImages/Bricks.png"},
        {TileType::Steel, "../../src/lib/graphic-lib/tilesImages/Steel.png"},
        {TileType::Trees, "../../src/lib/graphic-lib/tilesImages/Grass.png"},
        {TileType::Water, "../../src/lib/graphic-lib/tilesImages/Water.png"}
    };


    /// @brief Maps texture types to actual SFML textures
    std::unordered_map<TileType, sf::Texture> textureMap;


    /// @brief Sets texutres in textureMap
    void loadTextures(); // possible to make as a friend template to all board related classes


    /// @brief Sets attributes of a given tile texture object
    void setTileTexture(sf::Sprite& tile, TileType tileType, float x_pos, float y_pos,
                           float tile_height, float tile_width,
                           sf::Vector2f scale = sf::Vector2f(1.f, 1.f));

public:

    /// @brief Renders all objects on the screen in given order
    virtual void render() override;


    /**
     * @brief Get the Grid object
     *
     * @return Grid*
     */
    Grid** getGrid();


    /**
     * @brief Get the RenderObjects objects in a vector
     *
     * @return std::vector<RenderTile>
     */
    std::vector<RenderObject> getRenderTiles();


    /**
     * @brief Construct a new TilesGraphic object
     *
     * @param windowView
     * @param grid
     * @param specialQueue
     */
    TilesGraphic(const WindowView& windowView, std::shared_ptr<Grid*> grid,
                 std::shared_ptr<std::queue<sf::Sprite>> specialQueue);
};

#endif //PROI_PROJEKT_TILESGRAPHIC_H