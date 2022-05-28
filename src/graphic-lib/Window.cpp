#include "include/Window.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include "../game-lib/include/GameState.h"
#include "include/ActiveState_dir/Borad_dir/BoardGraphic.h"

template<typename State, typename T>
bool Window::instanceOf(T* ptr)
{return dynamic_cast<State*>(ptr) != nullptr;}


Window::Window(GameState* gameState)
{
    selectgameState(gameState);
    videoMode = sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    window = std::make_unique<sf::RenderWindow>(videoMode, "Tanks", sf::Style::Close);
    windowView.window = window.get();
    windowView.view = window->getView();
    conscructComposit();
}


void Window::selectgameState(GameState* gameState)
{
    if(instanceOf<ActiveGameState>(gameState))
        this->gameState = Window::ActieveGameState;
    // else if(instanceOf<PausedGameState>(gameState))
    //     this->gameState = Window::PauseGameState;
    // else if(instanceOf<MenuGameState>(gameState))
    //     this->gameState = Window::MenuGameState;
    // else if (instanceOf<FinishedGameState>(gameState))
    //     this->gameState = Window::FinishedGameState
    // I need to fetch code to my branch with this states
}


void Window::fetchAcitveStatePointers(std::vector<Tank*>* tanks, Grid* tiles, std::vector<Bullet*>* bullets,
                                      int* level, int* livesLeft)
{
    activeStatePointers.tanks = tanks;
    activeStatePointers.bullets = bullets;
    activeStatePointers.level = level;
    activeStatePointers.playerLivesLeft = livesLeft;
    activeStatePointers.tiles = tiles;
    // And tiles but I don't know in which structure I will recieve them
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
        break;
    case GameStateGraphic::MenuGameState:
        foundChild = children_map[gameState];
        break;

    default:
        break;
    }
    for(const std::shared_ptr<AbstractWindow>& child : children)
    {
        child->render();
    }

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