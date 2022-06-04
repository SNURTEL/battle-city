#include "include/ActiveState_dir/Frame_dir/GameInfoGraphic.h"
#include <iostream>
#include <string>
#include <sstream>

GameInfoGraphic::GameInfoGraphic(const WindowView& windowView, const FrameGraphic::GameInfo& gameInfo)
: AbstractWindow(windowView), gameInfo(gameInfo)
{loadFont();}


void GameInfoGraphic::render()
{
    float offsetLeft = windowView.leftOffset;
    float offsetTop = windowView.topOffset;
    std::stringstream ss;
    ss << "Player's lives:" << "\n\t\t   " << std::to_string(*gameInfo.playerLives);
    std::string renderText = ss.str();
    this->renderText(renderText, offsetLeft, offsetTop);

    // To clear ss
    ss.clear();
    ss.str("");

    ss << "   \tLevel:" << "\n   \t\t" << std::to_string(*gameInfo.level);
    renderText = ss.str();
    offsetTop += 100;
    this->renderText(renderText, offsetLeft, offsetTop);
}


void GameInfoGraphic::renderText(std::string renderText, float leftOffset, float topOffset)
{
    sf::Text text;
    text.setFont(font);
    text.setString(renderText);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color::Black);
    text.setPosition(leftOffset, topOffset);
    windowView.window->draw(text);
}


void GameInfoGraphic::loadFont()
{
    if (!font.loadFromFile("../src/graphic-lib/fontFolder/CharisSIL-Regular.ttf"))
        ;
        // Exception
}