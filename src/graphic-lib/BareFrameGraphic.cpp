#include "include/ActiveState_dir/Frame_dir/BareFrameGraphic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


BareFrameGraphic::BareFrameGraphic(const WindowView& windowView, const FrameGraphic::BareFrameSize& bareFrameSize)
: AbstractWindow(windowView), frameSize(bareFrameSize), color(sf::Color(153, 153, 153))
{}


void BareFrameGraphic::render()
{
    windowView.window->setView(windowView.view);

    // Render upRect
    sf::Vector2f ractSizeUp(frameSize.upRect.width, frameSize.upRect.height);
    sf::Vector2f ractPosUp(frameSize.upRect.left, frameSize.upRect.top);
    sf::RectangleShape ractUp(ractSizeUp);
    ractUp.setPosition(ractPosUp);
    ractUp.setFillColor(color);
    windowView.window->draw(ractUp);

    // Render leftRect
    sf::Vector2f ractSizeLeft(frameSize.leftRect.width, frameSize.leftRect.height);
    sf::Vector2f ractPosLeft(frameSize.leftRect.left, frameSize.leftRect.top);
    sf::RectangleShape ractLeft(ractSizeLeft);
    ractLeft.setPosition(ractPosLeft);
    ractLeft.setFillColor(color);
    windowView.window->draw(ractLeft);

    // Render downRect
    sf::Vector2f ractSizeDown(frameSize.downRect.width, frameSize.downRect.height);
    sf::Vector2f ractPosDown(frameSize.downRect.left, frameSize.downRect.top);
    sf::RectangleShape ractDown(ractSizeDown);
    ractDown.setPosition(ractPosDown);
    ractDown.setFillColor(color);
    windowView.window->draw(ractDown);

    // Render rightRect
    sf::Vector2f ractSizeRight(frameSize.rightRect.width, frameSize.rightRect.height);
    sf::Vector2f ractPosRight(frameSize.rightRect.left, frameSize.rightRect.top);
    sf::RectangleShape ractRight(ractSizeRight);
    ractRight.setPosition(ractPosRight);
    ractRight.setFillColor(color);
    windowView.window->draw(ractRight);

    // Setting default view for next objects
    windowView.window->setView(windowView.window->getDefaultView());

}