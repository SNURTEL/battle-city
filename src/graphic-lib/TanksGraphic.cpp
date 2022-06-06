#include "include/ActiveState_dir/Borad_dir/TanksGraphic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <unordered_map>
#include <sstream>
#include "include/TextureLoadingError.h"

TanksGraphic::RenderObject::RenderObject(const sf::Vector2f& coords, Direction direction,
                                         Tank::TankType textureType)
: coords(coords), direction(direction), textureType(textureType)
{}


void TanksGraphic::render()
{
    float leftOfset = windowView.leftOffset;
    float topOfset = windowView.topOffset;

    makeRenderTanks(); // updates tanks info
    sf::RenderWindow* window = windowView.window;
    std::string path;
    sf::Texture texture;
    sf::Sprite sprite;
    for(const RenderObject& tank : renderTanks)
    {
        Tank::TankType tankType = tank.textureType;
        texture = textureMap[tankType];
        sprite.setTexture(texture);
        sprite.setPosition(tank.coords);
        float angle = getAngle(tank.direction);
        setTankRotation(sprite, angle);
        sprite.setScale(4.f, 4.f);
        windowView.window->draw(sprite);
    }
}


void TanksGraphic::setTankRotation(sf::Sprite& sprite, float angle)
{
    float tankSize = 40.f;
    sprite.setRotation(angle);
    int angleInt = static_cast<int>(angle);
    switch (angleInt)
    {
    case 0:
        break;
    case 90:
        sprite.move(sf::Vector2f(tankSize, 0));
        break;
    case 180:
        sprite.move(sf::Vector2f(tankSize, tankSize));
        break;
    case 270:
        sprite.move(sf::Vector2f(0, tankSize));
        break;
    default:
        break;
    }
}


float TanksGraphic::getAngle(Direction direction) const
{
    float angle;
    switch (direction)
        {
        case Direction::North:
            angle = 0.f;
            break;
        case Direction::West:
            angle = 270.f;
            break;
        case Direction::South:
            angle = 180.f;
            break;
        case Direction::East:
            angle = 90.f;
            break;
        default:
            break;
        }
    return angle;
}


TanksGraphic::TanksGraphic(const WindowView& windowView, std::shared_ptr<std::vector<std::shared_ptr<Tank>>> tanks,
                           std::shared_ptr<std::queue<sf::Sprite>> specialQueue)
: AbstractWindow(windowView), tanks(tanks), specialQueue(specialQueue)
{loadTextures();}


void TanksGraphic::makeRenderTanks()
{
    float leftOfset = windowView.leftOffset;
    float topOfset = windowView.topOffset;
    Tank::TankType tankType;
    sf::Vector2f tankPos;
    Direction direction;
    for (const std::shared_ptr<Tank> tank : *tanks)
    {
        direction = tank->getFacing();
        tankPos.x = tank->getX() + leftOfset;
        tankPos.y = tank->getY() + topOfset;
        tankType = tank->getType();
        renderTanks.push_back(RenderObject(tankPos, direction, tankType));
    }
}


void TanksGraphic::loadTextures()
{
    std::unordered_map<Tank::TankType, std::string>::iterator it_path = pathMap.begin();
    // std::unordered_map<Tank::TankType, sf::Texture>::iterator it_texture = textureMap.begin();
    Tank::TankType type;
    sf::Texture texture;
    std::string path;

    // Not the best solution but iterating over an enum is problematic
    while (it_path != pathMap.end())
    {

        type = it_path->first;
        // texture = it_texture->second;
        path = pathMap[type];

        if (!texture.loadFromFile(path))
        {
            std::stringstream ss;
            ss << "Failed to load tank's texture: " << path;
            throw TextureLoadingError(ss.str());
        }
        else
        {
            textureMap[type] = texture;
        }
        it_path ++;
    }
}


std::vector<std::shared_ptr<Tank>>* TanksGraphic::getTanks() const
{return tanks.get();}