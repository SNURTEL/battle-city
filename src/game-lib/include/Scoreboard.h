//
// Created by Rafal on 01.06.2022.
//

#ifndef PROI_PROJEKT_SCOREBOARD_H
#define PROI_PROJEKT_SCOREBOARD_H

#include <list>

#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

class ScoreboardIO;

/**
 * Class representing point system in the game
 */
class Scoreboard {
    public:
        /**
         * Creates point system
         * @param start_pts
         */
        explicit Scoreboard(unsigned int start_pts);

        /**
         * Returns points
         * @return points
         */
        unsigned int get_points();

        /**
         * Sets points
         * @param points
         */
        void set_points(unsigned int pts);

        /**
         * Adds points to current points
         * @param points
         */
        void add_points(unsigned int pts);

        friend class ScoreboardIO;
    private:
        unsigned int points_;
        std::list<unsigned int> scoreboard_;

        EventQueue<Event>* eventQueue_ = EventQueue<Event>::instance();
};



#endif