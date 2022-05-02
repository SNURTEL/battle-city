//
// Created by tomek on 02.05.2022.
//
#include <iostream>

#include "include/Game.h"
#include "../core-lib/include/Clock.h"  //FIXME GROSS
#include "../core-lib/include/EventQueue.h"
#include "include/GameState.h"
#include "include/KeyboardController.h"

Game::Game(unsigned int clock_freq) {
    active_state_ = std::make_unique<ActiveGameState>(this);
    Clock::initialize(clock_freq);
    clock_ = Clock::instance();
    eventQueue_ = EventQueue<Event>::instance();
}

void Game::setup() {
    // load scoreboard, init UI, etC
    createRenderWindow();
    keyboardController_ = std::make_unique<KeyboardController>(window_.get());
    keyboardController_->subscribe(clock_);
}

void Game::createRenderWindow() {
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(200, 200), ":D");
}

void Game::setActiveState() {
    state_ = active_state_.get();
}

void Game::run() {
    setup();
    setActiveState();

    while (true){
        clock_->tick();
        while (!eventQueue_->isEmpty()){
            state_->getEventHandler()->handleEvent(std::move(eventQueue_->pop()));
        }

        //redraw UI
        clock_->sleep();
    }
}