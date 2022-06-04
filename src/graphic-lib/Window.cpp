#include "include/Window.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include "../game-lib/include/GameState.h"
#include "include/ActiveState_dir/Borad_dir/BoardGraphic.h"
#include "../tank-lib/include/Tank.h"
#include "../tank-lib/include/Bullet.h"
#include "../game-lib/include/GameState.h"



template<typename State, typename T>
bool Window::instanceOf(T* ptr)
{return dynamic_cast<State*>(ptr) != nullptr;}


Window::Window()
{
    videoMode = sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    window = std::make_unique<sf::RenderWindow>(videoMode, "Tanks", sf::Style::Default);
    windowView.window = window.get();
    windowView.leftOffset = 0.f;
    windowView.topOffset = 0.f;
    initiateActiveStatePointers();
    conscructComposit();
}


void Window::selectgameState(GameState* gameState)
{
    if(instanceOf<ActiveGameState>(gameState))
        this->gameState = Window::ActieveGameState;
    else if(instanceOf<::PauseGameState>(gameState))
        this->gameState = Window::PauseGameState;
    else if(instanceOf<::MenuGameState>(gameState))
        this->gameState = Window::MenuGameState;
    else if (instanceOf<::FinishedGameState>(gameState))
        this->gameState = Window::FinishedGameState;
}


void Window::initiateActiveStatePointers()
{
    activeStatePointers.tanks = std::make_shared<std::vector<Tank*>>(); // Why without brackets it isn't working?
    activeStatePointers.bullets = std::make_shared<std::vector<Bullet*>>();
    activeStatePointers.tiles = std::make_shared<Grid*>();
    activeStatePointers.level = std::make_shared<int>();
    activeStatePointers.playerTank = std::make_shared<Tank*>();
}


// void Window::fetchMenuStatePointers()


void Window::render()
{
    // on which child the render method will be called denense on gameState
    std::shared_ptr<AbstractWindow> foundChild;
    switch (gameState)
    {
    case GameStateGraphic::ActieveGameState:
        foundChild = children_map[gameState];
        foundChild->render();
        break;
    case GameStateGraphic::MenuGameState:
        foundChild = children_map[gameState];
        break;

    default:
        break;
    }
    // for(const std::shared_ptr<AbstractWindow>& child : children)
    // {
    //     child->render();
    // }

}


void Window::conscructComposit()
{
    std::shared_ptr<AbstractWindow> activeState = std::make_shared<ActiveStateGraphic>(windowView, activeStatePointers);
    // Later another states should be implemented
    this->children_map[GameStateGraphic::ActieveGameState] = activeState;
}


// void Window::update()
// {
//     std::unordered_map<GameStateGraphic, std::shared_ptr<AbstractWindow>>::iterator it_children = children_map.begin();
//     for (it_children; it_children != children_map.end(); it_children++)
//     {
//         it_children->second->update();
//     }

// }


std::string Window::checkEntityType(Entity* e)
{
    Entity* trial = e;
    Tank* tank = static_cast<Tank*>(e);
    // Bullet* bullet = static_cast<Bullet*>(e);

    if(tank != nullptr)
        return "tank";
    else
        return "bullet";
}


void Window::addEntity(Entity* e)
{
    // Checking what type of Entity it is
    Tank* tank = dynamic_cast<Tank*>(e);
    Bullet* bullet = dynamic_cast<Bullet*>(e);

    if(tank != nullptr)
        activeStatePointers.tanks->push_back(tank);
    else if (bullet != nullptr)
        activeStatePointers.bullets->push_back(bullet);
}


// void Window::moveEntity(Entity* e)
// {
//     Tank* tank = static_cast<Tank*>(e);
//     Bullet* bullet = static_cast<Bullet*>(e);
//     std::vector<Tank*>* tanks = activeStatePointers.tanks;
//     std::vector<Bullet*>* bullets = activeStatePointers.bullets;

//     if(tank != nullptr)
//     {
//         std::vector<Tank*>::iterator it = std::find(tanks->begin(), tanks->end(), tank);
//         if (it != tanks->end())
//             it
//     }



//     else if (bullet != nullptr)
//         activeStatePointers.bullets->push_back(bullet);
// }


void Window::removeEntity(Entity* e)
{
    Tank* tank = dynamic_cast<Tank*>(e);
    Bullet* bullet = dynamic_cast<Bullet*>(e);
    std::vector<Tank*>* tanks = activeStatePointers.tanks.get();
    std::vector<Bullet*>* bullets = activeStatePointers.bullets.get();

    if(tank != nullptr)
    {
        std::vector<Tank*>::iterator it = std::find(tanks->begin(), tanks->end(), tank);
        if (it != tanks->end())
            tanks->erase(it);
    }
    else if (bullet != nullptr)
    {
        std::vector<Bullet*>::iterator it = std::find(bullets->begin(), bullets->end(), bullet);
        if (it != bullets->end())
            bullets->erase(it);
    }
}


void Window::loadLevel(Grid* grid, int levelNumber)
{
    *activeStatePointers.tiles = grid;
    *activeStatePointers.level = levelNumber;
}


void Window::addPlayer(Entity* e)
{
    Tank* tank = dynamic_cast<Tank*>(e);
    activeStatePointers.tanks->push_back(tank);
    *activeStatePointers.playerTank = tank;
}