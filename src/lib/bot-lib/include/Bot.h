//
// Created by tomek on 06.06.2022.
//

#ifndef PROI_PROJEKT_BOT_H
#define PROI_PROJEKT_BOT_H

#include "../../tank-lib/include/Entity.h"
#include "../../core-lib/include/SimpleSubscriber.h"

class BotController;

/**
 * Represents an """AI"""-controlled Entity. Managed via BotController
 */
class Bot : virtual public Entity, public SimpleSubscriber,  public std::enable_shared_from_this<Bot>{
public:
    ~Bot() override;

    /**
     * Decreases decision cooldown by 1; queues Event::BotDecisionRequest and re-sets the cooldown after reaching 0
     * @param pub
     */
    void notify(SimplePublisher *pub) override;

    /**
     * Queues an Event::BotDecisionRequest
     */
    void requestDecision();
protected:
    Bot()=default;
    /**
     * Upon creation, subscribes to Clock and increments BotController's bot counter
     * @param x
     * @param y
     * @param sizeX
     * @param sizeY
     * @param speed
     * @param facing
     */
    Bot(float x, float y, float sizeX, float sizeY, float speed, Direction facing);
    unsigned int maxDecisionCooldown;
    unsigned int decisionCooldown;
    BotController* botController;
};


#endif //PROI_PROJEKT_BOT_H
