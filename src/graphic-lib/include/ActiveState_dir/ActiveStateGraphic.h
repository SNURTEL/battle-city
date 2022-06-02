#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../Window.h"
// #include "Frame_dir/FrameGraphic.h"


#ifndef PROI_PROJEKT_ACTIVESTATEGRAPHIC_H
#define PROI_PROJEKT_ACTIVESTATEGRAPHIC_H

class Bullet;
// class FrameGraphic;

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
        std::shared_ptr<std::vector<Tank*>> tanks;
        std::shared_ptr<std::vector<Bullet*>> bullets;
        std::shared_ptr<Grid*> tiles;
    };


    /// @brief Stores given frame object pointers
    struct FramePointers
    {
        std::shared_ptr<int> level;

        std::shared_ptr<int*> playerLivesLeft;
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

    // /**
    //  * @brief Commands its children to update objects to render
    //  *
    //  */
    // virtual void update();

    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render() override;

    /// @brief Returns reference to class children
    std::vector<std::shared_ptr<AbstractWindow>>& getChildren();

protected:

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