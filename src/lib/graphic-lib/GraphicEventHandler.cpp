#include "include/Window.h"
#include "include/GraphicEventHandler.h"
#include "../board-lib/include/Grid.h"
#include "../game-lib/include/GameStatistics.h"


GraphicEventHandler::GraphicEventHandler(Window* window)
: window(window)
{}


void GraphicEventHandler::processEvent(std::unique_ptr<Event> event)
{
    switch (event->type)
    {
    case(Event::EntitySpawned):
    {
        std::shared_ptr<Entity> e = event->info.entityInfo.entity;
        window->addEntity(e);
        break;
    }
    case(Event::PlayerSpawned):
    {
        std::shared_ptr<Entity> e = event->info.entityInfo.entity;
        window->addEntity(e);
        break;
    }
    case(Event::EntityRemoved):
    {
        std::shared_ptr<Entity> e = event->info.entityInfo.entity;
        window->removeEntity(e);
        break;
    }
    case(Event::TankKilled):
    {
        std::shared_ptr<Entity> e = event->info.entityInfo.entity;
        window->removeEntity(e);
        break;
    }
    case(Event::LevelLoaded):
    {
        Grid* grid = event->info.levelInfo.grid;
        int levelNumber = event->info.levelInfo.levelNumber;
        std::pair<unsigned int, unsigned int> eagleLocation = grid->getEagleLocation();
        window->loadLevel(grid, levelNumber, eagleLocation);
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
    case(Event::GameWon):
    {
        window->gameWon();
    }
    default:
        break;
    }
}
