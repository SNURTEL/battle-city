#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>


#ifndef PROI_PROJEKT_ABSTRACTWINDOW_H
#define PROI_PROJEKT_ABSTRACTWINDOW_H

class Tank;
class GameState;


/**
 * @brief Struct that links window with view that should be applyed before rendering
 *
 * Before rendering leafs objects sets on window given view that is set by some
 * previous composite object
 */
struct WindowView
{
    sf::RenderWindow* window;
    sf::View view;
    WindowView(sf::RenderWindow* window, sf::View);
    WindowView();
};


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
    WindowView windowView;
    std::vector<std::shared_ptr<AbstractWindow>> children;

public:
    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render()=0;
    AbstractWindow();
    AbstractWindow(const WindowView& windowView);

    /**
     * @brief Sets defoult view on windowView.window
     *
     * Used after drawing objects using not default view
     *
     */
    void setDefalutView();

    // /**
    //  * @brief Commands its children to update objects to render
    //  *
    //  */
    // virtual void update();


    virtual ~AbstractWindow();
};


#endif //PROI_PROJKET_GRAPHIC_H