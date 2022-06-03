#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../../AbstractWindow.h"
#include <map>
#include "../../../../board-lib/include/Grid.h"
#include <unordered_map>

#ifndef PROI_PROJEKT_TILESGRAPHIC_H
#define PROI_PROJEKT_TILESGRAPHIC_H


// enum TileType: unsigned int;
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

    std::shared_ptr<Grid*> grid;

    /// @brief Maps texture types to actual paths to textures
    std::unordered_map<TileType, std::string> pathMap{
        {TileType::Bricks, "../src/graphic-lib/tilesImages/Bricks.png"},
        {TileType::Steel, "../src/graphic-lib/tilesImages/Steel.png"},
        {TileType::Trees, "../src/graphic-lib/tilesImages/Grass.png"},
        {TileType::Water, "../src/graphic-lib/tilesImages/Water.png"}
    };

    /// @brief Maps texture types to actual SFML textures
    std::unordered_map<TileType, sf::Texture> textureMap;

    /// @brief Sets texutres in textureMap
    void loadTextures(); // possible to make as a friend template to all board related classes

    /// @brief Creates tile texture object and draws it on the screen
    void createTileTexture(TileType tileType, float x_pos, float y_pos,
                           float tile_height, float tile_width,
                           sf::Vector2f scale = sf::Vector2f(1.f, 1.f));

public:

    /// @brief Renders all objects on the screen in given order
    virtual void render() override;


    // /// @brief Updates its objects
    // void update(int *tiles[52][52]);

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
     */
    TilesGraphic(WindowView windowView, std::shared_ptr<Grid*> grid);
};

#endif //PROI_PROJEKT_TILESGRAPHIC_H