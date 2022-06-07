//
// Created by tomek on 07.06.2022.
//

#include "include/Eagle.h"

void Eagle::kill() {
    EventQueue<Event>::instance()->registerEvent(std::make_unique<Event>(Event::GameEnded));
}