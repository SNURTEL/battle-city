//
// Created by tomek on 02.05.2022.
//

#include "include/GameState.h"
#include "include/ActiveEventHandler.h"
#include "include/FinishedEventHandler.h"
#include "include/PauseEventHandler.h"
#include "include/MenuEventHandler.h"
#include "include/Menu.h"
#include "../tank-lib/include/Tank.h"


PublisherEventHandler *GameState::getEventHandler() {
    return eventHandler_.get();
}

ActiveGameState::ActiveGameState(Game* game) : GameState(game, std::make_unique<ActiveEventHandler>(game, this)) {}

Board* ActiveGameState::get_board() {
    return board_;
}

std::shared_ptr<PlayerTank> ActiveGameState::get_player_tank() {
    return player_tank_;
}

PauseGameState::PauseGameState(Game* game) : GameState(game, std::make_unique<PauseEventHandler>(game, this)), menu_(std::make_unique<Menu>(2)) {}
MenuGameState::MenuGameState(Game* game) : GameState(game, std::make_unique<MenuEventHandler>(game, this)), menu_(std::make_unique<Menu>(2)) {}


Menu* MenuGameState::get_menu() {
    return menu_.get();
}

Menu* PauseGameState::get_menu() {
    return menu_.get();
}

FinishedGameState::FinishedGameState(Game* game) : GameState(game, std::make_unique<FinishedEventHandler>(game, this)) {}