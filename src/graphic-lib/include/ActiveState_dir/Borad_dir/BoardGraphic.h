#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
// #include "../../AbstractWindow.h"
#include "../ActiveStateGraphic.h"
#include <queue>

#ifndef PROI_PROJEKT_BoardGraphic_H
#define PROI_PROJEKT_BoardGraphic_H

// enum TileType;
// class Tank;
class Bullet;

/**
 * @brief Class responsible for board updates and render
 *
 * Updates its children attributes
 * Commands its children to render objects
 */
class BoardGraphic : public AbstractWindow
{
protected:
    ActiveStateGraphic::BoardPointers boardObjects;

    /// @brief Creates appropriate children
    void conscructComposite();


    // void setBoardObjects();
public:
    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render() override;

    // /**
    //  * @brief Commands its children to update objects to render
    //  *
    //  */
    // virtual void update();

    /**
     * @brief Construct a new Board View object
     *
     * @param window
     */
    BoardGraphic(const WindowView& windowView, const ActiveStateGraphic::BoardPointers& boardObjects);
};

#endif //PROI_PROJEKT_BoardGraphic_H