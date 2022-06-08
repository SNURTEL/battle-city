#include "include/ActiveState_dir/Borad_dir/BoardGraphic.h"
#include "include/ActiveState_dir/Borad_dir/TanksGraphic.h"
#include "include/ActiveState_dir/Borad_dir/TilesGraphic.h"
#include "include/ActiveState_dir/Borad_dir/BulletsGraphic.h"


void BoardGraphic::render()
{
    for(const std::shared_ptr<AbstractWindow>& child : children)
    {
        child->render();
    }

    for (int i = 0; i < extraRenderQueue->size(); i++)
    {
        sf::Sprite tile = extraRenderQueue->front();
        extraRenderQueue->pop();
        windowView.window->draw(tile);
    }
}


BoardGraphic::BoardGraphic(const WindowView& windowView, const ActiveStateGraphic::BoardPointers& boardObjects)
: AbstractWindow(windowView), boardObjects(boardObjects)
{
    initiateRenderQueue();
    conscructComposite();
}


void BoardGraphic::conscructComposite()
{
    std::shared_ptr<std::vector<std::shared_ptr<Tank>>> tanksList = boardObjects.tanks;
    std::shared_ptr<std::vector<std::shared_ptr<Bullet>>> bulltesList = boardObjects.bullets;
    std::shared_ptr<Grid*> gridObject = boardObjects.tiles;
    std::shared_ptr<AbstractWindow> tanks = std::make_shared<TanksGraphic>(windowView, tanksList, extraRenderQueue);
    std::shared_ptr<AbstractWindow> bullets = std::make_shared<BulletsGraphic>(windowView, bulltesList, extraRenderQueue);
    std::shared_ptr<AbstractWindow> tiles = std::make_shared<TilesGraphic>(windowView, gridObject, extraRenderQueue);

    // ORDER IS INHERENT IN LIST OF THOSE CHILDREN !!!

    children.push_back(tiles);
    children.push_back(tanks);
    children.push_back(bullets);
}



const ActiveStateGraphic::BoardPointers& BoardGraphic::getPointers() const
{return boardObjects;}


const std::vector<std::shared_ptr<AbstractWindow>> BoardGraphic::getChildren() const
{return children;}


void BoardGraphic::initiateRenderQueue()
{extraRenderQueue = std::make_shared<std::queue<sf::Sprite>>();}