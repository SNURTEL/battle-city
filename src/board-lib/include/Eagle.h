//
// Created by tomek on 07.06.2022.
//

#ifndef PROI_PROJEKT_EAGLE_H
#define PROI_PROJEKT_EAGLE_H

#include "../../tank-lib/include/Entity.h"

class Eagle : public Entity {
public:
    bool move() override { return false; };

    bool moveBack() override { return false; };
};


#endif //PROI_PROJEKT_EAGLE_H
