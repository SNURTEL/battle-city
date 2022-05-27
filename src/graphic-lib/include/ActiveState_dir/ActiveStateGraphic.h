#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../Window.h"


#ifndef PROI_PROJEKT_ACTIVESTATEGRAPHIC_H
#define PROI_PROJEKT_ACTIVESTATEGRAPHIC_H

class Bullet;


/**
 * @brief Class resoponsible for coumputing ActiveState graphic
 *
 * Sets views for board and frame rendering
 * Updates its children attributes
 * Commands its children to render objects
 *
 */
class ActiveStateGraphic : public AbstractWindow
{

public:

    /// @brief Stores given board object pointers
    struct BoardPointers
    {
        std::vector<Tank*> tanks;
        std::vector<Bullet*> bullets;
        int* tiles[52][52];
    };


    /// @brief Stores given frame object pointers
    struct FramePointers
    {
        int level;
        int playerLivesLeft;
        // And other ...
        // Will be added later
    };


    /**
     * @brief Construct a new Active State Graphic object
     *
     * Sets atributes boardView, frameView
     *
     * @param window
     */
    ActiveStateGraphic(sf::RenderWindow* window);


    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render() const override;


    /**
     * @brief Commands its children to update objects to render
     *
     */
    void update(Window::ActiveStatePointers objectsPointers);

private:

    sf::View boardView;
    sf::View frameView;

    /// @brief Calculates the view for board rendering
    sf::View setboardView();

    /// @brief Calculates the view for frame rednering
    sf::View setframeView();

    BoardPointers boardObjects;
    FramePointers frameObjects;


};

#endif // PROI_PROJEKT_ACTIVESTATEGRAPHIC_H