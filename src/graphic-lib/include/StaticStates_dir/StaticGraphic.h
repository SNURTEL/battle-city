// Created by Michał

#ifndef PROI_PROJEKT_STATICGRAPHIC_H
#define PROI_PROJEKT_STATICGRAPHIC_H

// #include "../AbstractWindow.h"

#include "../Window.h"
#include <unordered_map>


struct Text
    {
        std::string text1;
        std::string text2;
    };


class StaticGraphic : public AbstractWindow
{
public:

    StaticGraphic(const WindowView& windowView, const Window::StaticStatePointers& staticPointers);


    void constructComposite();


    struct ButtonsPositionMenu
    {
        sf::Vector2f button1Pos;
        sf::Vector2f button1Size;
        sf::Vector2f button2Pos;
        sf::Vector2f button2Size;
    };

    struct ButtonPoistionFinish
    {
        sf::Vector2f buttonPos;
        sf::Vector2f buttonSize;
    };

    virtual void render() override;


    std::unordered_map<Window::GameStateGraphic, std::shared_ptr<AbstractWindow>>& getChildren();

protected:

    Window::StaticStatePointers staticPointers;

    std::shared_ptr<Window::GameStateGraphic*> gameState;

    ButtonsPositionMenu buttonsPosition;


    void calculateButtonsPos();


    WindowView finishedStateView;


    std::unordered_map<Window::GameStateGraphic, std::shared_ptr<AbstractWindow>> childrensMap{};


    WindowView menuStatesView;


    void setMenuView();


    void setFinishedView();

};

#endif // PROI_PROJEKT_STATICGRAPHIC_H