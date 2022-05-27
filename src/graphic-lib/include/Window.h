#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "AbstractWindow.h"


#ifndef PROI_PROJEKT_GRAPHIC_H
#define PROI_PROJEKT_GRAPHIC_H

class Tank;
class GameState;
class Bullet;


/**
 * @brief Contains main SFML functions
 *
 * Class that summars main graphic-lib functions.
 * The root of composit
 * It contains vector of pointers to its children
 * Resposible for updating and rendering board and its objects.
 * Creates sf::RenderWindow
 * Window size is 800/800
 */
class Window : AbstractWindow
{
private:

    /**
     * @brief Unique poiter for sf:RenderWindow
     *
     * Made for safe memory allocation
     */
    std::unique_ptr<sf::RenderWindow> window_unique;


    /// @brief Enum that determine actual game state
    enum GameStateGraphic
    {
        ActieveGameState=0,
        PauseGameState,
        MenuGameState,
        FinishedGameState
    };

    GameStateGraphic gameState;


    /// @brief Cheks wich game state was given to constructor and sets appropriate attribute
    void selectgameState();


public:


    /// @brief Stores given ActiveState pointers
    struct ActiveStatePointers
    {
        std::vector<Tank*> tanks;
        std::vector<Bullet*> bullets;
        int tiles[52][52];
        int level;
        int playerLivesLeft;
        // And other ...
        // Will be added later
    };

    /// @brief Stores MenuState pointers
    struct MenuStatePointers
    {
        // Will be added later
    };

    /**
     * @brief Construct a new Window object
     *
     * Intiate window with apropriate size and mode
     * Gets actual gameState, sets gameState attribute
     *
     */
    Window(GameState* gameState);


    /**
     * @brief Fetches pointers for ActiveState objects
     *
     * Fetch poitners fo ActiveStateGraphic children and store it in window data structure
     *
     * @param tiles
     */
    void fetchAcitveStatePointers(std::vector<Tank*>/*, int *tiles[][]*/);


    /**
     * @brief Fetches pointers for ActiveState objects
     *
     * Fetch poitners fo ActiveStateGraphic children and store it in window data structure
     *
     * @param tiles
     */
    void fetchMenuStatePointers(int selected); // Only a concept


    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render() const;


    /**
     * @brief Commands its children to update objects to render
     *
     */
    virtual void update();


    virtual ~Window();
};

#endif //PROI_PROJKET_GRAPHIC_H