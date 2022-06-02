#include "include/ActiveState_dir/Borad_dir/TanksGraphic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <string>


TanksGraphic::RenderObject::RenderObject(const sf::Vector2f& coords, Direction direction,
                                         Tank::TankType textureType)
: coords(coords), direction(direction), textureType(textureType)
{}


void TanksGraphic::render()
{
    float leftOfset = windowView.leftOfset;
    float topOfset = windowView.topOfset;

    makeRenderTanks(); // updates tanks info
    sf::RenderWindow* window = windowView.window;
    std::string path;
    sf::Texture texture;
    sf::Sprite sprite;
    for(const RenderObject& tank : renderTanks)
    {
        Tank::TankType tankType = tank.textureType;
        // std::map<Tank::TankType, sf::Texture>::iterator tankTypeIt = textureMap.find(tankType);
        texture = textureMap[tankType];
        sprite.setTexture(texture);
        // sprite.set
        sprite.setPosition(tank.coords);
        float angle = getAngle(tank.direction);
        setTankRotation(sprite, angle);
        sprite.setScale(4.f, 4.f);
        // sprite.setScale() Might be used in the future
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


TanksGraphic::TanksGraphic(const WindowView& windowView, std::vector<Tank*>* tanks)
: AbstractWindow(windowView), tanks(tanks)
{loadTextures();}


void TanksGraphic::makeRenderTanks()
{
    float leftOfset = windowView.leftOfset;
    float topOfset = windowView.topOfset;
    Tank::TankType tankType;
    sf::Vector2f tankPos;
    Direction direction;
    for (const Tank* tank : *tanks)
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
            // Exception
        }
        else
        {
            textureMap[type] = texture;
        }
        it_path ++;
    }
}


std::vector<Tank*>* TanksGraphic::getTanks() const
{return tanks;}