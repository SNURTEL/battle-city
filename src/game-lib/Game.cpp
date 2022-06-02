//
// Created by tomek on 02.05.2022.
//

#include <SFML/Graphics.hpp>


#include "include/Game.h"
#include "../core-lib/include/Clock.h"  //FIXME GROSS
#include "../core-lib/include/EventQueue.h"
#include "include/GameState.h"
#include "include/KeyboardController.h"
#include "include/Scoreboard.h"
#include "include/ScoreboardIO.h"

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
    keyboardController_ = std::make_unique<KeyboardController>(window_.get());
    keyboardController_->subscribe(clock_);

    scoreboardIO = std::make_unique<ScoreboardIO>("scoreboard.txt");  // dummy, filename is ignored for now
}

void Game::initScoreboard() {
    scoreboard = std::move(scoreboardIO->loadScoreboard());
}

void Game::initUI() {
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(400, 400), ":D");
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
    running_ = false;
}

void Game::run() {
    setup();

    while (running_ == true){
        clock_->tick();
        while (!eventQueue_->isEmpty()){
            state_->getEventHandler()->handleEvent(std::move(eventQueue_->pop()));
        }

        //redraw UI
        clock_->sleep();
    }
}

GameState* Game::getState() {
    return state_;
}

Scoreboard* Game::getScoreboard() {
    return scoreboard.get();
}