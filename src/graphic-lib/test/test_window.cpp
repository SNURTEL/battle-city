#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include "../include/Window.h"
#include "../../game-lib/include/GameState.h"
#include "../../tank-lib/include/Tank.h"
#include "../../tank-lib/include/Bullet.h"
#include "../../board-lib/include/Grid.h"
#include "../../game-lib/include/Game.h"
#include <unordered_map>
#include "../include/AbstractWindow.h"
#include "../include/ActiveState_dir/ActiveStateGraphic.h"
#include "../include/ActiveState_dir/Borad_dir/BoardGraphic.h"
#include "../include/ActiveState_dir/Borad_dir/TanksGraphic.h"
#include "../include/ActiveState_dir/Frame_dir/FrameGraphic.h"
#include "testClasses.cpp"




SCENARIO("Game states are changing")
{
    GIVEN("A test window and game states")
    {
        Game* game = nullptr;
        ActiveGameState state(game);
        GameState* gameState = &state;
        TestWindow::ActiveStatePointers activePointers;
        TestWindow window(gameState, activePointers);

        WHEN("Window is created and the game state is ActiveGameState")
        {
            THEN("Window attribute gameState should be set to ActiveGameState")
            {
                REQUIRE(window.get_gameState() == TestWindow::ActieveGameState);
            }
        }

        WHEN("When game state changes to MenuGameState")
        {
            THEN("Window attribute gameState should be set to MenuGameState")
            {}
            // I will write test, after getting the code with states
        }
    }
}


SCENARIO("During the game tanks number, tiles and other game describing factor changes")
{
    GIVEN("TestWindow, GameState, std::vector<Tanks*>, std::vector<Bullets*>, Grid, int")
    {
        Game* game_ptr = nullptr;
        Game game(60);  // Initialize to instatiate an eventque it is neede for Grid to work
        game_ptr = &game;
        ActiveGameState state(game_ptr);
        GameState* gameState = &state;
        TestWindow::ActiveStatePointers activePointers;
        // Creating tanks
        TestTank tank1;
        TestTank tank2;

        std::vector<Tank*> tanks;
        tanks.push_back(&tank1);
        tanks.push_back(&tank2);

        // Creating bullets

        TestBullet bullet1;
        TestBullet bullet2;
        std::vector<Bullet*> bullets;
        bullets.push_back(&bullet1);
        bullets.push_back(&bullet2);


        Grid grid;
        Grid* gridPointer = &grid;
        int level = 2;
        int playerLives = 3;
        activePointers.bullets = &bullets;
        activePointers.tanks = &tanks;
        activePointers.tiles = gridPointer;
        activePointers.level = &level;
        activePointers.playerLivesLeft = &playerLives;

        TestWindow window(gameState, activePointers);

        WHEN("Some objects that composite has pointers to change")
        {
            playerLives = 20;
            level = 3;
            TestTank tank3;
            tanks.push_back(&tank3);
            std::vector<Tank*>* tanks_composite = window.getTanks();
            ActiveStateGraphic::BoardPointers boardPointers_composite = window.getBoardPointers();
            ActiveStateGraphic::FramePointers framePointers_composite = window.getFramePointers();

            THEN("Tanks inside the composit change too")
            {
                REQUIRE(&tanks == tanks_composite);
                REQUIRE(boardPointers_composite.tanks == tanks_composite);
            }
            THEN("Attribute level inside the compoiste changes too")
            {
                REQUIRE(level == *framePointers_composite.level);
            }
            THEN("Attirbute playerLives inside the composite changes too")
            {
                REQUIRE(playerLives == *framePointers_composite.playerLivesLeft);
            }
        }

    }
}
