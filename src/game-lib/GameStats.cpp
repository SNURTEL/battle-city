//
// Created by Rafal on 01.06.2022.
//


#include <utility>
#include <iostream>

#include "include/GameStats.h"
#include "include/GameStatsIO.h"


void GameStats::addPoints(unsigned int pts) {
    points_ += pts;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StatsChanged, this));
}

void GameStats::setPoints(unsigned int pts) {
    points_ = pts;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StatsChanged, this));
}

unsigned int GameStats::getPoints() const {
    return points_;
}

GameStats::GameStats(unsigned int startPoints, unsigned int startLevel, unsigned int startLives) : points_(startPoints), level_(startLevel), lives_(startLives) {}

unsigned int GameStats::getLevel() const {
    return level_;
}

void GameStats::setLevel(unsigned int level) {
    level_ = level;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StatsChanged, this));
}

void GameStats::resetStats() {
    points_ = 0;
    level_ = 1;
    lives_ = 3;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StatsChanged, this));
}

unsigned int GameStats::getLives() const {
    return lives_;
}

void GameStats::setLives(unsigned int lives) {
    lives_ = lives;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::StatsChanged, this));
}
