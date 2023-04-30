//
// Created by tomek on 07.06.2022.
//

#include "include/Eagle.h"

Eagle::Eagle(unsigned int x, unsigned int y) : Entity(x, y, 4, 4, 0, North){}

void Eagle::kill() {
    EventQueue<Event>::instance()->registerEvent(std::make_unique<Event>(Event::GameEnded));
}