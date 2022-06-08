#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../include/ActiveState_dir/Frame_dir/FrameGraphic.h"
#include "testClasses.cpp"
#include "../../board-lib/include/Grid.h"
#include "../../tank-lib/include/Tank.h"
#include "../../board-lib/include/GridBuilder.h"


// Showing rendered game on the screen


// SCENARIO("Rendering active staet board")
// {
//     GIVEN("sf::RednerWindow, FrameGraphic, sf::View, Grid")
//     {
//         // sf::RenderWindow window(sf::VideoMode(700, 700), "Frame", sf::Style::Default);
//         // sf::View view = window.getDefaultView();
//         Game game(60);
//         ActiveGameState activeGameState(&game);
//         GameState* gameStatePointer = &activeGameState;
//         Window::ActiveStatePointers activeStatePointer;

//         // Setting grid
//         Grid grid; // Why grid() is not working???

//         for (int i = 0; i < 26; i++)
//         {
//             for (int j = 0; j < 52; j++)
//             {
//                 grid.setTile(j, i, TileType::Steel);
//             }
//         }

//         for (int i = 45; i < 52; i++)
//         {
//             for (int j = 0; j < 52; j++)
//             {
//                 grid.setTile(j, i, TileType::Trees);
//             }
//         }

//         for (int j = 0; j < 52; j++)
//         {
//             grid.setTile(j, 40, TileType::Bricks);
//         }

//         grid.setTile(33, 33, TileType::Water);
//         grid.setTile(34, 34, TileType::Water);
//         grid.setTile(33, 34, TileType::Water);
//         grid.setTile(34, 33, TileType::Water);

//         grid.setTile(31, 31, TileType::Bricks);

//         // grid.setTile(0, 0, TileType::Water);


//         // Tank::TankType type = Tank::ArmorTank;

//         std::unique_ptr<Grid> grid_ptr = GridBuilder::buildLevel(1);


//         TestTank tank1;
//         TestTank tank2;
//         TestTankCustom playerTank;
//         tank1.setFacing(Direction::South);
//         tank1.setX(40);
//         playerTank.setX(200);
//         playerTank.setY(200);
//         tank2.setX(400);
//         tank2.setY(420);
//         std::shared_ptr<Entity> tank1_ptr = std::make_shared<TestTank>(tank1);
//         std::shared_ptr<Entity> tank2_ptr = std::make_shared<TestTank>(tank2);
//         std::shared_ptr<Entity> playerTank_ptr = std::make_shared<TestTankCustom>(playerTank);



//         TestBullet bullet1;
//         TestBullet bullet2;

//         std::shared_ptr<Entity> bullet1_ptr = std::make_shared<TestBullet>(bullet1);
//         std::shared_ptr<Entity> bullet2_ptr = std::make_shared<TestBullet>(bullet2);


//         TestWindow window;

//         window.selectgameState(gameStatePointer);
//         window.addEntity(tank1_ptr);
//         window.addEntity(tank2_ptr);
//         window.addEntity(playerTank_ptr);
//         window.addEntity(bullet1_ptr);
//         window.addEntity(bullet2_ptr);
//         window.loadLevel(grid_ptr.get(), 1);
//         window.loadStats(5, 5);
//         window.changeMenuPos(1);

//         // // State changed
//         // FinishedGameState menuGameState(&game);
//         // gameStatePointer = &menuGameState;
//         window.selectgameState(gameStatePointer);



//         sf::RenderWindow *renderWindow = window.getWindowView().window;
//         THEN("Render board")
//         {

//             while (true)
//             {
//                 renderWindow->clear(sf::Color::Black);
//                 window.render();
//                 renderWindow->display();

//             }
//         }
//     }
// }
