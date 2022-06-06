//
// Created by tomek on 02.05.2022.
//

#include <SFML/Graphics.hpp>


#include "include/Game.h"
#include "../core-lib/include/Clock.h"  //FIXME GROSS
#include "../core-lib/include/EventQueue.h"
#include "include/GameState.h"
#include "include/KeyboardController.h"
#include "include/GameStatistics.h"
#include "include/GameStatsIO.h"
#include "../board-lib/include/Board.h"
#include "../board-lib/include/Grid.h"
#include "../bot-lib/include/BotController.h"


Game::Game(unsigned int clockFreq) {
    active_state_ = std::make_unique<ActiveGameState>(this);
    finished_state_ = std::make_unique<FinishedGameState>(this);
    pause_state_ = std::make_unique<PauseGameState>(this);
    menu_state_ = std::make_unique<MenuGameState>(this);
    Clock::initialize(clockFreq);
    clock_ = Clock::instance();
    eventQueue_ = EventQueue<Event>::instance();
}

void Game::setup() {
    // load scoreboard, init UI, etC
    initUI();

    initStates();
    initComponents();
    initScoreboard();

    setMenuState();
    running_ = true;
}

void Game::initStates() {
    active_state_ = std::make_unique<ActiveGameState>(this);
    pause_state_ = std::make_unique<PauseGameState>(this);
    finished_state_ = std::make_unique<FinishedGameState>(this);
    menu_state_ = std::make_unique<MenuGameState>(this);
}

void Game::initComponents() {
    keyboardController_ = std::make_unique<KeyboardController>(window_->getWindow());
    keyboardController_->subscribe(clock_);

    gameStatsIO_ = std::make_unique<GameStatsIO>("scoreboard.txt");  // dummy, filename is ignored for now

    BotController::initialize(4, 240);

    board_ = std::make_unique<Board>();
}

void Game::initScoreboard() {
    gameStats_ = std::move(gameStatsIO_->loadScoreboard());
}

void Game::initUI() {
    window_ = std::make_unique<Window>();
}

void Game::setActiveState() {
    state_ = active_state_.get();
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StateChanged, state_));
}

void Game::setFinishedState() {
    state_ = finished_state_.get();
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StateChanged, state_));
}

void Game::setMenuState() {
    state_ = menu_state_.get();
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StateChanged, state_));
}

void Game::setPauseState() {
    state_ = pause_state_.get();
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StateChanged, state_));
}

void Game::quit() {
    gameStatsIO_->saveScoreboard(std::move(gameStats_));
    running_ = false;
}

void Game::run() {
    setup();

    while (running_ == true) {
        clock_->tick();
        while (!eventQueue_->isEmpty()) {
            state_->getEventHandler()->handleEvent(std::move(eventQueue_->pop()));
        }

        redrawUI();

        clock_->sleep();
    }
}

GameState *Game::getState() {
    return state_;
}

GameStatistics *Game::getStats() {
    return gameStats_.get();
}

void Game::start() {
    reset();
    prepareLevel(1);
    setActiveState();
}

void Game::reset() {
    board_->removeAllEntities();
    gameStats_->resetStats();
}

void Game::prepareLevel(unsigned int levelNum) {
    board_->removeAllEntities();
    board_->loadLevel(levelNum);
    board_->spawnPlayer(18, 48);
}

void Game::end() {
    board_->removeAllEntities();
    setFinishedState();
}


void Game::redrawUI() {
    window_->getWindow()->clear(sf::Color::Black);
    window_->render();
    window_->getWindow()->display();
}