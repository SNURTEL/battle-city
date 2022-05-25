#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
// #include "BoardView.hpp"
// #include "../tank-lib/include/Tank.h"

#ifndef PROI_PROJEKT_FRAMEVIEW_H
#define PROI_PROJEKT_FRAMEVIEW_H

enum TileType;
class Tank;
class Bullet;


/**
 * \brief Class responsible for frame upadates and render
 *
 * Redners the frame around the board and all the objects connected to it.
 */
class FrameView
{
private:
    // Tank* playerTank; How will we implement this?
    sf::RenderWindow* window;
public:

    /// \brief Renders bare frame made from sf::Ractangle shape around the board
    void renderBareFrame();

    /// \brief Renders tiles on the screen
    void renderTiles();

};


#endif // PROI_PROJEKT_FRAMEVIEW_H