// Created by Michał


#ifndef PROI_PROJEKT_GRAPHIC_H
#define PROI_PROJEKT_GRAPHIC_H

#include "AbstractWindow.h"
#include <unordered_map>
#include "../../tank-lib/include/Entity.h"
#include "../../board-lib/include/Grid.h"
#include "../../tank-lib/include/Tank.h"
#include "../../tank-lib/include/Bullet.h"
#include "../../game-lib/include/GameState.h"
// #include "StaticStates_dir/StaticGraphic.h"


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

    /// @brief Enum that determine actual game state
    enum GameStateGraphic
    {
        ActieveGameState=0,
        PauseGameState,
        MenuGameState,
        FinishedGameState,
        StaticStates
    };


    /// @brief Stores given ActiveState pointers
    struct ActiveStatePointers
    {
        std::shared_ptr<sf::Vector2f> eaglePos;
        std::shared_ptr<std::vector<std::shared_ptr<Tank>>> tanks;
        std::shared_ptr<std::vector<std::shared_ptr<Bullet>>> bullets;
        std::shared_ptr<Grid*> tiles;
        std::shared_ptr<int> level;
        std::shared_ptr<int> playerLives;
        std::shared_ptr<int> points;
    };

    /// @brief Stores MenuState pointers
    struct StaticStatePointers
    {
        std::shared_ptr<Window::GameStateGraphic*> gameState;
        std::shared_ptr<int> points;
        std::shared_ptr<int> menuPos;
    };



    /**
     * @brief Construct a new Window object
     *
     * Intiate window with apropriate size and mode
     */
    Window();


    /// @brief Cheks which game state was given and sets appropriate attribute
    void selectgameState(GameState* gmaeState);


    /**
     * @brief Initiates active state pointers as shared pointers
     *
     */
    void initiateActiveStatePointers();


    /**
     * @brief Initiates static state pointers as shared pointers
     *
     */
    void initateStaticStatePointers();


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
    virtual void render() override;


    /// @brief Adding Entity to the list of tanks or bullets
    void addEntity(std::shared_ptr<Entity> e);


    /// @brief Removing Entity from the tanks or bullets list
    void removeEntity(std::shared_ptr<Entity> e);


    /// @brief Loading next level
    void loadLevel(Grid* grid, int levelNumber, std::pair<uint, uint> eaglePos);


    /// @brief Loads player stats
    void loadStats(int playerLives, int points);


    /// @brief Loads curren menu position selected
    void changeMenuPos(uint menuPos);


    /**
     * @brief Cheks given Entity type
     *
     * Returns "tank" if it is a tank or "bullet" if it is a bullets
     *
     * @param e
     * @return std::string
     */
    std::string checkEntityType(std::shared_ptr<Entity> e);


protected:

    /**
     * @brief Unique pointer for sf:RenderWindow
     *
     * Made for safe memory allocation
     */
    std::unique_ptr<sf::RenderWindow> window;


    /// @brief Create appropriate children
    void conscructComposit();


    GameStateGraphic gameState{};


    /// @brief Map that contains Window childrens
    std::unordered_map<GameStateGraphic, std::shared_ptr<AbstractWindow>> children_map{};


    sf::VideoMode videoMode;


    /// @brief Cheks if given objects is type of declared object
    template<typename State, typename T>
    bool instanceOf(T* ptr);


    ActiveStatePointers activeStatePointers;

    StaticStatePointers staticStatesPointers;

};

#endif //PROI_PROJKET_GRAPHIC_H