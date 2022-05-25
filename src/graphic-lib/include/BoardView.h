#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "FrameView.h"
// #include "../tank-lib/include/Tank.h"

#ifndef PROI_PROJEKT_BOARDVIEW_H
#define PROI_PROJEKT_BOARDVIEW_H

// enum TileType;
// class Tank;
class Bullet;

/**
 * \brief Class responsible for board updates and render
 *
 * Renders the board and all objects connected to it tanks, tiles, bulltes.
 * Using the SFML View
 */
class BoardView
{
private:
    sf::View view;
    sf::RenderWindow* window;
    // TileType* grid[52][52];

    // std::vector<RenderObject>
    // std::vector<Bullet>* bullets;
public:
    /// \brief Renders tanks on the screen
    void renderTanks();

    /// \brief Renders tiles on the screen
    void renderTiles();

    /// \brief Renders bullets on the screen
    void renderBullets();

    /**
     * @brief Construct a new Board View object
     *
     * @param window
     * @param grid
     * @param tanks
     * @param bullets
     *
     * Positions the view in the window.
     */
    BoardView(sf::RenderWindow window, TileType* grid[52][52], std::vector<Tank>* tanks /*, std::vector<Bullet>* bullets*/);
};

#endif //PROI_PROJEKT_BOARDVIEW_H