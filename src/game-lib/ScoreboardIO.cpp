//
// Created by tomek on 02.06.2022.
//

#include "include/ScoreboardIO.h"
#include "include/Scoreboard.h"

std::unique_ptr<Scoreboard> ScoreboardIO::loadScoreboard() {
    // dummy
    return std::make_unique<Scoreboard>(0);
}

void ScoreboardIO::saveScoreboard(std::unique_ptr<Scoreboard> scoreboard) {
    // dummy
}