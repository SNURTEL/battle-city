#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


#ifndef PROI_PROJEKT_ABSTRACTWINDOW_H
#define PROI_PROJEKT_ABSTRACTWINDOW_H

class Tank;
class GameState;


/**
 * @brief Contains main SFML functions
 *
 * Class that summars main graphic-lib functions.
 * The basic class for the composit classes
 * It contains vector of pointers to its children
 * Sets interface for updating and rendering board and its objects.
 * Window size is 800/800
 */
class AbstractWindow
{
protected:

    const uint WINDOW_HEIGHT = 800;
    const uint WINDOW_WIDTH = 800;
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    std::vector<std::unique_ptr<AbstractWindow>> children;

public:
    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render() const;


    /**
     * @brief Commands its children to update objects to render
     *
     */
    virtual void update();


    virtual ~AbstractWindow();
};

#endif //PROI_PROJKET_GRAPHIC_H