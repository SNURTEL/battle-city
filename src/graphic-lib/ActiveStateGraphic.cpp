#include "include/ActiveState_dir/Borad_dir/BoardGraphic.h"
#include "include/ActiveState_dir/Frame_dir/FrameGraphic.h"
#include "include/ActiveState_dir/ActiveStateGraphic.h"
#include <cmath>

ActiveStateGraphic::ActiveStateGraphic(const WindowView& windowView, const Window::ActiveStatePointers& activestatePointers)
: AbstractWindow(windowView), activeStateObjects(activestatePointers)
{
    setboardObjects();
    setframeObjects();
    setframeView();
    setboardView();
    conscructComposite();
}


void ActiveStateGraphic::setboardView()
{
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
    frameObjects.playerLives = activeStateObjects.playerLives;
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
