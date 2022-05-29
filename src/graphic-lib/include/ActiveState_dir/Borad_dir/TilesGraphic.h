#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../../AbstractWindow.h"
#include <map>

#ifndef PROI_PROJEKT_TILESGRAPHIC_H
#define PROI_PROJEKT_TILESGRAPHIC_H


enum TileType: unsigned int;
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

    Grid* grid;

    /// @brief Maps texture types to actual paths to textures
    std::map<TileType, std::string> textureMap;

    /**
     * @brief Makes RenderObjects vector from tanks list
     *
     * @return std::vector<RenderObject>
     */
    std::vector<RenderObject> makeRenderTiles() const;

public:

    /// @brief Renders all objects on the screen in given order
    virtual void render() override;


    // /// @brief Updates its objects
    // void update(int *tiles[52][52]);


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
    TilesGraphic(WindowView windowView, Grid* grid);
};

#endif //PROI_PROJEKT_TILESGRAPHIC_H