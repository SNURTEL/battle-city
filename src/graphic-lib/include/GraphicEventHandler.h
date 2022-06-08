// Created by Michał


#ifndef PROI_PROJEKT_GRAPHICEVENTHANDLER_H
#define PROI_PROJEKT_GRAPHICEVENTHANDLER_H

#include "AbstractWindow.h"
#include <unordered_map>
#include "../../core-lib/include/EventHandler.h"
#include "Window.h"


/**
 * @brief Class responsible forprocessing events connected to graphic-lib
 *
 * Process events and calls proper Window class methods
 */
class GraphicEventHandler
{
public:

    /**
     * @brief Construct a new Graphic Event Handler object
     *
     * @param window
     */
    GraphicEventHandler(Window* window);

    /// @brief Process given event using proper Window class function
    void processEvent(std::unique_ptr<Event> event);

protected:

    Window* window;
};

#endif //PROI_PROJEKT__GRAPHICEVENTHANDLER_H