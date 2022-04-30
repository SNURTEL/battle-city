//
// Created by tomek on 26.04.2022.
//

#ifndef PROI_PROJEKT_CLOCK_H
#define PROI_PROJEKT_CLOCK_H

#include <memory>
#include <chrono>

#include "SimplePublisher.h"

/**
 * \brief Represents an externally controlled clock running at a given frequency.
 *
 * Clock is a singleton publisher class and can be subscribed to by class derived from SimpleSubscriber.
 * Can be used for synchronizing events and managing timings.
 *
 * Clock instance must be initialized before using it with ::initialize(unsigned int), and then it can be accessed
 * with ::instance().
 *
 * Clock calls should happen in a loop:
 * <pre>
 * while(active){
 *     clock->tick()
 *     // stuff happens
 *     clock.sleep()
 * </pre>
 */
class Clock: public SimplePublisher{
public:
    Clock()=delete;

    Clock(const Clock&)=delete;

    Clock operator=(const Clock&)=delete;

    /**
     * Saves the tick timestamp and notifies all subscribers.
     */
    void tick();

    /**
     * Freezes the program until the required amount of time since last ::tick() passes.
     */
    void sleep();

    /**
     * Initializes the clock, must be called before ::instance()
     *
     * @param frequency Frequency at which clock's ticks happen
     */
    static void initialize(unsigned int frequency);

    /**
     * Accesses clock's instance
     *
     * @return A pointer to clock's instance
     */
    static Clock* instance();

private:
    explicit Clock(unsigned int frequency);

    std::chrono::time_point<std::chrono::steady_clock> last_tick_;
    std::chrono::nanoseconds interval_{};

    static std::unique_ptr<Clock> self_;
};


#endif //PROI_PROJEKT_CLOCK_H
