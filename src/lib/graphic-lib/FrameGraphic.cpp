#include "include/ActiveState_dir/Frame_dir/FrameGraphic.h"
#include "include/ActiveState_dir/Frame_dir/BareFrameGraphic.h"
#include "../tank-lib/include/Tank.h"
#include "include/ActiveState_dir/Frame_dir/GameInfoGraphic.h"
#include <memory>

void FrameGraphic::render()
{
    makeGamePointers(); // Players lives needs to be updated every render loop
    for(const std::shared_ptr<AbstractWindow>& child : children)
    {
        child->render();
    }
}


FrameGraphic::FrameGraphic(const WindowView& windowView, const ActiveStateGraphic::FramePointers& frameObjects)
: AbstractWindow(windowView), frameObjects(frameObjects)
{
    initiatesGameInfoPointers();
    calculateInfoTextPoistion();
    calculateBareFrameSize();
    conscructComposite();
}


void FrameGraphic::makeGamePointers()
{
    *gameInfoPointers.playerLives = *frameObjects.playerLives;
    *gameInfoPointers.level = *frameObjects.level;
    *gameInfoPointers.points = *frameObjects.points;
}


void FrameGraphic::conscructComposite()
{
    std::shared_ptr<AbstractWindow> bareFrame = std::make_shared<BareFrameGraphic>(windowView, bareFrameSize);
    std::shared_ptr<AbstractWindow> gameInfo = std::make_shared<GameInfoGraphic>(infoWindowView, gameInfoPointers);
    children.push_back(bareFrame);
    children.push_back(gameInfo);
}

void FrameGraphic::calculateBareFrameSize()
{
    float defaultFrameX = (windowView.window->getSize().x - 520)/2;
    float defaultFramey = (windowView.window->getSize().y - 520)/2;
    float windowSizeX = windowView.window->getSize().x;
    float windowSizeY = windowView.window->getSize().y;
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
    bareFrameSize.downRect = sf::FloatRect(sf::Vector2f(windowBeginPoint, downRectPos),
                                         sf::Vector2f(upSizeX, upSizeY));

    // leftRect
    float leftSizeX = defaultFrameX * 2/3;
    float leftSizeY = windowSizeY - 2 * upSizeY;
    bareFrameSize.leftRect = sf::FloatRect(sf::Vector2f(windowBeginPoint, upSizeY),
                                           sf::Vector2f(leftSizeX, leftSizeY));

    // rightRect
    float rightSizeX = defaultFrameX * 4/3;
    float rightSizeY = windowSizeY - 2 * upSizeY;
    float rightRectPos = windowSizeX - rightSizeX;
    bareFrameSize.rightRect = sf::FloatRect(sf::Vector2f(rightRectPos, upSizeY),
                                            sf::Vector2f(rightSizeX, rightSizeY));
}


const ActiveStateGraphic::FramePointers& FrameGraphic::getPointers() const
{return frameObjects;}


void FrameGraphic::calculateInfoTextPoistion()
{
    float defaultFrameXRight = (windowView.window->getSize().x - 520)/2 * 4/3;
    float windowSizeX = windowView.window->getSize().x;
    float windowSizeY = windowView.window->getSize().y;

    float extraPixelsPadding = 10;

    float offsetX = windowSizeX - defaultFrameXRight + extraPixelsPadding;

    float offsetY = windowSizeY * 1/4;

    infoWindowView.leftOffset = offsetX;
    infoWindowView.topOffset = offsetY;
    infoWindowView.window = windowView.window;
}


void FrameGraphic::initiatesGameInfoPointers()
{
    gameInfoPointers.level = std::make_shared<int>();
    gameInfoPointers.playerLives = std::make_shared<int>();
    gameInfoPointers.points = std::make_shared<int>();
}