//
// Created by tomek on 06.06.2022.
//

#ifndef PROI_PROJEKT_BOTCONTROLLER_H
#define PROI_PROJEKT_BOTCONTROLLER_H

#include <memory>
#include <vector>
#include <queue>

#include "../../core-lib/include/SimpleSubscriber.h"
#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

class Bot;

/**
 * Exception thrown when trying to spawn a bot, but no spawnpoints were given
 */
class NoSpawnpointException : public std::exception {
    [[nodiscard]] const char * what () const noexcept override;
};

/**
 * Manages Bot objects
 */
class BotController : public SimpleSubscriber {
public:
    BotController()=delete;

    BotController& operator=(const BotController &other)=delete;

    /**
     * Analyzes bot's state and makes a decision about what should it do
     *
     * Queues Event::BotMove/Rotate/FireDecision
     * @param bot
     */
    void makeBotDecision(const std::shared_ptr<Bot>& bot);

    /**
     * Increments internal bot counter
     */
    void registerBot();

    /**
     * Decrements internal bot counter
     */
    void deregisterBot();

    /**
     * Sets the internal bot counter to 0
     */
    void deregisterAllBots();

    /**
     * Returns counter's value
     * @return
     */
    [[nodiscard]] unsigned int getRegisteredBotsCount() const;

    /**
     * Decreases spawn cooldown by 1; queues Event::BotSpawnDecision and resets the counter if reached 0
     * @param pub
     */
    void notify(SimplePublisher *pub) override;

    /**
     * Sets available spawnpoints
     * @param spawnpoints
     */
    void setSpawnpoints(const std::vector<std::pair<unsigned int, unsigned int>> &spawnpoints);

    /**
     * Returns controller's sole instance. ::initialize() must be called earlier
     * @return
     */
    static BotController* instance();

    /**
     * Inits the controller
     * @param maxRegisteredBots
     * @param spawnCooldown
     */
    static void initialize(unsigned int maxRegisteredBots, unsigned int spawnCooldown);

    /**
     * Sets the TankType sequence
     * @param types
     */
    void setTypes(const std::queue<Tank::TankType> &types);

protected:
    /**
     * Upon creation, subscribes to Clock
     * @param n_maxRegisteredBots
     * @param n_spawnCooldown
     */
    BotController(unsigned int n_maxRegisteredBots, unsigned int n_spawnCooldown);

    /**
     * If spawnpoints and TankTypes are available, constructs and queues Event::BotSpawnDecision
     */
    void requestSpawnBot();
    std::vector<std::pair<unsigned int, unsigned int>> spawnpoints_;
    std::queue<Tank::TankType> types_{};

    unsigned int maxSpawnCooldown;
    unsigned int spawnCooldown;
    unsigned int maxRegisteredBots_;
    unsigned int registeredBots_;

    static std::unique_ptr<BotController> self_;

    EventQueue<Event>* eventQueue_;

};


#endif //PROI_PROJEKT_BOTCONTROLLER_H
