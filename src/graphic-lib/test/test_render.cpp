#include "catch2/catch_all.hpp"
#include "catch2/catch_test_macros.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../include/ActiveState_dir/Frame_dir/FrameGraphic.h"
#include "testClasses.cpp"


SCENARIO("Rendering bare frame")
{
    GIVEN("sf::RednerWindow, FrameGraphic, sf::View")
    {
        sf::RenderWindow window(sf::VideoMode(700, 700), "Frame", sf::Style::Default);
        sf::View view = window.getDefaultView();
        ActiveStateGraphic::FramePointers frameObjects;
        int level = 2;
        int playerLIvesLeft = 5;
        frameObjects.level = &level;
        frameObjects.playerLivesLeft = &playerLIvesLeft;
        FrameGraphic frameGraphic(WindowView(&window, view), frameObjects);
        THEN("Render board")
        {

            while (true)
            {
                window.clear(sf::Color::Green);
                frameGraphic.render();
                window.display();

            }


        }

    }

}
