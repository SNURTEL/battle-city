// Created by Michał


#ifndef PROI_PROJEKT_ACTIVESTATEGRAPHIC_H
#define PROI_PROJEKT_ACTIVESTATEGRAPHIC_H


#include "../Window.h"

class Bullet;


/**
 * @brief Class resoponsible for computing ActiveState graphic
 *
 * Sets offsets in WindowView objects for board and frame rendering
 * Updates its children
 * Commands its children to render objects
 *
 */
class ActiveStateGraphic : public AbstractWindow
{

public:

    /// @brief Stores given board object pointers
    struct BoardPointers
    {
        std::shared_ptr<std::vector<std::shared_ptr<Tank>>> tanks;
        std::shared_ptr<std::vector<std::shared_ptr<Bullet>>> bullets;
        std::shared_ptr<Grid*> tiles;
        std::shared_ptr<sf::Vector2f> eaglePos;
    };


    /// @brief Stores given frame object pointers
    struct FramePointers
    {
        std::shared_ptr<int> level;
        std::shared_ptr<int> playerLives;
        std::shared_ptr<int> points;
    };


    /**
     * @brief Construct a new Active State Graphic object
     *
     * Sets atributes boardView, frameView
     * Constructs next composite branches
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