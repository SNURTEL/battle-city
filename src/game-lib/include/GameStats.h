//
// Created by Rafal on 01.06.2022.
//

#ifndef PROI_PROJEKT_GAMESTATS_H
#define PROI_PROJEKT_GAMESTATS_H

#include <list>

#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

class GameStatsIO;

/**
 * Class representing point system in the game
 */
class GameStats {
public:
    /**
     * Creates point system
     * @param startPoints
     */
    GameStats(unsigned int startPoints, unsigned int startLevel, unsigned int startLives);

    /**
     * Returns points
     * @return points
     */
    [[nodiscard]] unsigned int getPoints() const;

    /**
     * Sets points
     * @param points
     */
    void setPoints(unsigned int pts);

    /**
     * Adds points to current points
     * @param points
     */
    void addPoints(unsigned int pts);

    [[nodiscard]] unsigned int getLevel() const;

    void setLevel(unsigned int level);

    [[nodiscard]] unsigned int getLives() const;

    void setLives(unsigned int lives);

    void resetStats();

    friend class ScoreboardIO;

private:
    unsigned int points_;
    unsigned int level_;
    unsigned int lives_;

private:
    std::list<unsigned int> scoreboard_;

    EventQueue<Event> *eventQueue_ = EventQueue<Event>::instance();
};


#endif