// Created by Michał


#ifndef PROI_PROJEKT_ABSTRACTWINDOW_H
#define PROI_PROJEKT_ABSTRACTWINDOW_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>

class Tank;
class GameState;


/**
 * @brief Struct that contains the sf::RenderWindow nad offset of elements to render
 */
struct WindowView
{
    sf::RenderWindow* window;
    float topOffset;
    float leftOffset;
    WindowView(sf::RenderWindow* window, float topOffset, float leftOffset);
    WindowView();
};


/**
 * @brief Contains main SFML functions
 *
 * Class that summars main graphic-lib functionalities.
 * The basic class for the composit classes
 * It contains vector of pointers to its children
 * Sets interface for updating and rendering board and its objects.
 * Window size is 800/800
 */
class AbstractWindow
{
protected:

    const uint WINDOW_HEIGHT = 700;
    const uint WINDOW_WIDTH = 700;
    WindowView windowView;
    std::vector<std::shared_ptr<AbstractWindow>> children;

public:

    /**
     * @brief Get the Window View object
     *
     * @return const WindowView&
     */
    sf::RenderWindow* getWindow();

    /**
     * @brief Commands its children to render objects on the screen
     *
     */

    virtual void render()=0;

    AbstractWindow();

    /**
     * @brief Construct a new Abstract Window object
     *
     * Sets windowView attribute
     *
     * @param windowView
     */
    AbstractWindow(const WindowView& windowView);

    virtual ~AbstractWindow();
};


#endif //PROI_PROJKET_GRAPHIC_H