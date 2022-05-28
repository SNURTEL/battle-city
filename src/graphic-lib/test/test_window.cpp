#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include "../include/Window.h"
#include "../../game-lib/include/GameState.h"


class TestWindow : public Window
{
public:
    TestWindow(GameState* gameState): Window(gameState)
    {};

    GameStateGraphic get_gameState()
    {return gameState;};
};




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


SCENARIO("During the game tanks number, tiles and other game describing factor changes")
{
    GIVEN("TestWindow, GameState, std::vector<Tanks*>, std::vector<Bullets*>, Grid, int")
    {
        Game* game = nullptr;
        ActiveGameState state(game);
        GameState* gameState = &state;
        TestWindow window(gameState);
        
    }
}
