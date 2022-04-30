//
// Created by tomek on 26.04.2022.
//

#include "include/Event.h"

const char *EventConstructionException::what() const noexcept {
    return "Could not initialize event instance";
}

Event::Event(EventType e) {
    switch (e) {
        case NullEvent: {
            break;
        }
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, unsigned int i1) {
    switch (e) {
        case Keypress: {
            info.key = {i1};
            break;
        }
        default: {
            throw EventConstructionException();
        }
    }
}