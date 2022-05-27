#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../../AbstractWindow.h"

#ifndef PROI_PROJEKT_BULLETSGRAPHIC_H
#define PROI_PROJEKT_BULLETSGRAPHIC_H


class Bullet;

/**
 * @brief Class responsible for bullets updates and render
 *
 * Fetchs bullets positions and redners its on the screen
 * Defines enum TextureType for matching textures with bullet type
 * Defines struct RenderObject for storing matched texture together with bullet coords
 * Stores paths to textures in a std::map<TextureType, std::string>
 */
class BulletsGraphic : public AbstractWindow
{
private:

    /// @brief Enum that conains possible tanks types textures
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
    std::vector<RenderObject> makeRenderBullets() const;
public:

    /// @brief Renders all objects on the screen in given order
    virtual void render() const override;


    /// @brief Updates its objects
    void update(std::vector<Bullet*> bullets);


    /**
     * @brief Get the RenderObjects object in a vector
     *
     * @return std::vector<sf::Vector2f>
     */
    std::vector<RenderObject> getRenderBullets(std::vector<Bullet>* bullets);


    /**
     * @brief Construct a new BulletsGraphic object
     *
     * Positions the view in the window.
     *
     * @param window
     * @param TanksGraphic
     */
    BulletsGraphic(sf::RenderWindow* window);
};

#endif //PROI_PROJEKT_BULLETSGRAPHIC_H