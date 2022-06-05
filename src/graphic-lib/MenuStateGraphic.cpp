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


MenuStateGraphic::MenuStateGraphic(const WindowView& windowView, const StaticGraphic::ButtonsPosition& buttonPos,
                                   const Window::StaticStatePointers& staticPointers)
: AbstractWindow(windowView), buttonPos(buttonPos), staticPointers(staticPointers), color(sf::Color(153, 153, 153))
{loadFont();}


void MenuStateGraphic::render()
{
    initiatePauseStateHeader(); // Amount of points might vary every render sesion

    Window::GameStateGraphic gameState = *(*staticPointers.gameState);
    Text textButtons = (gameState==Window::GameStateGraphic::MenuGameState) ? menuStateText : pauseStateText;
    Text textHeader = (gameState==Window::GameStateGraphic::MenuGameState) ? menuStateHeader : pauseStateHeader;

    renderHeader(gameState);

    renderButtons(gameState);
}


void MenuStateGraphic::initiatePauseStateHeader()
{
    std::stringstream ss;
    ss << "PLAYER POINTS: " << std::to_string(*staticPointers.points);
    pauseStateHeader.text2 = ss.str();
}


void MenuStateGraphic::renderHeader(Window::GameStateGraphic gameState)
{
    Text textHeader = (gameState==Window::GameStateGraphic::MenuGameState) ? menuStateHeader : pauseStateHeader;
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
    Text textButtons = (gameState==Window::GameStateGraphic::MenuGameState) ? menuStateText : pauseStateText;
    uint selection = *staticPointers.menuPos;

    sf::Color color1(sf::Color::White);
    sf::Color color2(sf::Color::White);


   if (selection == 1)
   {
       color1 = this->color;
   }
   else
   {
       color2 = this->color;
   }


    sf::RectangleShape button(buttonPos.button1Size);
    button.setPosition(buttonPos.button1Pos);
    button.setFillColor(color1);
    windowView.window->draw(button);

    sf::RectangleShape button2(buttonPos.button2Size);
    button2.setPosition(buttonPos.button2Pos);
    button2.setFillColor(color2);
    windowView.window->draw(button2);

    // Render texts
    leftOffset = buttonPos.button1Pos.x + 40;
    topOffset = buttonPos.button1Pos.y + 40;
    renderText(textButtons.text1, leftOffset, topOffset);
    topOffset = buttonPos.button2Pos.y + 40;
    renderText(textButtons.text2, leftOffset, topOffset);
}


const Window::StaticStatePointers& MenuStateGraphic::getStaticPointers()
{return staticPointers;}
