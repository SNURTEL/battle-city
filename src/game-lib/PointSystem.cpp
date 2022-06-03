//
// Created by Rafal on 01.06.2022.
//


#include <utility>
#include <iostream>

#include "include/PointSystem.h"



void PointSystem::add_points(unsigned int pts) {
    points_ += pts;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::PointsChanged, this, points_));
}

void PointSystem::set_points(unsigned int pts) {
    points_ = pts;
    eventQueue_->registerEvent(std::make_unique<Event>(Event::PointsChanged, this, points_));
}

unsigned int PointSystem::get_points() {
    return points_;
}

PointSystem::PointSystem(unsigned int start_pts) {
    points_ = start_pts;
}