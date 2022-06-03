//
// Created by Rafal on 01.06.2022.
//

#ifndef PROI_PROJEKT_POINTSYSTEM_H
#define PROI_PROJEKT_POINTSYSTEM_H

#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

/**
 * Class representing point system in the game
 */
class PointSystem {
    public:
        /**
         * Creates point system
         * @param start_pts
         */
        explicit PointSystem(unsigned int start_pts);

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

    private:
        unsigned int points_;
        EventQueue<Event>* eventQueue_ = EventQueue<Event>::instance();
};



#endif