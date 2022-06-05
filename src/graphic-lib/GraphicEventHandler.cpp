#include "include/Window.h"
#include "include/GraphicEventHandler.h"
#include "../board-lib/include/Grid.h"
#include "../game-lib/include/GameStatistics.h"


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
    case(Event::PlayerSpawned):
    {
        Entity* e = event->info.entityInfo.entity;
        window->addEntity(e);
    }
    case(Event::EntityRemoved):
    {
        Entity* e = event->info.entityInfo.entity;
        window->removeEntity(e);
        break;
    }
    case(Event::LevelLoaded):
    {
        Grid* grid = event->info.levelInfo.grid;
        int levelNumber = event->info.levelInfo.levelNumber;
        window->loadLevel(grid, levelNumber);
        break;
    }
    case(Event::StateChanged):
    {
        GameState* gameState = event->info.stateInfo.state_;
        window->selectgameState(gameState);
        break;
    }
    case(Event::MenuSelectionChange):
    {
        int menuPos = event->info.menuInfo.new_pos;
        window->changeMenuPos(menuPos);
        break;
    }
    case(Event::StatisticsChanged):
    {
        int playerLives = event->info.pointsInfo.stats_->getLives();
        int points = event->info.pointsInfo.stats_->getPoints();
        window->loadStats(playerLives, points);
        break;
    }
    default:
        break;
    }
}
