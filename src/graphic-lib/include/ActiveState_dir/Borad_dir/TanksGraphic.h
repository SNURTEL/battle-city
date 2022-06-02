#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../../AbstractWindow.h"
#include "../../../../tank-lib/include/Tank.h" // Does a better way exists?



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
protected:


    std::vector<Tank*>* tanks;

    /// @brief Sturct that links object position with its texture
    struct RenderObject
    {
        sf::Vector2f coords;
        Direction direction;
        Tank::TankType textureType;
        RenderObject(const sf::Vector2f& coords, Direction direction,
                     Tank::TankType textureType);
        RenderObject();
    };

    /// @brief Maps texture types to actual textures
    std::unordered_map<Tank::TankType, sf::Texture> textureMap{}; // Why before initialization compilatro was showing an error

    /// @brief Maps paths types to actual textures
    std::unordered_map<Tank::TankType, std::string> pathMap
    {
        {Tank::ArmorTank, "../src/graphic-lib/tanksImages/ArmorTank.png"},
        {Tank::FastTank, "../src/graphic-lib/tanksImages/FastTank.png"},
        {Tank::BasicTank, "../src/graphic-lib/tanksImages/BasicTank.png"},
        {Tank::PlayerTank, "../src/graphic-lib/tanksImages/PlayerTank.png"},
        {Tank::PowerTank, "../src/graphic-lib/tanksImages/PowerTank.png"}
    };


    /// @brief Sets tank's appropriate rotation
    void setTankRotation(sf::Sprite& sprite, float angle);

    /**
     * @brief Makes RenderObjects vector from tanks list
     *
     * Stores made RenderObjects vector in vector renderTanks
     */
    void makeRenderTanks();

    std::vector<RenderObject> renderTanks;

    /// @brief Loads textures into the textureMap
    void loadTextures();

    /// @brief Returns angle in degrees from given direction
    float getAngle(Direction direction) const;

public:

   /// @brief Renders all objects on the screen in given order
    virtual void render() override;


    // /// @brief Updates its objects
    // virtual void update(std::vector<Tank*> tanks);

    std::vector<Tank*>* getTanks() const;

    /**
     * @brief Get the RenderObjects objects in a vector
     *
     * @return std::vector<RenderTank>
     */
    std::vector<RenderObject> getRenderTanks();


    /**
     * @brief Construct a new TanksGraphic object
     *
     * @param window
     * @param TanksGraphic
     */
    TanksGraphic(const WindowView& windowView, std::vector<Tank*>* tanks);
};

#endif //PROI_PROJEKT_TANKSGRAPHIC_H