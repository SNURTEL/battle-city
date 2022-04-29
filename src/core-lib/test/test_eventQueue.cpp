//
// Created by tomek on 29.04.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/Event.h"
#include "../include/EventQueue.h"

template <class E>
class TestEventQueue: public EventQueue<E> {
public:
    std::queue<E>* getQueue() {
        return &(this->events_);
    }
};

SCENARIO("Event queue - Regular behavior") {
    GIVEN("An empty event queue"){
        TestEventQueue<Event> queue{};  // TODO Write tests after implementing Event
    }
}