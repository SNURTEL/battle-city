// Created by Michał



#ifndef PROI_PROJEKT_MENUSTATEGRAPHIC_H
#define PROI_PROJEKT_MENUSTATEGRAPHIC_H

#include "../AbstractWindow.h"
#include "StaticGraphic.h"
#include "../Window.h"

class MenuStateGraphic : public AbstractWindow
{
public:

    MenuStateGraphic(const WindowView& windowView, const StaticGraphic::ButtonsPosition& buttonPos,
                     const Window::StaticStatePointers& staticPointers);


    virtual void render() override;

    const Window::StaticStatePointers& getStaticPointers();

protected:

    StaticGraphic::ButtonsPosition buttonPos;

    Window::StaticStatePointers staticPointers;

    sf::Color color;

    void loadFont();

    sf::Font font;

    Text menuStateText
    {
        .text1 = "START GAME",
        .text2 = "      QUIT"
    };

    Text pauseStateText
    {
        .text1 = "    RESUME",
        .text2 = "GO TO MENU"
    };

    Text menuStateHeader
    {
        .text1 = " MENU",
        .text2 = ""
    };

    Text pauseStateHeader
    {
        .text1 = "PAUSE",
    };

    void renderText(std::string renderText, float leftOffset,
                    float topOffset, float fontSize=25,
                    sf::Color color=sf::Color::Black);


    void initiatePauseStateHeader();


    void renderHeader(Window::GameStateGraphic gameState);

    void renderButtons(Window::GameStateGraphic gameState);
};


#endif // PROI_PROJEKT_MENUSTATEGRAPHIC_H
