// Created by Michał


#ifndef PROI_PROJEKT_TANKSGRAPHIC_H
#define PROI_PROJEKT_TANKSGRAPHIC_H

#include <queue>
#include <unordered_map>
#include "../../AbstractWindow.h"
#include "../../../../tank-lib/include/Tank.h"

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


    std::shared_ptr<std::vector<std::shared_ptr<Tank>>> tanks;


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
        {Tank::ArmorTank, "../../src/lib/graphic-lib/tanksImages/ArmorTank.png"},
        {Tank::FastTank, "../../src/lib/graphic-lib/tanksImages/FastTank.png"},
        {Tank::BasicTank, "../../src/lib/graphic-lib/tanksImages/BasicTank.png"},
        {Tank::PlayerTank, "../../src/lib/graphic-lib/tanksImages/PlayerTank.png"},
        {Tank::PowerTank, "../../src/lib/graphic-lib/tanksImages/PowerTank.png"}
    };


    /// @brief Sets tank's appropriate rotation
    void setTankRotation(sf::Sprite& sprite, float angle);


    /**
     * @brief Makes RenderObjects vector from tanks list
     *
     * Stores made RenderObjects vector in vector renderTanks
     */
    void makeRenderTanks();


    std::shared_ptr<std::queue<sf::Sprite>> specialQueue;


    std::vector<RenderObject> renderTanks;


    /// @brief Loads textures into the textureMap
    void loadTextures();


    /// @brief Returns angle in degrees from given direction
    float getAngle(Direction direction) const;

public:

   /// @brief Renders all objects on the screen in given order
    virtual void render() override;


    /**
     * @brief Get the Tanks object
     *
     * @return std::vector<std::shared_ptr<Tank>>*
     */
    std::vector<std::shared_ptr<Tank>>* getTanks() const;


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
    TanksGraphic(const WindowView& windowView, std::shared_ptr<std::vector<std::shared_ptr<Tank>>> tanks,
                 std::shared_ptr<std::queue<sf::Sprite>> specialQueue);
};

#endif //PROI_PROJEKT_TANKSGRAPHIC_H