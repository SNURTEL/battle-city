// Created by Michał


#ifndef PROI_PROJEKT_BoardGraphic_H
#define PROI_PROJEKT_BoardGraphic_H

#include "../ActiveStateGraphic.h"
#include <queue>

class Bullet;

/**
 * @brief Class responsible for board updates and render
 *
 * Derived from AbstarctWindow
 * Stores struce BoardPointers
 * Commands its children to render objects
 */
class BoardGraphic : public AbstractWindow
{
protected:

    ActiveStateGraphic::BoardPointers boardObjects;


    /// @brief Creates appropriate children
    void conscructComposite();

    /**
     *  @brief Queue used to draw special priority textues
     *
     *  Tetures added to the queue are renedered at the end of
     *  Board Graphic rendering sesion
     */
    std::shared_ptr<std::queue<sf::Sprite>> extraRenderQueue;

public:

    /**
     * @brief Commands its children to render objects on the screen
     *
     * Constructs next composite branches
     */
    virtual void render() override;


    const ActiveStateGraphic::BoardPointers& getPointers() const;


    const std::vector<std::shared_ptr<AbstractWindow>> getChildren() const;;


    /// @brief Initates special render queue
    void initiateRenderQueue();


    /**
     * @brief Construct a new Board View object
     *
     * @param window
     */
    BoardGraphic(const WindowView& windowView, const ActiveStateGraphic::BoardPointers& boardObjects);
};

#endif //PROI_PROJEKT_BoardGraphic_H