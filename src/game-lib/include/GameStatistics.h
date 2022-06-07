//
// Created by Rafal on 01.06.2022.
//

#ifndef PROI_PROJEKT_GAMESTATISTICS_H
#define PROI_PROJEKT_GAMESTATISTICS_H

#include <list>

#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

class GameStatsIO;

/**
 * Class representing point system in the game
 */
class GameStatistics {
public:
    /**
     * Creates point system
     * @param startPoints
     */
    GameStatistics(unsigned int startPoints, unsigned int startLevel, unsigned int startLives);

    /**
     * Returns points
     * @return points
     */
    [[nodiscard]] unsigned int getPoints() const;

    /**
     * Sets points
     *
     * Queues Event::StatisticsChanged
     * @param points
     */
    void setPoints(unsigned int pts);

    /**
     * Adds points to current points
     *
     * Queues Event::StatisticsChanged
     * @param points
     */
    void addPoints(unsigned int pts);

    /**
     * Returns the current level
     * @return
     */
    [[nodiscard]] unsigned int getLevel() const;

    /**
     * Sets the current level
     *
     * Queues Event::StatisticsChanged
     * @param level
     */
    void setLevel(unsigned int level);

    /**
     * Returns the number of remaining lives
     * @return
     */
    [[nodiscard]] unsigned int getLives() const;

    /**
     * Sets the number of remaining lives
     *
     * Queues Event::StatisticsChanged
     * @param lives
     */
    void setLives(unsigned int lives);

    void decrementLives(unsigned int deltaLives);

    /**
     * Resets stats to:
     * Score = 0
     * Level = 1
     * Lives = 3
     *
     * Queues Event::StatisticsChanged
     */
    void resetStats();

    friend class ScoreboardIO;

private:
    unsigned int points_;
    unsigned int level_;
    unsigned int lives_;

    std::list<unsigned int> scoreboard_;

    EventQueue<Event> *eventQueue_ = EventQueue<Event>::instance();
};


#endif