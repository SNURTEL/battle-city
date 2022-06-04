#include "include/ActiveState_dir/Borad_dir/BulletsGraphic.h"
#include <iterator>
#include <string>
#include <unordered_map>
#include "include/TextureLoadingError.h"
#include <sstream>


BulletsGraphic::RenderObject::RenderObject(sf::Vector2f pos, Direction direction, Bullet::BulletType type)
: coords(pos), direction(direction), textureType(type)
{}

BulletsGraphic::BulletsGraphic(const WindowView& windowView, std::shared_ptr<std::vector<Bullet*>> bullets)
: AbstractWindow(windowView), bullets(bullets)
{loadTextures();}


void BulletsGraphic::render()
{
    float leftOfset = windowView.leftOffset;
    float topOfset = windowView.topOffset;

    makeRenderBullets(); // updates bullets info
    sf::RenderWindow* window = windowView.window;
    std::string path;
    sf::Texture texture;
    sf::Sprite sprite;
    for(const RenderObject& bullet : renderBullets)
    {
        Bullet::BulletType bulletType = bullet.textureType;
        texture = textureMap[bulletType];
        sprite.setTexture(texture);
        sprite.setPosition(bullet.coords);
        float angle = getAngle(bullet.direction);
        setBulletRotation(sprite, angle);
        sprite.setScale(0.1f, 0.1f);
        windowView.window->draw(sprite);
    }
}


float BulletsGraphic::getAngle(Direction direction) const
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

void BulletsGraphic::setBulletRotation(sf::Sprite& sprite, float angle)
{
    float bulletSize = 40.f;
    sprite.setRotation(angle);
    int angleInt = static_cast<int>(angle);
    switch (angleInt)
    {
    case 0:
        break;
    case 90:
        sprite.move(sf::Vector2f(bulletSize, 0));
        break;
    case 180:
        sprite.move(sf::Vector2f(bulletSize, bulletSize));
        break;
    case 270:
        sprite.move(sf::Vector2f(0, bulletSize));
        break;
    default:
        break;
    }
}


void BulletsGraphic::makeRenderBullets()
{
    float leftOfset = windowView.leftOffset;
    float topOfset = windowView.topOffset;
    Bullet::BulletType bulletType;
    sf::Vector2f bulletPos;
    Direction direction;
    for (const Bullet* bullet : *bullets)
    {
        direction = bullet->getFacing();
        bulletPos.x = bullet->getX() + leftOfset;
        bulletPos.y = bullet->getY() + topOfset;
        renderBullets.push_back(BulletsGraphic::RenderObject(bulletPos, direction));
    }
}


void BulletsGraphic::loadTextures()
{
    std::unordered_map<Bullet::BulletType, std::string>::iterator it_path = pathMap.begin();
    // std::unordered_map<Bullet::BulletType, sf::Texture>::iterator it_texture = textureMap.begin();
    Bullet::BulletType type;
    sf::Texture texture;
    std::string path;

    // Not the best solution but iterating over an enum is problematic
    while (it_path != pathMap.end())
    {

        type = it_path->first;
        path = pathMap[type];

        if (!texture.loadFromFile(path))
        {
            std::stringstream ss;
            ss << "Failed to load bullet's texture: " << path;
            throw TextureLoadingError(ss.str());
        }
        else
        {
            textureMap[type] = texture;
        }
        it_path ++;
    }
}


std::vector<Bullet*>* BulletsGraphic::getBullets() const
{return bullets.get();};