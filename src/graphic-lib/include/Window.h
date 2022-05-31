#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "AbstractWindow.h"
#include <unordered_map>
#include "../../tank-lib/include/Entity.h"
#include "../../board-lib/include/Grid.h"
// #include "ActiveState_dir/ActiveStateGraphic.h"


#ifndef PROI_PROJEKT_GRAPHIC_H
#define PROI_PROJEKT_GRAPHIC_H

class Tank;
class GameState;
class Bullet;
class Grid;


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
class Window : public AbstractWindow
{
public:


    /// @brief Stores given ActiveState pointers
    struct ActiveStatePointers
    {
        std::shared_ptr<std::vector<Tank*>> tanks;
        std::shared_ptr<std::vector<Bullet*>> bullets;
        std::shared_ptr<Grid*> tiles;
        std::shared_ptr<int> level;
        std::shared_ptr<int*> playerLivesLeft;
        // And others ...
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
     * @brief Initiates active state pointers as shared pointers
     *
     */
    void initiateActiveStatePointers();


    /**
     * @brief Fetches pointers for ActiveState objects
     *
     * Fetch poitners fo ActiveStateGraphic children and store it in window data structure
     *
     * @param tiles
     */
    void fetchMenuStatePointers(int selected); // Only a concept

    /// @brief Cheks wich game state was given to constructor and sets appropriate attribute
    void selectgameState();

    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render() override;

    /// @brief Adding Entity to the list of tanks or bullets
    void addEntity(Entity* e);

    // /// @brief Changing properties of given Entity
    // void moveEntity(Entity* e);

    /// @brief Removing Entity from the tanks or bullets list
    void removeEntity(Entity* e);

    // /// @brief Changing tile type of the given tile
    // void changeTile(uint x, uint y, TileType tileType);

    /// @brief Loading next level
    void loadLevel(Grid* grid, int levelNumber);

    /**
     * @brief Cheks given Entity type
     *
     * Returns "tank" if it is a tank or "bullet" if it is a bullets
     *
     * @param e
     * @return std::string
     */
    std::string checkEntityType(Entity* e);

    // /**
    //  * @brief Commands its children to update objects to render
    //  *
    //  */
    // virtual void update();

    /// @brief Enum that determine actual game state
    enum GameStateGraphic
    {
        ActieveGameState=0,
        PauseGameState,
        MenuGameState,
        FinishedGameState
    };


    // virtual ~Window();
protected:

    /**
     * @brief Unique pointer for sf:RenderWindow
     *
     * Made for safe memory allocation
     */
    std::unique_ptr<sf::RenderWindow> window;


    /// @brief Create appropriate children
    void conscructComposit();


    GameStateGraphic gameState;

    /// @brief Map that contains Window children
    std::unordered_map<GameStateGraphic, std::shared_ptr<AbstractWindow>> children_map{};

    sf::VideoMode videoMode;

    /// @brief Cheks which game state was given to constructor and sets appropriate attribute
    void selectgameState(GameState* gameState);

    /// @brief Cheks if given objects is type of declared object
    template<typename State, typename T>
    bool instanceOf(T* ptr);


    ActiveStatePointers activeStatePointers;

};

#endif //PROI_PROJKET_GRAPHIC_H