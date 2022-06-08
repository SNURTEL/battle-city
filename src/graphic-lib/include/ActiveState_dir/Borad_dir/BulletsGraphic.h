// Created by Michał


#ifndef PROI_PROJEKT_BULLETSGRAPHIC_H
#define PROI_PROJEKT_BULLETSGRAPHIC_H


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "../../AbstractWindow.h"
#include "../../../../tank-lib/include/Bullet.h"
#include <queue>


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
protected:

    /// @brief Sturct that links object position with its texture
    struct RenderObject
    {
        sf::Vector2f coords;
        Bullet::BulletType textureType;
        Direction direction;
        RenderObject(const sf::Vector2f& pos, Direction direction, Bullet::BulletType type=Bullet::Enemy);
    };


    /// @brief Maps texture types to actual paths to textures
    std::unordered_map<Bullet::BulletType, std::string> pathMap
    {
        {Bullet::Enemy, "../src/graphic-lib/bulletsImages/bullet.png"},
        {Bullet::Friendly, "../src/graphic-lib/bulletsImages/bullet.png"}
    };


    /// @brief Maps texture types to actual textures
    std::unordered_map<Bullet::BulletType, sf::Texture> textureMap{};


    std::shared_ptr<std::vector<std::shared_ptr<Bullet>>> bullets;


    std::shared_ptr<std::queue<sf::Sprite>> specialQueue;


    std::vector<RenderObject> renderBullets;
    /**
     * @brief Makes RenderObjects vector from tanks list
     *
     * @return std::vector<RenderObject>
     */
    std::vector<RenderObject> makeRenderBullets() const;


    /// @brief Loads textures into the textureMap
    void loadTextures();


    /// @brief Returns angle in degrees from given direction
    float getAngle(Direction direction) const;


    /// @brief Sets bullet's appropriate rotation
    void setBulletRotation(sf::Sprite& sprite, float angle);


    /**
     * @brief Makes RenderObjects vector from tanks list
     *
     * Stores made RenderObjects vector in vector renderTanks
     */
    void makeRenderBullets();

public:

    /// @brief Renders all objects on the screen in given order
    virtual void render() override;


    /**
     * @brief Get the RenderObjects object in a vector
     *
     * @return std::vector<sf::Vector2f>
     */
    std::vector<RenderObject> getRenderBullets(std::vector<std::shared_ptr<Bullet>>* bullets);


    /**
     * @brief Get the Bullets object
     *
     * @return std::vector<std::shared_ptr<Bullet>>*
     */
    std::vector<std::shared_ptr<Bullet>>* getBullets() const;


    /**
     * @brief Construct a new BulletsGraphic object
     *
     * Positions the view in the window.
     *
     * @param window
     * @param TanksGraphic
     */
    BulletsGraphic(const WindowView& windowView, std::shared_ptr<std::vector<std::shared_ptr<Bullet>>> bullets,
                   std::shared_ptr<std::queue<sf::Sprite>> specialQueue);
};

#endif //PROI_PROJEKT_BULLETSGRAPHIC_H