#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
// #include "AbstractWindow.h"
#include <unordered_map>
// #include "../../core-lib/include/EventHandler.h"  //FIXME SUPER UGLY
#include "include/Window.h"
#include "include/GraphicEventHandler.h"
#include "../board-lib/include/Grid.h"


GraphicEventHandler::GraphicEventHandler(Game* game, Window* window)
: game(game), window(window)
{}


void GraphicEventHandler::processEvent(std::unique_ptr<Event> event)
{
    switch (event->type)
    {
    case(Event::EntitySpawned):
    {
        Entity* e = event->info.entityInfo.entity;
        window->addEntity(e);
        break;
    }

    // case(Event::EntityMoved):
    //     Entity* e = event->info.entityInfo.entity;
    //     window.moveEntity(e);
    //     break;
    case(Event::EntityRemoved):
    {
        Entity* e = event->info.entityInfo.entity;
        window->removeEntity(e);
        break;
    }

    // case(Event::TilePlaced || Event::TileChanged || Event::TileDeleted):
    //     float xPos = event->info.tileInfo.tile_x;
    //     float yPos = event->info.tileInfo.tile_y;
    //     Grid* grid = event->info.tileInfo.grid;
    //     TileType tileType = grid->getTileAtPosition(xPos, yPos);
    //     window.changeTile(xPos, yPos, tileType);
    //     break;
    case(Event::LevelLoaded):
    {
        Grid* grid = event->info.levelInfo.grid;
        int levelNumber = event->info.levelInfo.levelNumber;
        window->loadLevel(grid, levelNumber);
        break;
    }

    default:
        break;
    }
}
