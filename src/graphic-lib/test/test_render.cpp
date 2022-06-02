#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../include/ActiveState_dir/Frame_dir/FrameGraphic.h"
#include "testClasses.cpp"
#include "../../board-lib/include/Grid.h"
#include "../../tank-lib/include/Tank.h"

SCENARIO("Rendering bare frame with tiles")
{
    GIVEN("sf::RednerWindow, FrameGraphic, sf::View, Grid")
    {
        // sf::RenderWindow window(sf::VideoMode(700, 700), "Frame", sf::Style::Default);
        // sf::View view = window.getDefaultView();
        Game game(60);
        ActiveGameState activeGameState(&game);
        GameState* gameStatePointer = &activeGameState;
        Window::ActiveStatePointers activeStatePointer;
        int level = 2;
        int playerLIvesLeft = 5;
        activeStatePointer.level = &level;
        activeStatePointer.playerLivesLeft = &playerLIvesLeft;

        // Setting grid
        Grid grid; // While grid() is not working???

        for (int i = 0; i < 52; i++)
        {
            for (int j = 0; j < 52; j+= 4)
            {
                grid.setTile(j, i, TileType::Bricks);
            }

            for (int j = 0; j < 52; j += 9)
            {
                grid.setTile(j, i, TileType::Trees);
            }

        }

        grid.setTile(0, 0, TileType::Water);

        activeStatePointer.tiles = &grid;
        // Tank::TankType type = Tank::ArmorTank;
        TestTank tank1;
        TestTank tank2;
        tank2.setX(400);
        tank2.setY(300);
        std::vector<Tank*> tanks;
        tanks.push_back(&tank1);
        tanks.push_back(&tank2);
        activeStatePointer.tanks = &tanks;

        TestBullet bullet1;
        TestBullet bullet2;
        // bullet2.
        std::vector<Bullet*> bullets;
        bullets.push_back(&bullet1);
        bullets.push_back(&bullet2);
        activeStatePointer.bullets = &bullets;

        TestWindow window(gameStatePointer, activeStatePointer);
        // BoardGraphic BoardGraphic(WindowView(&window))

        // FrameGraphic frameGraphic(WindowView(&window, view), activeStatePointer);
        sf::RenderWindow *renderWindow = window.getWindowView().window;
        THEN("Render board")
        {

            while (true)
            {
                renderWindow->clear(sf::Color::Black);
                window.render();
                renderWindow->display();

            }
        }
    }
}
