#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include "../include/Window.h"
#include "../../tank-lib/include/Tank.h"
#include "../../tank-lib/include/Bullet.h"
#include "../../board-lib/include/Grid.h"
#include <unordered_map>
#include "../include/ActiveState_dir/Borad_dir/TanksGraphic.h"
#include "../include/GraphicEventHandler.h"
#include "testClasses.cpp"
#include "../../game-lib/include/GameStatistics.h"
#include "../../game-lib/include/Menu.h"
#include "../include/GraphicEventHandler.h"



SCENARIO("Handling events connected to graphic-lib")
{
    GIVEN("TestWindow, GameState, tanks, bullets, Grid, GraphicEventHandler, Events, GameStatistics")
    {

        // Initiating window, states, game and handler
        Game* game_ptr = nullptr;
        Game game(60);  // Initialize to instatiate an eventque it is neede for Grid to work
        game_ptr = &game;
        ActiveGameState state(game_ptr);
        GameState* gameState = &state;
        TestWindow window;
        GraphicEventHandler graphicEventHandler(&window);
        window.selectgameState(gameState);

        // Initiating Entities
        TestTank tank1;
        std::shared_ptr<Entity> eTank = std::make_shared<Tank>(tank1);

        TestBullet bullet;
        std::shared_ptr<Entity> eBullet = std::make_shared<Bullet>(bullet);

        // Initiating Grid
        Grid grid;

        grid.setTile(0, 0, TileType::Bricks);

        // Initiating level and playerLivesLeft

        int level = 5;
        int playerLives = 5;
        GameStatistics gameStats(playerLives, level, 5);

        // Creating Menu

        Menu menu(2);


        WHEN("Handling given events")
        {
            graphicEventHandler.processEvent(std::make_unique<Event>(Event::EntitySpawned, eTank));
            graphicEventHandler.processEvent(std::make_unique<Event>(Event::EntitySpawned, eBullet));
            graphicEventHandler.processEvent(std::make_unique<Event>(Event::LevelLoaded, level, &grid));
            graphicEventHandler.processEvent(std::make_unique<Event>(Event::StatisticsChanged, &gameStats));
            graphicEventHandler.processEvent(std::make_unique<Event>(Event::MenuSelectionChange, &menu, 2));

            THEN("Composite sturcture should be updated")
            {
                std::vector<std::shared_ptr<Tank>>* tanksComposite = window.getTanks();
                std::vector<std::shared_ptr<Bullet>>* bulletsComposite = window.getBullets();
                Grid** gridComposite = window.getGrid();
                ActiveStateGraphic::BoardPointers boardPointers = window.getBoardPointers();
                ActiveStateGraphic::FramePointers framePointers = window.getFramePointers();
                Window::StaticStatePointers staticPointers = window.getStaticPointersLeaf();

                REQUIRE((*tanksComposite)[0] == eTank);
                REQUIRE((*bulletsComposite)[0] == eBullet);
                REQUIRE((*gridComposite) == &grid);
                REQUIRE(*framePointers.level == level);
                REQUIRE(*framePointers.playerLives == playerLives);
                REQUIRE(*staticPointers.menuPos == 2);
                REQUIRE(*staticPointers.points == 5);
            }

            GIVEN("More Events")
            {
                // Creating more Events

                tank1.setX(200);
                TestBullet bullet2;
                std::shared_ptr<Bullet> eBullet2 = std::make_shared<Bullet>(bullet2);
                // Event tankMoved(Event::EntityMoved, eTank);
                // Event bulletSpawned(Event::EntitySpawned, eBullet2);

                WHEN("Handling events and changing grid")
                {
                    // Grid changes
                    grid.setTile(5, 5, TileType::Steel);

                    graphicEventHandler.processEvent(std::make_unique<Event>(Event::EntityMoved, eTank));
                    graphicEventHandler.processEvent(std::make_unique<Event>(Event::EntitySpawned, eBullet2));
                    THEN("The composite structure should be updated")
                    {
                        std::vector<std::shared_ptr<Tank>>* tanksComposite = window.getTanks();
                        std::vector<std::shared_ptr<Bullet>>* bulletsComposite = window.getBullets();
                        Grid** gridComposite = window.getGrid();
                        ActiveStateGraphic::BoardPointers boardPointers = window.getBoardPointers();
                        ActiveStateGraphic::FramePointers framePointers = window.getFramePointers();
                        REQUIRE((*(*tanksComposite)[0]).getX() == eTank->getX());
                        REQUIRE((*bulletsComposite)[0] == eBullet);
                        REQUIRE((*bulletsComposite)[1] == eBullet2);
                        REQUIRE((*gridComposite)->getTileAtPosition(5, 5) == TileType::Steel);
                        REQUIRE(*framePointers.level == level);
                        REQUIRE(*framePointers.playerLives == playerLives);
                    }


                    GIVEN("RemoveEntity event")
                    {
                        WHEN("Handling RemoveEntity event")
                        {
                            graphicEventHandler.processEvent(std::make_unique<Event>(Event::EntityRemoved, eTank));
                            graphicEventHandler.processEvent(std::make_unique<Event>(Event::EntityRemoved, eBullet));

                            THEN("The composite structure should be updated")
                            {
                                std::vector<std::shared_ptr<Tank>>* tanksComposite = window.getTanks();
                                std::vector<std::shared_ptr<Bullet>>* bulletsComposite = window.getBullets();
                                Grid** gridComposite = window.getGrid();
                                REQUIRE((*tanksComposite).size() == 0);
                                REQUIRE((*bulletsComposite)[0] == eBullet2);
                            }
                        }
                    }
                }
            }
        }
    }
}



SCENARIO("Game states are changing")
{
    GIVEN("A test window , game states, graphic event handler, game object")
    {
        // Initiating window, states, game and handler
        Game* game_ptr;
        Game game(60);
        game_ptr = &game;
        ActiveGameState state(game_ptr);
        GameState* gameState = &state;
        TestWindow window;
        Window* window_ptr;
        window_ptr = &window;
        GraphicEventHandler graphicEventHandler(window_ptr);

        graphicEventHandler.processEvent(std::make_unique<Event>(Event::StateChanged, gameState));

        WHEN("Window is created and the game state is ActiveGameState")
        {
            THEN("Window attribute gameState should be set to ActiveGameState")
            {
                REQUIRE(window.get_gameState() == TestWindow::ActieveGameState);
            }
        }

        WHEN("When game state changes to MenuGameState")
        {
            PauseGameState pauseGameState(game_ptr);
            gameState = &pauseGameState;
            graphicEventHandler.processEvent(std::make_unique<Event>(Event::StateChanged, gameState));
            THEN("Window attribute gameState should be set to MenuGameState")
            {
                REQUIRE(window.get_gameState() == TestWindow::PauseGameState);
            }
        }
    }
}