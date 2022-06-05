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
        GraphicEventHandler graphicEventHandler(game_ptr, &window);
        window.selectgameState(gameState);
        // Initiating Entities
        TestTank tank1;
        Entity* eTank = &tank1;

        TestBullet bullet;
        Entity* eBullet = &bullet;

        // Initiating Grid
        Grid grid;

        grid.setTile(0, 0, TileType::Bricks);

        // Initiating level and playerLivesLeft

        int level = 5;
        int playerLives = 5;
        GameStatistics gameStats(playerLives, level, 5);

        // Creating Menu

        Menu menu(2);

        // Creating Events

        Event tankSpawned(Event::EntitySpawned, eTank);
        Event bulletSpawned(Event::EntitySpawned, eBullet);
        Event levelLoaded(Event::LevelLoaded, level, &grid);
        Event statisticsChanged(Event::StatisticsChanged, &gameStats);
        Event menuSelectionChanged(Event::MenuSelectionChange, &menu, 2);

        WHEN("Handling given events")
        {
            graphicEventHandler.processEvent(std::make_unique<Event>(tankSpawned));
            graphicEventHandler.processEvent(std::make_unique<Event>(bulletSpawned));
            graphicEventHandler.processEvent(std::make_unique<Event>(levelLoaded));
            graphicEventHandler.processEvent(std::make_unique<Event>(statisticsChanged));
            graphicEventHandler.processEvent(std::make_unique<Event>(menuSelectionChanged));

            THEN("Composite sturcture should be updated")
            {
                std::vector<Tank*>* tanksComposite = window.getTanks();
                std::vector<Bullet*>* bulletsComposite = window.getBullets();
                Grid** gridComposite = window.getGrid();
                ActiveStateGraphic::BoardPointers boardPointers = window.getBoardPointers();
                ActiveStateGraphic::FramePointers framePointers = window.getFramePointers();
                Window::StaticStatePointers staticPointers = window.getStaticPointersLeaf();

                REQUIRE((*tanksComposite)[0] == &tank1);
                REQUIRE((*bulletsComposite)[0] == &bullet);
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
                Entity* eBullet2 = &bullet2;
                Event tankMoved(Event::EntityMoved, eTank);
                Event bulletSpawned(Event::EntitySpawned, eBullet2);

                WHEN("Handling events and changing grid")
                {
                    // Grid changes
                    grid.setTile(5, 5, TileType::Steel);

                    graphicEventHandler.processEvent(std::make_unique<Event>(tankMoved));
                    graphicEventHandler.processEvent(std::make_unique<Event>(bulletSpawned));
                    THEN("The composite structure should be updated")
                    {
                        std::vector<Tank*>* tanksComposite = window.getTanks();
                        std::vector<Bullet*>* bulletsComposite = window.getBullets();
                        Grid** gridComposite = window.getGrid();
                        ActiveStateGraphic::BoardPointers boardPointers = window.getBoardPointers();
                        ActiveStateGraphic::FramePointers framePointers = window.getFramePointers();
                        REQUIRE((*(*tanksComposite)[0]).getX() == tank1.getX());
                        REQUIRE((*bulletsComposite)[0] == &bullet);
                        REQUIRE((*bulletsComposite)[1] == &bullet2);
                        REQUIRE((*gridComposite)->getTileAtPosition(5, 5) == TileType::Steel);
                        REQUIRE(*framePointers.level == level);
                        REQUIRE(*framePointers.playerLives == playerLives);
                    }


                    GIVEN("RemoveEntity event")
                    {
                        Event tankRemoved(Event::EntityRemoved, eTank);
                        Event bulletRemoved(Event::EntityRemoved, eBullet);
                        WHEN("Handling RemoveEntity event")
                        {
                            graphicEventHandler.processEvent(std::make_unique<Event>(tankRemoved));
                            graphicEventHandler.processEvent(std::make_unique<Event>(bulletRemoved));

                            THEN("The composite structure should be updated")
                            {
                                std::vector<Tank*>* tanksComposite = window.getTanks();
                                std::vector<Bullet*>* bulletsComposite = window.getBullets();
                                Grid** gridComposite = window.getGrid();
                                REQUIRE((*tanksComposite).size() == 0);
                                REQUIRE((*bulletsComposite)[0] == &bullet2);
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
        GraphicEventHandler graphicEventHandler(game_ptr, window_ptr);

        Event activeStateEvent(Event::StateChanged, gameState);
        graphicEventHandler.processEvent(std::make_unique<Event>(activeStateEvent));

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
            Event pauseStateEvent(Event::StateChanged, gameState);
            graphicEventHandler.processEvent(std::make_unique<Event>(pauseStateEvent));
            THEN("Window attribute gameState should be set to MenuGameState")
            {
                REQUIRE(window.get_gameState() == TestWindow::PauseGameState);
            }
        }
    }
}