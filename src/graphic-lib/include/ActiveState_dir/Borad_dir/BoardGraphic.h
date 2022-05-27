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
private:
    ActiveStateGraphic::BoardPointers boardObjects;
public:
    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render() const override;


    /**
     * @brief Commands its children to update objects to render
     *
     */
    void update(ActiveStateGraphic::BoardPointers boardObjects);


    /**
     * @brief Construct a new Board View object
     *
     * @param window
     */
    BoardGraphic(sf::RenderWindow* window);
};

#endif //PROI_PROJEKT_BoardGraphic_H