#include "include/ActiveState_dir/Borad_dir/BoardGraphic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "include/ActiveState_dir/Borad_dir/TanksGraphic.h"
#include "include/ActiveState_dir/Borad_dir/TilesGraphic.h"
#include "include/ActiveState_dir/Borad_dir/BulletsGraphic.h"


void BoardGraphic::render()
{
    for(const std::shared_ptr<AbstractWindow>& child : children)
    {
        child->render();
    }
}


BoardGraphic::BoardGraphic(const WindowView& windowView, const ActiveStateGraphic::BoardPointers& boardObjects)
: AbstractWindow(windowView), boardObjects(boardObjects)
{}


void BoardGraphic::conscructComposite()
{
    std::vector<Tank*> tanksList = boardObjects.tanks;
    std::vector<Bullet*> bulltesList = boardObjects.bullets;
    Grid* gridObject = boardObjects.tiles;
    std::shared_ptr<AbstractWindow> tanks = std::make_shared<TanksGraphic>(windowView, tanksList);
    std::shared_ptr<AbstractWindow> bullets = std::make_shared<BulletsGraphic>(windowView, bulltesList);
    std::shared_ptr<AbstractWindow> tiles = std::make_shared<TilesGraphic>(windowView, gridObject);

    // ORDER IS INHERENT IN LIST OF THOSE CHILDREN !!!

    children.push_back(tiles);
    children.push_back(tanks);
    children.push_back(bullets);

}