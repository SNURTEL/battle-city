#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "AbstractWindow.h"
#include <unordered_map>
#include "../../core-lib/include/EventHandler.h"  //FIXME SUPER UGLY
#include "Window.h"

class Game;



#ifndef PROI_PROJEKT_GRAPHICEVENTHANDLER_H
#define PROI_PROJEKT_GRAPHICEVENTHANDLER_H

class GraphicEventHandler
{
public:
    GraphicEventHandler(Game* game, Window* window);

    void processEvent(std::unique_ptr<Event> event);

protected:

    Window* window;
    Game* game;
};

#endif //PROI_PROJEKT__GRAPHICEVENTHANDLER_H