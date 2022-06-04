#include "include/ActiveState_dir/Borad_dir/BoardGraphic.h"
#include "include/ActiveState_dir/Frame_dir/FrameGraphic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

ActiveStateGraphic::ActiveStateGraphic(const WindowView& windowView, const Window::ActiveStatePointers& activestatePointers)
: AbstractWindow(windowView)
{
    activeStateObjects = activestatePointers;
    setboardObjects();
    setframeObjects();
    setframeView();
    setboardView();
    conscructComposite();
}


void ActiveStateGraphic::setboardView()
{
    // sf::View defaultView = windowView.window->getView();
    // sf::Vector2f defaultSize = defaultView.getSize();
    // sf::Vector2f boardSize(520.f, 520.f);

    // setting the view origin
    // sf::Vector2f boardPoistion = defaultSize - boardSize;
    // float x_move = boardSize.x * std::sqrt(2) / 2;
    // float y_move = boardSize.y * std::sqrt(2) / 2;
    // boardPoistion.x += x_move;
    // boardPoistion.y += y_move;
    // sf::View boardView(boardPoistion, boardSize);
    // float trial = (defaultSize.x - boardSize.x) / WINDOW_HEIGHT;
    // boardView.setViewport(sf::FloatRect(sf::Vector2f(0.5, 0.5), sf::Vector2f(trial, trial)));
    // boardWindowView.view = boardView;

    float defaultFrameX = (windowView.window->getSize().x - 520)/2;
    float defaultFrameY = (windowView.window->getSize().y - 520)/2;

    boardWindowView.leftOffset = defaultFrameX * 2/3;
    boardWindowView.topOffset = defaultFrameY;
    boardWindowView.window = windowView.window;
}


void ActiveStateGraphic::setframeView()
{
    frameWindowView = windowView;
}


void ActiveStateGraphic::setboardObjects()
{
    boardObjects.tiles = activeStateObjects.tiles;
    boardObjects.tanks = activeStateObjects.tanks;
    boardObjects.bullets = activeStateObjects.bullets;
}


void ActiveStateGraphic::setframeObjects()
{
    frameObjects.level = activeStateObjects.level;
    frameObjects.playerTank = activeStateObjects.playerTank;
}


std::vector<std::shared_ptr<AbstractWindow>>& ActiveStateGraphic::getChildren()
{
    return children;
}


void ActiveStateGraphic::render()
{
    for(const std::shared_ptr<AbstractWindow>& child : children)
    {
        child->render();
    }
}


void ActiveStateGraphic::conscructComposite()
{
    std::shared_ptr<AbstractWindow> board = std::make_shared<BoardGraphic>(boardWindowView, boardObjects);
    std::shared_ptr<AbstractWindow> frame = std::make_shared<FrameGraphic>(frameWindowView, frameObjects);
    this->children.push_back(board);
    this->children.push_back(frame);
}


// void ActiveStateGraphic::update()
// {
//     // setframeObjects();
//     // setboardObjects();
//     for(std::shared_ptr<AbstractWindow> child : children)
//     {
//         child->update();
//     }
// }