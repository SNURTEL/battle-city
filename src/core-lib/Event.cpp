//
// Created by tomek on 26.04.2022.
//
#include <memory>

#include "include/Event.h"

const char *EventConstructionException::what() const noexcept {
    return "Could not initialize event instance";
}

Event::Event(EventType e) {
    type = e;
    switch (e) {
        case NullEvent: {
            break;
        }
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, unsigned int i1) {
    type = e;
    switch (e) {
        case (KeyPressed): {
            info.keyInfo = {i1, KeyEventInfo::Pressed};
            break;
        }
        case (KeyReleased): {
            info.keyInfo = {i1, KeyEventInfo::Released};
            break;
        }
        default: {
            throw EventConstructionException();
        }
    }
}

Event::Event(EventType e, Tank *tank) {
    type = e;
    switch (e) {
        case TankSpawned:
        case TankMoved:
        case TankHit:
        case TankRemoved:
        case TankKilled:  {
            info.tankInfo = {tank};
            break;
        }
        default: {
            throw EventConstructionException();
        }
    }
}
