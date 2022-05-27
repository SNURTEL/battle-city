#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../../AbstractWindow.h"


#ifndef PROI_PROJEKT_TANKSGRAPHIC_H
#define PROI_PROJEKT_TANKSGRAPHIC_H


/**
 * @brief Class responsible for tanks updates and render
 *
 * Fetchs tanks positions and renders its on the screen using appropiate texture
 * Defines enum TextureType for matching textures with tank type
 * Defines structr RenderObject for sotring matched texture together with tank coords
 * Stores paths to textures in a std::map<TextureType, std::string>
 */
class TanksGraphic : public AbstractWindow
{
private:
     enum TextureType
    {
        PlayerTank=0,
        BasicTank,
        FastTank,
        PowerTank,
        ArmorTank,
    };

    /// @brief Sturct that links object position with its texture
    struct RenderObject
    {
        sf::Vector2f coords;
        TextureType textureType;
    };

    /// @brief Maps texture types to actual paths to textures
    std::map<TextureType, std::string> textureMap;

    /**
     * @brief Makes RenderObjects vector from tanks list
     *
     * @return std::vector<RenderObject>
     */
    std::vector<RenderObject> makeRenderTanks() const;

    /// @brief Enum that conains possible tanks types textures


public:

   /// @brief Renders all objects on the screen in given order
    virtual void render() const override;


    /// @brief Updates its objects
    virtual void update(std::vector<Tank*> tanks);


    /**
     * @brief Get the RenderObjects objects in a vector
     *
     * @return std::vector<RenderTank>
     */
    std::vector<RenderObject> getRenderTanks();


    /**
     * @brief Construct a new TanksGraphic object
     *
     * Positions the view in the window.
     *
     * @param window
     * @param TanksGraphic
     */
    TanksGraphic(sf::RenderWindow* window);
};

#endif //PROI_PROJEKT_TANKSGRAPHIC_H