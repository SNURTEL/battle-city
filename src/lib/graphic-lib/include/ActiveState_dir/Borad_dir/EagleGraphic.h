// Created by Michał


#ifndef PROI_PROJEKT_EAGLEGRAPHIC_H
#define PROI_PROJEKT_EAGLEGRAPHIC_H

#include "../../AbstractWindow.h"
#include "../../../../tank-lib/include/Tank.h"
#include <queue>
#include "../../../../board-lib/include/Eagle.h"

/**
 * @brief Class responsible for eagle rendering
 *
 * Fetchs eagle positions and renders its on the screen using appropiate texture
 * Stores paths to eagle's texture
 */
class EagleGraphic : public AbstractWindow
{
protected:

    std::shared_ptr<sf::Vector2f> eaglePos;

    std::string pathToTexture = "../../src/lib/graphic-lib/eagleImage/Eagle.png";

    bool eagleSet = false;

    std::shared_ptr<std::queue<sf::Sprite>> specialQueue;

    sf::Texture eagleTexture;


    /**
     * @brief Sets eagle position
     *
     */
    void setEaglePos();


    /// @brief Loads eagle's texture
    void loadTexture();

public:

   /// @brief Renders eagle on the screen
   virtual void render() override;


    /**
     * @brief Construct a new EagleGraphic object
     *
     * @param window
     * @param eagle
     * @param specialQueue
     */
    EagleGraphic(const WindowView& windowView, std::shared_ptr<sf::Vector2f> eagle,
                 std::shared_ptr<std::queue<sf::Sprite>> specialQueue);
};

#endif //PROI_PROJEKT_EAGLEGRAPHIC_H