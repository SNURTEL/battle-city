//
// Created by Rafal on 01.06.2022.
//


#include <utility>
#include <iostream>

#include "include/Scoreboard.h"
#include "include/ScoreboardIO.h"


void Scoreboard::add_points(unsigned int pts) {
    points_ += pts;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::PointsChanged, this, points_));
}

void Scoreboard::set_points(unsigned int pts) {
    points_ = pts;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::PointsChanged, this, points_));
}

unsigned int Scoreboard::get_points() {
    return points_;
}

Scoreboard::Scoreboard(unsigned int start_pts) {
    points_ = start_pts;
}