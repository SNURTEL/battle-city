//
// Created by Rafal on 01.06.2022.
//


#include <utility>
#include <iostream>
#include <algorithm>

#include "include/GameStatistics.h"
#include "include/GameStatsIO.h"


void GameStatistics::addPoints(unsigned int pts) {
    points_ += pts;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StatisticsChanged, this));
}

void GameStatistics::setPoints(unsigned int pts) {
    points_ = pts;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StatisticsChanged, this));
}

unsigned int GameStatistics::getPoints() const {
    return points_;
}

GameStatistics::GameStatistics(unsigned int startPoints, unsigned int startLevel, unsigned int startLives) : points_(startPoints), level_(startLevel), lives_(startLives) {}

unsigned int GameStatistics::getLevel() const {
    return level_;
}

void GameStatistics::setLevel(unsigned int level) {
    level_ = level;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StatisticsChanged, this));
}

void GameStatistics::resetStats() {
    points_ = 0;
    level_ = 1;
    lives_ = 3;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StatisticsChanged, this));
}

unsigned int GameStatistics::getLives() const {
    return lives_;
}

void GameStatistics::setLives(unsigned int lives) {
    lives_ = lives;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StatisticsChanged, this));

    if(lives_ == 0){
        eventQueue_->registerEvent(std::make_unique<Event>(Event::GameEnded, this));
    }
}

void GameStatistics::decrementLives(unsigned int deltaLives) {
    setLives(std::min(0u, lives_-deltaLives));
}