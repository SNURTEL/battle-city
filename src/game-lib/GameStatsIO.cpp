//
// Created by tomek on 02.06.2022.
//

#include "include/GameStatsIO.h"
#include "include/GameStats.h"

std::unique_ptr<GameStats> GameStatsIO::loadScoreboard() {
    // dummy
    return std::make_unique<GameStats>(0, 1, 3);
}

void GameStatsIO::saveScoreboard(std::unique_ptr<GameStats> scoreboard) {
    // dummy
}