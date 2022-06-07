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
        case GameEnded:
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
            info.dataStructs.keyInfo = {i1, KeyEventInfo::Pressed};
            break;
        }
        case (KeyReleased): {
            info.dataStructs.keyInfo = {i1, KeyEventInfo::Released};
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
            info.dataStructs.entityInfo = {std::move(entity)};
            info.entity_ptr = &(info.dataStructs.entityInfo.entity);
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
            info.dataStructs.tileInfo = {x, y, grid};
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
            info.dataStructs.levelInfo = {levelNumber, grid};
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
            info.dataStructs.menuInfo = {menu, new_pos};
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
            info.dataStructs.stateInfo = {new_state};
            break;
        }
        default:
            throw EventConstructionException();
    }
}

Event::Event(EventType e, GameStatistics* statsObject) {
    type = e;
    switch (e) {
        case StatisticsChanged:{
            info.dataStructs.pointsInfo = {statsObject};
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
            info.dataStructs.botInfo = {std::move(bot)};
            info.bot_ptr = &(info.dataStructs.botInfo.bot);
            break;
        }
        case BotFireDecision:{
            info.dataStructs.fireDecisionInfo = {std::move(bot)};
            info.bot_ptr = &(info.dataStructs.botInfo.bot);
            break;
        }
        default: {
            throw EventConstructionException();
        }
    }
}

Event::Event(EventType e, unsigned int x, unsigned int y, Tank::TankType tankType, Direction direction) {
    type = e;
    switch (e) {
        case BotSpawnDecision: {
            info.dataStructs.spawnDecisionInfo = {x, y, tankType, direction};
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
            info.dataStructs.rotateDecisionInfo = {std::move(bot), direction};
            info.bot_ptr = &(info.dataStructs.botInfo.bot);
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
            info.dataStructs.moveDecisionInfo = {std::move(bot),flag};
            info.bot_ptr = &(info.dataStructs.botInfo.bot);
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
            info.dataStructs.collisionInfo = {std::move(mem1), std::move(mem2)};
            break;
        }
        default:
            throw EventConstructionException();
    }
}
