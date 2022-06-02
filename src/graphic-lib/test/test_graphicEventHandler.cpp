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



SCENARIO("Handling events connected to graphic-lib")
{
    GIVEN("TestWindow, GameState, tanks, bullets, Grid, GraphicEventHandler, Events")
    {

        // Initiating window, states, game and handler
        Game* game_ptr = nullptr;
        Game game(60);  // Initialize to instatiate an eventque it is neede for Grid to work
        game_ptr = &game;
        ActiveGameState state(game_ptr);
        GameState* gameState = &state;
        TestWindow window(gameState);
        GraphicEventHandler graphicEventHandler(game_ptr, &window);

        // Initiating Entities
        TestTank tank1;
        Entity* eTank = &tank1;

        TestBullet bullet;
        Entity* eBullet = &bullet;

        // Initiating Grid
        Grid grid;

        grid.setTile(0, 0, TileType::Bricks);

        // Initiating level and playerLIvesLeft

        int level = 5;
        int playersLivesLeft = 5;

        // Creating Events

        Event tankSpawned(Event::EntitySpawned, eTank);
        Event bulletSpawned(Event::EntitySpawned, eBullet);
        Event levelLoaded(Event::LevelLoaded, level, &grid);

        WHEN("Handling given events")
        {
            graphicEventHandler.processEvent(std::make_unique<Event>(tankSpawned));
            graphicEventHandler.processEvent(std::make_unique<Event>(bulletSpawned));
            graphicEventHandler.processEvent(std::make_unique<Event>(levelLoaded));

            THEN("Composite sturcture should be updated")
            {
                std::vector<Tank*>* tanksComposite = window.getTanks();
                std::vector<Bullet*>* bulletsComposite = window.getBullets();
                Grid** gridComposite = window.getGrid();
                REQUIRE((*tanksComposite)[0] == &tank1);
                REQUIRE((*bulletsComposite)[0] == &bullet);
                REQUIRE((*gridComposite) == &grid);
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
                    THEN("Composite structure should be updated")
                    {
                        std::vector<Tank*>* tanksComposite = window.getTanks();
                        std::vector<Bullet*>* bulletsComposite = window.getBullets();
                        Grid** gridComposite = window.getGrid();
                        REQUIRE((*(*tanksComposite)[0]).getX() == tank1.getX());
                        REQUIRE((*bulletsComposite)[0] == &bullet);
                        REQUIRE((*bulletsComposite)[1] == &bullet2);
                        REQUIRE((*gridComposite)->getTileAtPosition(5, 5) == TileType::Steel);
                    }


                    GIVEN("RemoveEntity event")
                    {
                        Event tankRemoved(Event::EntityRemoved, eTank);
                        Event bulletRemoved(Event::EntityRemoved, eBullet);
                        WHEN("Handling RemoveEntity event")
                        {
                            graphicEventHandler.processEvent(std::make_unique<Event>(tankRemoved));
                            graphicEventHandler.processEvent(std::make_unique<Event>(bulletRemoved));

                            THEN("Composite structure should be updated")
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
