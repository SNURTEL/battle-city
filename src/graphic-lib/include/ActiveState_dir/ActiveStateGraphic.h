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
        Grid* tiles;
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
    ActiveStateGraphic(const WindowView& windowView, const Window::ActiveStatePointers& activeStatePointers);


    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render() override;

    /// @brief Returns reference to class children
    std::vector<std::shared_ptr<AbstractWindow>>& getChildren();

private:

    /// @brief Creates appropriate children
    void conscructComposite();

    WindowView boardWindowView;
    WindowView frameWindowView;

    /// @brief Calculates the view for board rendering
    void setboardView();

    /// @brief Calculates the view for frame rednering
    void setframeView();

    /// @brief Sets boardObjects from activeStateGraphic
    void setboardObjects();

    /// @brief Sets frameObjects from activeStateGraphic
    void setframeObjects();

    BoardPointers boardObjects;
    FramePointers frameObjects;
    Window::ActiveStatePointers activeStateObjects;


};

#endif // PROI_PROJEKT_ACTIVESTATEGRAPHIC_H