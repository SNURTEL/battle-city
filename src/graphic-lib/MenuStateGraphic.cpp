#include "include/StaticStates_dir/MenuStateGraphic.h"
#include "include/TextureLoadingError.h"
#include <sstream>
#include "include/Window.h"


void MenuStateGraphic::loadFont()
{
    std::string path = "../src/graphic-lib/fontFolder/CharisSIL-Regular.ttf";
    if (!font.loadFromFile(path))
    {
        std::stringstream ss;
        ss << "Failed to load font: " << path;
        throw TextureLoadingError(ss.str());
    }
}


void MenuStateGraphic::renderText(std::string renderText, float leftOffset, float topOffset,
                                  float fontSize, sf::Color color)
{
    sf::Text text;
    text.setFont(font);
    text.setString(renderText);
    text.setCharacterSize(fontSize);
    text.setFillColor(color);
    text.setPosition(leftOffset, topOffset);
    windowView.window->draw(text);
}


MenuStateGraphic::MenuStateGraphic(const WindowView& windowView, const StaticGraphic::ButtonsPositionMenu& buttonPos,
                                   const Window::StaticStatePointers& staticPointers)
: AbstractWindow(windowView), buttonPos(buttonPos), staticPointers(staticPointers), color(sf::Color(153, 153, 153))
{loadFont();}


void MenuStateGraphic::render()
{
    initiatePointsStateHeader(); // Amount of points might vary every render sesion

    Window::GameStateGraphic gameState = *(*staticPointers.gameState);

    renderHeader(gameState);

    renderButtons(gameState);
}


void MenuStateGraphic::initiatePointsStateHeader()
{
    std::stringstream ss;
    ss << "\t\t\t   " << "PLAYER's POINTS: " << std::to_string(*staticPointers.points);
    pauseStateHeader.text2 = ss.str();

    ss.clear();
    ss.str("");
    std::string result = (*staticPointers.gameWon) ? "YOU WON!" : "YOU LOST";
    ss << "\t\t\t\t\t\t" << result << "\n"
       << "\t\t\t   " << "PLAYER'S POINTS: " << std::to_string(*staticPointers.points);
    finishStateHeader.text2 = ss.str();
}


void MenuStateGraphic::renderHeader(Window::GameStateGraphic gameState)
{
    Text textHeader;
    if (gameState == Window::GameStateGraphic::MenuGameState)
        textHeader = menuStateHeader;
    else if (gameState == Window::GameStateGraphic::PauseGameState)
        textHeader = pauseStateHeader;
    else
        textHeader = finishStateHeader;

    float leftOffset = windowView.leftOffset;
    float topOffset = windowView.topOffset;
    float fontSize = 60;
    sf::Color color(sf::Color::White);

    renderText(textHeader.text1, leftOffset+15, topOffset, fontSize, color);

    topOffset += 80;
    fontSize = 25;

    renderText(textHeader.text2, leftOffset, topOffset, fontSize, color);
}


void MenuStateGraphic::renderButtons(Window::GameStateGraphic gameState)
{
    float leftOffset = windowView.leftOffset;
    float topOffset = windowView.topOffset;
    Text textButtons;
    if (gameState == Window::GameStateGraphic::MenuGameState)
        textButtons = menuStateText;
    else if (gameState == Window::GameStateGraphic::PauseGameState)
        textButtons = pauseStateText;
    else
        textButtons = finishStateText;

    uint selection = *staticPointers.menuPos;

    sf::Color color1(sf::Color::White);
    sf::Color color2(sf::Color::White);


   if (selection != 1 && gameState != Window::GameStateGraphic::FinishedGameState)
   {
       color1 = this->color;
   }
   else if (gameState != Window::GameStateGraphic::FinishedGameState)
   {
       color2 = this->color;
   }


    sf::RectangleShape button(buttonPos.button1Size);
    button.setPosition(buttonPos.button1Pos);
    button.setFillColor(color1);
    windowView.window->draw(button);

    if (gameState != Window::GameStateGraphic::FinishedGameState)
    {
        sf::RectangleShape button2(buttonPos.button2Size);
        button2.setPosition(buttonPos.button2Pos);
        button2.setFillColor(color2);
        windowView.window->draw(button2);
    }

    // Render texts
    leftOffset = buttonPos.button1Pos.x + 40;
    topOffset = buttonPos.button1Pos.y + 40;
    renderText(textButtons.text1, leftOffset, topOffset);
    topOffset = buttonPos.button2Pos.y + 40;
    renderText(textButtons.text2, leftOffset, topOffset);
}


const Window::StaticStatePointers& MenuStateGraphic::getStaticPointers()
{return staticPointers;}
