//
// Created by Rafal on 01.06.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/GameState.h"
#include "../include/Game.h"
#include "../include/ActiveEventHandler.h"
#include "../include/FinishedEventHandler.h"
#include "../include/MenuEventHandler.h"
#include "../include/PauseEventHandler.h"
#include "../../core-lib/include/Event.h"

//TODO test the constructor lmao
namespace helper {
    EventQueue<Event> *getEmptyEventQueue() {
        EventQueue<Event> *eventQueue = EventQueue<Event>::instance();
        eventQueue->clear();
        return eventQueue;
    }

}


