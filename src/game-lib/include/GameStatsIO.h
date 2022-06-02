//
// Created by tomek on 02.06.2022.
//

#ifndef PROI_PROJEKT_GAMESTATSIO_H
#define PROI_PROJEKT_GAMESTATSIO_H

#include <memory>
#include <string>
#include <utility>

class GameStats;

class GameStatsIO {
public:
    GameStatsIO() = delete;

    explicit GameStatsIO(std::string n_scoreboardFilename) : scoreboardFilename(std::move(n_scoreboardFilename)) {};

    std::unique_ptr<GameStats> loadScoreboard();

    void saveScoreboard(std::unique_ptr<GameStats> scoreboard);

private:
    std::string scoreboardFilename;
};


#endif //PROI_PROJEKT_GAMESTATSIO_H
