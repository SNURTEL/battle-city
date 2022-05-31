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
        TestWindow window(gameState);

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



