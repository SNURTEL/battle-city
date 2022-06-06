//
// Created by tomek on 06.06.2022.
//

#ifndef PROI_PROJEKT_BOTCONTROLLER_H
#define PROI_PROJEKT_BOTCONTROLLER_H

#include <memory>
#include <vector>

#include "../../core-lib/include/SimpleSubscriber.h"
#include "../../core-lib/include/EventQueue.h"
#include "../../core-lib/include/Event.h"

class Bot;

class NoSpawnpointException : public std::exception {
    [[nodiscard]] const char * what () const noexcept override;
};

class BotController : public SimpleSubscriber {
public:
    BotController()=delete;

    BotController& operator=(const BotController &other)=delete;

    void makeBotDecision(const std::shared_ptr<Bot>& bot);

    void registerBot();

    void deregisterBot();

    void deregisterAllBots();

    unsigned int getRegisteredBotsCount() const;

    void notify(SimplePublisher *pub) override;

    void setSpawnpoints(const std::vector<std::pair<unsigned int, unsigned int>> &spawnpoints);

    static BotController* instance();

    static void initialize(unsigned int maxRegisteredBots, unsigned int spawnCooldown);

protected:

    BotController(unsigned int n_maxRegisteredBots, unsigned int n_spawnCooldown);

    void requestSpawnBot();

    std::vector<std::pair<unsigned int, unsigned int>> spawnpoints_;

    unsigned int maxSpawnCooldown;
    unsigned int spawnCooldown;
    unsigned int maxRegisteredBots_;
    unsigned int registeredBots_;

    static std::unique_ptr<BotController> self_;

    EventQueue<Event>* eventQueue_;

};


#endif //PROI_PROJEKT_BOTCONTROLLER_H
