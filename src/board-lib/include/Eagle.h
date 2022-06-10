//
// Created by tomek on 07.06.2022.
//

#ifndef PROI_PROJEKT_EAGLE_H
#define PROI_PROJEKT_EAGLE_H

#include "../../tank-lib/include/Entity.h"
#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

class Eagle : public Entity {
public:
    /**
     * Inits Eagle class
     * @param x
     * @param y
     */
    Eagle(unsigned int x, unsigned int y);

    bool move() override { return false; };

    bool moveBack() override { return false; };

    /**
     * Immediately queues Event::GameEnded
     */
    void kill();
};


#endif //PROI_PROJEKT_EAGLE_H
