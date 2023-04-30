//
// Created by tomek on 02.06.2022.
//

#include "include/GameStatsIO.h"
#include "include/GameStatistics.h"

std::unique_ptr<GameStatistics> GameStatsIO::loadScoreboard() {
    // dummy
    return std::make_unique<GameStatistics>(0, 1, 3);
}

void GameStatsIO::saveScoreboard(std::unique_ptr<GameStatistics> gameStats) {
    // dummy
}