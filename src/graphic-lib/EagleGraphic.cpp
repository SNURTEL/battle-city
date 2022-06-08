#include "include/ActiveState_dir/Borad_dir/EagleGraphic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <unordered_map>
#include <sstream>
#include "include/TextureLoadingError.h"


void EagleGraphic::render()
{
    if (!eagleSet)
        setEaglePos();
        eagleSet = true;

    float leftOfset = windowView.leftOffset;
    float topOfset = windowView.topOffset;

    sf::RenderWindow* window = windowView.window;
    sf::Sprite sprite;

    sprite.setTexture(eagleTexture);
    sprite.setPosition(eaglePos);
    windowView.window->draw(sprite);

}


EagleGraphic::EagleGraphic(const WindowView& windowView, std::shared_ptr<std::shared_ptr<Eagle>> eagle,
                           std::shared_ptr<std::queue<sf::Sprite>> specialQueue)
: AbstractWindow(windowView), eagle(eagle), specialQueue(specialQueue)
{loadTexture();}


void EagleGraphic::setEaglePos()
{
    float leftOffset = windowView.leftOffset;
    float topOffset = windowView.topOffset;
    eaglePos.x = (*eagle)->getX() * 10 + leftOffset;
    eaglePos.y = (*eagle)->getY() * 10 + topOffset;
}


void EagleGraphic::loadTexture()
{
    if (!eagleTexture.loadFromFile(pathToTexture))
    {
        std::stringstream ss;
        ss << "Failed to load tank's texture: " << pathToTexture;
        throw TextureLoadingError(ss.str());
    }
}
