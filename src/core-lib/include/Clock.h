//
// Created by tomek on 26.04.2022.
//

#ifndef PROI_PROJEKT_CLOCK_H
#define PROI_PROJEKT_CLOCK_H

#include <memory>
#include <chrono>

#include "SimplePublisher.h"

class Clock: public SimplePublisher{
public:
    Clock()=delete;

    Clock(const Clock&)=delete;

    Clock operator=(const Clock&)=delete;

    void tick();

    void sleep();

    static void initialize(unsigned int);

    static Clock* instance();

private:
    explicit Clock(unsigned int);

    std::chrono::time_point<std::chrono::steady_clock> last_tick_;
    std::chrono::nanoseconds interval_{};

    static std::unique_ptr<Clock> self_;
};


#endif //PROI_PROJEKT_CLOCK_H
