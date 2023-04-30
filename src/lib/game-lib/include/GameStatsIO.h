//
// Created by tomek on 02.06.2022.
//

#ifndef PROI_PROJEKT_GAMESTATSIO_H
#define PROI_PROJEKT_GAMESTATSIO_H

#include <memory>
#include <string>
#include <utility>

class GameStatistics;

/**
 * Used for loading and saving scoreboards
 */
class GameStatsIO {
public:
    GameStatsIO() = delete;

    /**
     * Inits class GameStatsIO
     * @param n_scoreboardFilename Path to scoreboard file
     */
    explicit GameStatsIO(std::string n_scoreboardFilename) : scoreboardFilename(std::move(n_scoreboardFilename)) {};

    /**
     * Loads the scoreboard from the given file and returns a GameStatistics instance with scoreboard field filled
     *
     * DOES NOTHING - PLACEHOLDER
     * @return A new scoreboard
     */
    std::unique_ptr<GameStatistics> loadScoreboard();

    /**
     * Extracts and saves the gameStats to given file
     *
     * DOES NOTHING - PLACEHOLDER
     * @param gameStats Stats object containing the scoreboard that should be saved
     */
    void saveScoreboard(std::unique_ptr<GameStatistics> gameStats);

private:
    std::string scoreboardFilename;
};


#endif //PROI_PROJEKT_GAMESTATSIO_H
