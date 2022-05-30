#include "include/ActiveState_dir/Frame_dir/FrameGraphic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


void FrameGraphic::render()
{
    for(const std::shared_ptr<AbstractWindow>& child : children)
    {
        child->render();
    }
}


FrameGraphic::FrameGraphic(const WindowView& windowView, const ActiveStateGraphic::FramePointers& frameObjects)
: AbstractWindow(windowView), frameObjects(frameObjects)
{
    calculateBareFrameSize();
    // createCop
}


// void FrameGraphic::conscructComposite()
// {

// }

void FrameGraphic::calculateBareFrameSize()
{
    float defaultFrameX = windowView.view.getSize().x * 3/4 / 2;
    float defaultFramey = windowView.view.getSize().y * 3/4 / 2;
    float windowSizeX = windowView.view.getSize().x;
    float windowSizeY = windowView.view.getSize().y;
    float windowBeginPoint = 0.f;

    // upRect
    float upSizeX = windowSizeX;
    float upSizeY = defaultFramey;
    bareFrameSize.upRect = sf::FloatRect(sf::Vector2f(windowBeginPoint, windowBeginPoint),
                                         sf::Vector2f(upSizeX, upSizeY));

    // downRect
    float downSizeX = windowSizeX;
    float downSizeY = defaultFramey;
    float downRectPos = windowSizeY - downSizeY;
    bareFrameSize.upRect = sf::FloatRect(sf::Vector2f(windowBeginPoint, downRectPos),
                                         sf::Vector2f(upSizeX, upSizeY));

    // leftRect
    float leftSizeX = defaultFrameX / 2;
    float leftSizeY = windowSizeY - 2 * upSizeY;
    bareFrameSize.leftRect = sf::FloatRect(sf::Vector2f(windowBeginPoint, upSizeY),
                                           sf::Vector2f(leftSizeX, leftSizeY));

    // rightRect
    float rightSizeX = defaultFrameX * 2;
    float rightSizeY = windowSizeY - 2 * upSizeY;
    float rightRectPos = windowSizeX - rightSizeX;
    bareFrameSize.rightRect = sf::FloatRect(sf::Vector2f(rightRectPos, upSizeY),
                                            sf::Vector2f(rightSizeX, rightSizeY));
}


const ActiveStateGraphic::FramePointers& FrameGraphic::getPointers() const
{return frameObjects;}
