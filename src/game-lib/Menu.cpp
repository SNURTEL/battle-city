//
// Created by Rafal on 31.05.2022.
//

#include "include/Menu.h"

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

unsigned int Menu::get_max_pos() {
    return max_pos_;
}

void Menu::notify() {
    eventQueue_->registerEvent(std::make_unique<Event>(Event::MenuSelectionChange, this, pos_));
}

void Menu::notify_enter() {
    eventQueue_->registerEvent(std::make_unique<Event>(Event::MenuEnterClicked, this, pos_));
}