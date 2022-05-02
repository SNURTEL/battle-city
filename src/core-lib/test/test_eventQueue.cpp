//
// Created by tomek on 29.04.2022.
//
#include <memory>

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/EventQueue.h"
#include "../include/Event.h"


// Singletons cannot be simply extended this way, as ::instance() would always return the base class.
// The solution is to create a base AbstractSingleton class without the ::instance() method and define ::instance() in
// derived classes, but I'm not sure if this will be ever needed in this project

//template <class E>
//class TestEventQueue: public EventQueue<E> {
//public:
//    std::queue<std::unique_ptr<E>>* getQueue() {
//        return &(this->events_);
//    }
//};

SCENARIO("Event queue - Regular behavior") {
    GIVEN("An empty event queue"){
        EventQueue<Event>* queue = EventQueue<Event>::instance();

        WHEN("Adding events to queue"){
            REQUIRE_NOTHROW(queue->registerEvent(std::make_unique<Event>(Event::KeyPressed, 123, Event::KeyEventInfo::KeyAction::Pressed)));
            REQUIRE_NOTHROW(queue->registerEvent(std::make_unique<Event>(Event::KeyPressed, 234, Event::KeyEventInfo::KeyAction::Pressed)));
            REQUIRE_NOTHROW(queue->registerEvent(std::make_unique<Event>(Event::NullEvent)));

            THEN("Events should be added correctly") {
//                REQUIRE(queue->getQueue()->size() == 3);

                AND_WHEN("Events are removed from queue"){
                    THEN("Events should be removed duh"){
                        REQUIRE(queue->pop()->info.key.keyCode == 123);
//                        REQUIRE(queue->getQueue()->size() == 2);
                        REQUIRE(queue->pop()->info.key.keyCode == 234);
//                        REQUIRE(queue->getQueue()->size() == 1);
                        queue->pop();
                        REQUIRE(queue->isEmpty());
                    }
                }
            }
        }
    }
}

SCENARIO("Event queue - singleton-specific behavior") {
    WHEN("Instantiating the queue multiple times"){
        EventQueue<Event>* myQueue = EventQueue<Event>::instance();
        EventQueue<Event>* myOtherQueue = EventQueue<Event>::instance();

        THEN("All pointers should point to the same object") {
            REQUIRE(myQueue == myOtherQueue);
            REQUIRE(myQueue != nullptr);

            AND_WHEN("Instantiating a queue for different event types") {
                class ExtendedEvent: public Event {};

                EventQueue<ExtendedEvent>* anotherQueue = EventQueue<ExtendedEvent>::instance();

                THEN("Separate instances should be created") {
                    REQUIRE(anotherQueue != nullptr);
                    myQueue->registerEvent(std::make_unique<Event>(Event::KeyPressed, 234, Event::KeyEventInfo::KeyAction::Pressed));
                    REQUIRE_FALSE(myQueue->isEmpty());
                    REQUIRE(anotherQueue->isEmpty());
                }
            }
        }
    }
}

