#include "include/StaticStates_dir/StaticGraphic.h"
#include "include/StaticStates_dir/MenuStateGraphic.h"


StaticGraphic::StaticGraphic(const WindowView& windowView, const Window::StaticStatePointers& staticPointers)
: AbstractWindow(windowView), staticPointers(staticPointers)
{
    gameState = staticPointers.gameState;
    setMenuView();
    calculateButtonsPos();
    setFinishedView();
    constructComposite();
}


void StaticGraphic::calculateButtonsPos()
{
    float sizeX = windowView.window->getSize().x;
    float sizeY = windowView.window->getSize().y;

    // Button1
    buttonsPosition.button1Pos.x = sizeX * 1/3;
    buttonsPosition.button1Pos.y = sizeY * 1/3;
    buttonsPosition.button1Size.x = sizeX * 1/3;
    buttonsPosition.button1Size.y = sizeY * 1/6;

    // Button2
    buttonsPosition.button2Pos.x = sizeX * 1/3;
    buttonsPosition.button2Pos.y = sizeY * 3/5;
    buttonsPosition.button2Size.x = sizeX * 1/3;
    buttonsPosition.button2Size.y = sizeY * 1/6;
}


void StaticGraphic::setFinishedView()
{
    float sizeX = windowView.window->getSize().x;
    float sizeY = windowView.window->getSize().y;

    windowView.leftOffset = sizeX / 3;
    windowView.topOffset = sizeY / 3;
}


void StaticGraphic::constructComposite()
{
    std::shared_ptr<AbstractWindow> menuGameState = std::make_shared<MenuStateGraphic>
                                                    (menuStatesView, buttonsPosition,staticPointers);
    // childrensMap[Window::GameStateGraphic::MenuGameState] = menuGameState;
    children.push_back(menuGameState);

}


void StaticGraphic::render()
{
   for (const auto& child : children)
   {
       child->render();
   }
}


void StaticGraphic::setMenuView()
{
    menuStatesView.window = windowView.window;
    float leftOffset = menuStatesView.window->getSize().x / 6;
    float topOffset = menuStatesView.window->getSize().y / 17;
    menuStatesView.leftOffset = leftOffset;
    menuStatesView.topOffset = topOffset;
}


std::unordered_map<Window::GameStateGraphic, std::shared_ptr<AbstractWindow>>& StaticGraphic::getChildren()
{
    return childrensMap;
}
