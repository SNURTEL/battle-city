#include "../include/Window.h"
#include "../../game-lib/include/GameState.h"
#include "../../tank-lib/include/Tank.h"
#include "../../tank-lib/include/Bullet.h"
#include "../../board-lib/include/Grid.h"
#include "../../game-lib/include/Game.h"
#include <unordered_map>
#include "../include/AbstractWindow.h"
#include "../include/ActiveState_dir/ActiveStateGraphic.h"
#include "../include/ActiveState_dir/Borad_dir/BoardGraphic.h"
#include "../include/ActiveState_dir/Borad_dir/TanksGraphic.h"
#include "../include/ActiveState_dir/Frame_dir/FrameGraphic.h"
#include "../include/ActiveState_dir/Borad_dir/BulletsGraphic.h"
#include "../include/ActiveState_dir/Borad_dir/TilesGraphic.h"



/// Extended window derived class for testing pupouse
class TestWindow : public Window
{
public:
    TestWindow(GameState* gameState)
    : Window(gameState)
    {};

    WindowView getWindowView() const
    {return windowView;};

    GameStateGraphic get_gameState()
    {return gameState;};

    std::unordered_map<GameStateGraphic, std::shared_ptr<AbstractWindow>> getChildren()
    {return children_map;};

    const Window::ActiveStatePointers& getPointers() const
    {return activeStatePointers;};

    /// @brief Gets tanks pointers from leaf object for tests
    std::vector<Tank*>* getTanks()
    {
        AbstractWindow* activeStateGraphicAb = getChildren()[GameStateGraphic::ActieveGameState].get();
        ActiveStateGraphic* activeStateGrahic = static_cast<ActiveStateGraphic*>(activeStateGraphicAb);
        // assuming BoardGraphic is firts on the list
        BoardGraphic* boardGraphic = static_cast<BoardGraphic*>(activeStateGrahic->getChildren()[0].get());
        // assuming TanksGraphic is second on the list
        AbstractWindow* tanksGraphicAb = boardGraphic->getChildren()[1].get();
        TanksGraphic* tanksGraphic = static_cast<TanksGraphic*>(tanksGraphicAb);
        std::vector<Tank*>* tanks = tanksGraphic->getTanks();
        return tanks;
    };

    /// @brief Gets tanks pointers from leaf object for tests
    Grid** getGrid()
    {
        AbstractWindow* activeStateGraphicAb = getChildren()[GameStateGraphic::ActieveGameState].get();
        ActiveStateGraphic* activeStateGrahic = static_cast<ActiveStateGraphic*>(activeStateGraphicAb);
        // assuming BoardGraphic is firts on the list
        BoardGraphic* boardGraphic = static_cast<BoardGraphic*>(activeStateGrahic->getChildren()[0].get());
        // assuming TilesGraphic is first on the list
        AbstractWindow* tilesGraphicAb = boardGraphic->getChildren()[0].get();
        TilesGraphic* tilesGraphic = static_cast<TilesGraphic*>(tilesGraphicAb);
        Grid** grid = tilesGraphic->getGrid();
        return grid;
    };

    /// @brief Gets tanks pointers from leaf object for tests
    std::vector<Bullet*>* getBullets()
    {
        AbstractWindow* activeStateGraphicAb = getChildren()[GameStateGraphic::ActieveGameState].get();
        ActiveStateGraphic* activeStateGrahic = static_cast<ActiveStateGraphic*>(activeStateGraphicAb);
        // assuming BoardGraphic is firts on the list
        BoardGraphic* boardGraphic = static_cast<BoardGraphic*>(activeStateGrahic->getChildren()[0].get());
        // assuming TanksGraphic is third on the list
        AbstractWindow* bulletsGraphicAb = boardGraphic->getChildren()[2].get();
        BulletsGraphic* bulletsGraphic = static_cast<BulletsGraphic*>(bulletsGraphicAb);
        std::vector<Bullet*>* bullets = bulletsGraphic->getBullets();
        return bullets;
    };

    const ActiveStateGraphic::BoardPointers& getBoardPointers()
    {
        AbstractWindow* activeStateGraphicAb = getChildren()[GameStateGraphic::ActieveGameState].get();
        ActiveStateGraphic* activeStateGrahic = static_cast<ActiveStateGraphic*>(activeStateGraphicAb);
        // assuming BoardGraphic is firts on the list
        BoardGraphic* boardGraphic = static_cast<BoardGraphic*>(activeStateGrahic->getChildren()[0].get());
        return boardGraphic->getPointers();
    }

    const ActiveStateGraphic::FramePointers& getFramePointers()
    {
        AbstractWindow* activeStateGraphicAb = getChildren()[GameStateGraphic::ActieveGameState].get();
        ActiveStateGraphic* activeStateGrahic = static_cast<ActiveStateGraphic*>(activeStateGraphicAb);
        // assuming FrameGraphic is second on the list
        FrameGraphic* boardGraphic = static_cast<FrameGraphic*>(activeStateGrahic->getChildren()[1].get());
        return boardGraphic->getPointers();
    }
};


/**
 * Extended Tank derived class for testing purposes
 */
class TestTank : public Tank {
public:
    TestTank() : Tank(Tank::PowerTank, 20, 20, 1, 1, 1, North, 100) {};
};

/**
 * Extended Bullet derived class for testing purposes
 */
class TestBullet : public Bullet
{
public:
    TestBullet() : Bullet(100.f, 100.f, Direction::East, 5.f, BulletType::Enemy) {};
};