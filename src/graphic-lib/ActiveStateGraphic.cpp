#include "include/ActiveState_dir/Borad_dir/BoardGraphic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


ActiveStateGraphic::ActiveStateGraphic(const WindowView& windowView, const Window::ActiveStatePointers& activestatePointers)
: AbstractWindow(windowView)
{
    activeStateObjects = activestatePointers;
    setboardObjects();
    setframeObjects();
    conscructComposite();
}


void ActiveStateGraphic::setboardView()
{
    sf::View defaultView = windowView.window->getView();
    sf::Vector2f defaultSize = defaultView.getSize();
    sf::Vector2f boardSize(defaultSize.x * 3/4, defaultSize.y * 3/4);
    sf::Vector2f boardPoistion = defaultSize - boardSize;
    sf::View boardView(sf::FloatRect(boardPoistion, boardSize));

    boardWindowView.view = boardView;
    boardWindowView.window = windowView.window;
}


void ActiveStateGraphic::setframeView()
{
    sf::View frameView;
    frameView = windowView.window->getView();
    frameWindowView.view = frameView;
    frameWindowView.window = windowView.window;
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
    frameObjects.playerLivesLeft = activeStateObjects.playerLivesLeft;
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
    std::shared_ptr<AbstractWindow> frame = std::make_shared<BoardGraphic>(frameWindowView, boardObjects);
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