#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "BoardView.h"
#include "FrameView.h"
#include "../../tank-lib/include/Tank.h"
// #include "../tank-lib/include/Tank.h"

#ifndef PROI_PROJEKT_GRAPHIC_H
#define PROI_PROJEKT_GRAPHIC_H

enum TileType;
// class Bullet;


/**
 * \brief Contains main SFML functions
 *
 * Class that summars main graphic-lib functions.
 * It contains BoardView and FrameView objects and functions
 * resposible for updating and rendering board and its objects.
 * Window size is 800/800
 */
class Window
{
private:

    const uint WINDOW_HEIGHT = 800;
    const uint WINDOW_WIDTH = 800;
    sf::Vector2u windowSize;
    sf::Vector2u boardviewSize;
    std::unique_ptr<sf::RenderWindow> window;
    BoardView boardView;
    FrameView frameView;
    TileType* grid[52][52];
    // Tank* playerTank; How we will implement this?
    std::vector<Tank>* tanks;
    // std::vector<Bullet>* bullets;

public:
    Window(TileType* grid[52][52], std::vector<Tank>* tanks/*, std::vector<Bullet>* bullets*/);

    /**
     * @brief Renders objects on the screen
     *
     */
    void render() const;

    /**
     * @brief Updates objects to render
     *
     */
    void update();

    ~Window();
};


/// \brief Sturct that should be given to BoardView for rendering tanks
struct RenderTank
{
    sf::Vector2f coords;
    Tank::TankType textureType;
};


/// \brief Sturct that should be given to BoardView for rendering tiles
struct RenderTile
{
    sf::Vector2f coords;
    Tank::TankType textureType;
};




#endif //PROI_PROJKET_GRAPHIC_H