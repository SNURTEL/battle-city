#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Window.h"
#include <queue>

#ifndef PROI_PROJEKT_BOARDVIEW_H
#define PROI_PROJEKT_BOARDVIEW_H

// enum TileType;
// class Tank;
class Bullet;

/**
 * @brief Class responsible for board updates and render
 *
 * Renders the board and all objects connected to it tanks, tiles, bulltes.
 * Using the SFML View
 */
class BoardView
{
private:
    sf::View view;
    sf::RenderWindow* window;

    /// @brief Maps texture types to actual paths to textures
    std::map<TextureType, std::string> textureMap;

    /// @brief Queue that stores vocotrs of elements to be rendered in apropriate order
    std::queue<std::vector<RenderObject>> renderQueue;

public:
    /// @brief Renders all objects on the screen
    void renderTexturesQueue();


    /**
     * @brief Construct a new Board View object
     *
     * Positions the view in the window.
     *
     * @param window
     * @param renderQueue
     */
    BoardView(sf::RenderWindow* window, const sf::FloatRect& boardView, std::queue<std::vector<RenderObject>> renderQueue);
};

#endif //PROI_PROJEKT_BOARDVIEW_H