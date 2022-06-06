//
// Created by tomek on 26.04.2022.
//
#include <memory>
#include <utility>

#include "include/Event.h"
#include "../tank-lib/include/Entity.h"
#include "../bot-lib/include/Bot.h"

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
        case (LevelLoaded): {
            info.levelInfo.levelNumber = {i1};
            break;
        }
        default: {
            throw EventConstructionException();
        }
    }
}

Event::Event(EventType e, std::shared_ptr<Entity> entity) {
    type = e;
    switch (e) {
        case EntitySpawned:
        case EntityMoved:
        case EntityRemoved:
        case TankRotated:
        case TankHit:
        case TankKilled:
        case PlayerSpawned:
        case PlayerKilled:{
            info.entityInfo = {std::move(entity)};
            break;
        }
        default: {
            throw EventConstructionException();
        }
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

Event::Event(EventType e, Menu* menu, unsigned int new_pos) {
    type = e;
    switch (e) {
        case MenuSelectionChange:
        case MenuEnterClicked: {
            info.menuInfo = {menu, new_pos};
            break;
        }
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, GameState* new_state) {
    type = e;
    switch (e) {
        case StateChanged: {
            info.stateInfo = {new_state};
            break;
        }
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, GameStatistics* statsObject) {
    type = e;
    switch (e) {
        case StatisticsChanged: {
            info.pointsInfo = {statsObject};
            break;
        }
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, std::shared_ptr<Bot> bot) {
    type = e;
    switch (e) {
        case BotDecisionRequest:{
            info.botInfo = {std::move(bot)};
            break;
        }
        case BotFireDecision:{
            info.fireDecisionInfo = {std::move(bot)};
            break;
        }
        default: {
            throw EventConstructionException();
        }
    }
}

Event::Event(EventType e, unsigned int x, unsigned int y, Direction direction) {
    type = e;
    switch (e) {
        case BotSpawnDecision: {
            info.spawnDecisionInfo = {x, y, direction};
            break;
        }
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, std::shared_ptr<Bot> bot, Direction direction) {
    type = e;
    switch (e) {
        case BotRotateDecision: {
            info.rotateDecisionInfo = {std::move(bot), direction};
            break;
        }
        default:
            throw EventConstructionException();
    }
}


Event::Event(EventType e, std::shared_ptr<Bot> bot, bool flag) {
    type = e;
    switch (e) {
        case BotMoveDecision: {
            info.moveDecisionInfo = {std::move(bot),flag};
            break;
        }
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, CollisionMember mem1, CollisionMember mem2) {
    type = e;
    switch (e) {
        case Collision: {
            info.collisionInfo = {std::move(mem1), std::move(mem2)};
            break;
        }
        default:
            throw EventConstructionException();
    }
}
