//
// Created by tomek on 02.05.2022.
//

#include "include/GameState.h"
#include "include/ActiveEventHandler.h"
#include "include/FinishedEventHandler.h"
#include "include/PauseEventHandler.h"
#include "include/MenuEventHandler.h"

PublisherEventHandler *GameState::getEventHandler() {
    return eventHandler_.get();
}

ActiveGameState::ActiveGameState(Game* game) {
    eventHandler_ = std::make_unique<ActiveEventHandler>(game);
}
PauseGameState::PauseGameState(Game* game) {
    eventHandler_ = std::make_unique<PauseEventHandler>(game);
}
MenuGameState::MenuGameState(Game* game) {
    eventHandler_ = std::make_unique<MenuEventHandler>(game);
}
void MenuGameState::set_position(unsigned int new_pos) {
    position_ = new_pos;
}
unsigned int MenuGameState::get_position() {
    return position_;
}
FinishedGameState::FinishedGameState(Game* game) {
    eventHandler_ = std::make_unique<FinishedEventHandler>(game);
}