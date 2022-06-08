#include "include/Window.h"
#include "include/ActiveState_dir/Borad_dir/BoardGraphic.h"
#include "include/StaticStates_dir/StaticGraphic.h"




template<typename State, typename T>
bool Window::instanceOf(T* ptr)
{return dynamic_cast<State*>(ptr) != nullptr;}


Window::Window()
{
    videoMode = sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    window = std::make_unique<sf::RenderWindow>(videoMode, "Tanks", sf::Style::Default);
    window->setKeyRepeatEnabled(false);
    window->setFramerateLimit(60);
    windowView.window = window.get();
    windowView.leftOffset = 0.f;
    windowView.topOffset = 0.f;
    initateStaticStatePointers(); // Order of initiation is important
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
    activeStatePointers.tanks = std::make_shared<std::vector<std::shared_ptr<Tank>>>();
    activeStatePointers.bullets = std::make_shared<std::vector<std::shared_ptr<Bullet>>>();
    activeStatePointers.tiles = std::make_shared<Grid*>();
    activeStatePointers.level = std::make_shared<int>();
    activeStatePointers.playerLives = std::make_shared<int>();
    activeStatePointers.eagle = std::make_shared<std::shared_ptr<Eagle>>();
    activeStatePointers.points = staticStatesPointers.points;
}


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
    default:
        foundChild = children_map[GameStateGraphic::StaticStates];
        foundChild->render();
    }
}


void Window::conscructComposit()
{
    std::shared_ptr<AbstractWindow> activeState = std::make_shared<ActiveStateGraphic>(windowView, activeStatePointers);
    std::shared_ptr<AbstractWindow> staticState = std::make_shared<StaticGraphic>(windowView, staticStatesPointers);
    this->children_map[GameStateGraphic::ActieveGameState] = activeState;
    this->children_map[GameStateGraphic::StaticStates] = staticState;
}


void Window::addEntity(std::shared_ptr<Entity> e)
{
    // Checking what type of Entity it is
    std::shared_ptr<Tank> tank = std::dynamic_pointer_cast<Tank>(e);
    std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(e);
    std::shared_ptr<Eagle> eagle = std::dynamic_pointer_cast<Eagle>(e);

    if(tank != nullptr)
        activeStatePointers.tanks->push_back(tank);
    else if (bullet != nullptr)
        activeStatePointers.bullets->push_back(bullet);
    else if (bullet != nullptr)
        *activeStatePointers.eagle = eagle;
}


void Window::removeEntity(std::shared_ptr<Entity> e)
{
    std::shared_ptr<Tank> tank = std::dynamic_pointer_cast<Tank>(e);
    std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(e);
    std::vector<std::shared_ptr<Tank>>* tanks = activeStatePointers.tanks.get();
    std::vector<std::shared_ptr<Bullet>>* bullets = activeStatePointers.bullets.get();

    if(tank != nullptr)
    {
        std::vector<std::shared_ptr<Tank>>::iterator it = std::find(tanks->begin(), tanks->end(), tank);
        if (it != tanks->end())
            tanks->erase(it);
    }
    else if (bullet != nullptr)
    {
        std::vector<std::shared_ptr<Bullet>>::iterator it = std::find(bullets->begin(), bullets->end(), bullet);
        if (it != bullets->end())
            bullets->erase(it);
    }
}


void Window::loadLevel(Grid* grid, int levelNumber)
{
    *activeStatePointers.tiles = grid;
    *activeStatePointers.level = levelNumber;
}


void Window::loadStats(int playerLives, int points)
{
    *activeStatePointers.playerLives = playerLives;
    *staticStatesPointers.points = points;
}


void Window::initateStaticStatePointers()
{
    staticStatesPointers.menuPos = std::make_shared<int>();
    staticStatesPointers.points = std::make_shared<int>();
    staticStatesPointers.gameState = std::make_shared<GameStateGraphic*>(&gameState);

}


void Window::changeMenuPos(uint menuPos)
{
    *staticStatesPointers.menuPos = menuPos;
}
