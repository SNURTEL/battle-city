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

Event::Event(EventType e, Entity *entity) {
    type = e;
    switch (e) {
        case EntitySpawned:
        case EntityMoved:
        case EntityRemoved:
        case TankRotated:
        case TankHit:
        case TankKilled: {
            info.entityInfo = {entity};
            break;
        }
        default: {
            throw EventConstructionException();
        }
    }
}

Event::Event(EventType e, Entity *entity1, Entity *entity2) {
    type = e;
    switch (e) {
        case EntityEntityCollision:{
            info.entityEntityCollisionInfo = {entity1, entity2};
            break;
        }
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, Entity *entity, unsigned int x, unsigned int y) {
    type = e;
    switch (e) {
        case EntityGridCollision:{
            info.entityGridCollisionInfo = {entity, x, y};
            break;
        }
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, unsigned int x, unsigned int y, Grid *grid) {
    type = e;
    switch (e) {
        case TilePlaced:
        case TileChanged:
        case TileDeleted: {
            info.tileInfo = {x, y, grid};
            break;
        };
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, unsigned int levelNumber, Grid *grid) {
    type = e;
    switch (e) {
        case LevelLoaded: {
            info.levelInfo = {levelNumber, grid};
            break;
        }
        default:
            throw EventConstructionException();
    }
}