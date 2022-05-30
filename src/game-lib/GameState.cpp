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
    eventHandler_ = std::make_unique<ActiveEventHandler>(game, this);
    playerTank_ = std::make_unique<PlayerTank>(10, 10, 5);
}

PlayerTank* ActiveGameState::get_player_tank() {
    return playerTank_.get();
}

PauseGameState::PauseGameState(Game* game) {
    eventHandler_ = std::make_unique<PauseEventHandler>(game, this);
}
MenuGameState::MenuGameState(Game* game) {
    eventHandler_ = std::make_unique<MenuEventHandler>(game, this);
    menu_ = std::make_unique<Menu>(4);
}
Menu* MenuGameState::get_menu() {
    return menu_.get();
}
FinishedGameState::FinishedGameState(Game* game) {
    eventHandler_ = std::make_unique<FinishedEventHandler>(game, this);
}


Menu::Menu(unsigned int max_pos) {
    max_pos_ = max_pos;
}

void Menu::add_pos() {
    pos_++;
    if (pos_ > max_pos_) pos_ = 1;
}

void Menu::sub_pos() {
    pos_--;
    if (pos_ < 1) pos_ = max_pos_;
}

void Menu::set_pos(unsigned int pos) {
    if (pos > max_pos_)
        return;
    pos_ = pos;
}

unsigned int Menu::get_pos() {
    return pos_;
}