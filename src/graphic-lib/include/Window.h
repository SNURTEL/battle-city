#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include "../../tank-lib/include/Tank.h"
#include "../../board-lib/include/Grid.h"


#ifndef PROI_PROJEKT_GRAPHIC_H
#define PROI_PROJEKT_GRAPHIC_H

enum TileType;

class BoardView;
class FrameView;

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
    sf::FloatRect boardviewSize;
    std::unique_ptr<sf::RenderWindow> window;
    sf::VideoMode videoMode;
    BoardView boardView;
    FrameView frameView;
    TileType* grid[52][52];
    // Tank* playerTank; How we will implement this?
    std::vector<Tank>* tanks;
    // std::vector<Bullet>* bullets;

    /**
     * @brief Makes RenderObjects vector from tanks list
     *
     * @return std::vector<RenderObject>
     */
    std::vector<RenderObject> makeRenderTanks() const;


    /**
     * @brief Makes RenderObjects vector from tiles list
     *
     * @return std::vector<RenderObject>
     */
    std::vector<RenderObject> makeRenderTile() const;


    /**
     * @brief Makes RenderObjects vector from bullete list
     *
     * @return std::vector<RenderObject>
     */
    std::vector<RenderObject> makeRenderBullet() const;


    /// @brief Makes renderQueue for BoardView
    std::queue<std::vector<RenderObject>> makeRenderQueue() const;


    /// @brief Calcualtes board size from a window size
    void calculateBoardSize();

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

    /**
     * @brief Get the RenderObjects objects in a vector
     *
     * @return std::vector<RenderTank>
     */
    std::vector<RenderObject> getRenderTanks();

    /**
     * @brief Get the RenderObjects objects in a vector
     *
     * @return std::vector<RenderTile>
     */
    std::vector<RenderObject> getRenderTiles();

    /**
     * @brief Get the RenderObjects object in a vector
     *
     * @return std::vector<sf::Vector2f>
     */
    std::vector<RenderObject> getRenderBullets();
    ~Window();
};


/// @brief Enum that represents differen types of textures needed
enum TextureType
{
    PlayerTank=0,
    BasicTank,
    FastTank,
    PowerTank,
    ArmorTank,
    Bricks,
    Steel,
    Water,
    Trees,
    NullTile,
    Bullet
};


/// \brief Sturct that should be given to BoardView for rendering objects
struct RenderObject
{
    sf::Vector2f coords;
    TextureType textureType;
};



#endif //PROI_PROJKET_GRAPHIC_H