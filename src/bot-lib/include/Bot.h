//
// Created by tomek on 06.06.2022.
//

#ifndef PROI_PROJEKT_BOT_H
#define PROI_PROJEKT_BOT_H

#include "../../tank-lib/include/Entity.h"
#include "../../core-lib/include/SimpleSubscriber.h"

class BotController;

class Bot : public SimpleSubscriber, public Entity, public std::enable_shared_from_this<Bot>{
public:
    ~Bot() override;

    void notify(SimplePublisher *pub) override;

    void requestDecision();
protected:
    Bot()=default;
    Bot(float x, float y, float sizeX, float sizeY, float speed, Direction facing);
    unsigned int maxDecisionCooldown;
    unsigned int decisionCooldown;
    BotController* botController;
};


#endif //PROI_PROJEKT_BOT_H
