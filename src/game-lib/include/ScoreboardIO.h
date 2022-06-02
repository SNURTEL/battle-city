//
// Created by tomek on 02.06.2022.
//

#ifndef PROI_PROJEKT_SCOREBOARDIO_H
#define PROI_PROJEKT_SCOREBOARDIO_H

#include <memory>
#include <string>
#include <utility>

class Scoreboard;

class ScoreboardIO {
public:
    ScoreboardIO() = delete;

    explicit ScoreboardIO(std::string n_scoreboardFilename) : scoreboardFilename(std::move(n_scoreboardFilename)) {};

    std::unique_ptr<Scoreboard> loadScoreboard();

    void saveScoreboard(std::unique_ptr<Scoreboard> scoreboard);

private:
    std::string scoreboardFilename;
};


#endif //PROI_PROJEKT_SCOREBOARDIO_H
