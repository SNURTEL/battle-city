//
// Created by tomek on 29.04.2022.
//

#include <memory>

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/EventPublisher.h"
#include "../include/EventSubscriber.h"
#include "../include/event.h"
#include "../include/ObserverExceptions.h"

class TestEventPublisher : public EventPublisher {
public:
    int notification_count = 0;

    void notifyEventSubscribers(Event::EventType eventType) override {
        for (EventSubscriber *sub: event_subscribers_[eventType]) {
            sub->notify(this, eventType);
            notification_count++;
        }
    }
};

class TestEventSubscriber : public EventSubscriber {
public:
    bool notified1 = false;
    bool notified2 = false;

    void notify(EventPublisher *pub, Event::EventType eventType) override {
        if(eventType==Event::KeyPressed){
            notified1= true;
        }else if(eventType == Event::NullEvent){
            notified2 = true;
        }
    }

    unsigned int getSubscribedSubjectsCount(Event::EventType eventType) {
        return subscribedSubjects_[eventType].size();
    }
};

SCENARIO("Event observer - Regular behavior"){
    GIVEN("A simple subscriber - publisher setup") {
        std::shared_ptr<TestEventPublisher> publisher = std::make_shared<TestEventPublisher>();
        std::shared_ptr<TestEventSubscriber> subscriber1 = std::make_shared<TestEventSubscriber>();
        std::shared_ptr<TestEventSubscriber> subscriber2 = std::make_shared<TestEventSubscriber>();

        WHEN("Subscribed to an event"){
            subscriber1->subscribe(publisher.get(), Event::KeyPressed);
            subscriber2->subscribe(publisher.get(), Event::NullEvent);

            THEN("A correct subscriber should be notified when an event is registered"){
                REQUIRE_NOTHROW(publisher->notifyEventSubscribers(Event::KeyPressed));
                REQUIRE(publisher->notification_count==1);
                REQUIRE(subscriber1->notified1 == true);
                REQUIRE(subscriber2->notified2 == false);

                REQUIRE_NOTHROW(publisher->notifyEventSubscribers(Event::NullEvent));
                REQUIRE(publisher->notification_count==2);
                REQUIRE(subscriber2->notified2 == true);
            }

        }

        WHEN("Unsubscribing") {
            subscriber1->subscribe(publisher.get(), Event::KeyPressed);
            REQUIRE_NOTHROW(subscriber1->subscribe(publisher.get(), Event::KeyPressed));
            THEN("Subscriber should not be notified"){
                REQUIRE(subscriber1->notified1==false);
            }
        }

        WHEN("Subscribed to multiple events"){
            subscriber1->subscribe(publisher.get(), Event::KeyPressed);
            subscriber1->subscribe(publisher.get(), Event::NullEvent);

            publisher->notifyEventSubscribers(Event::KeyPressed);
            publisher->notifyEventSubscribers(Event::NullEvent);
            THEN("Subscriber should be notified multiple times"){
                REQUIRE(subscriber1->notified1 == true);
                REQUIRE(subscriber1->notified2 == true);
            }
        }
    }
}


SCENARIO("Event observer - Edge cases") {
    GIVEN("A simple publisher - subscriber setup") {
        std::shared_ptr<TestEventPublisher> publisher = std::make_shared<TestEventPublisher>();
        std::shared_ptr<TestEventSubscriber> subscriber1 = std::make_shared<TestEventSubscriber>();
        std::shared_ptr<TestEventSubscriber> subscriber2 = std::make_shared<TestEventSubscriber>();

        WHEN("Subscribing the same publisher twice") {
            subscriber1->subscribe(publisher.get(), Event::KeyPressed);
            REQUIRE_NOTHROW(subscriber1->subscribe(publisher.get(), Event::KeyPressed));

            THEN("Subscriber should only be notified once") {
                publisher->notifyEventSubscribers(Event::KeyPressed);
                REQUIRE(publisher->notification_count == 1);
                REQUIRE(subscriber1->getSubscribedSubjectsCount(Event::KeyPressed) == 1);
            }
        }

        WHEN("Trying to unsubscribe when not subscribed"){
            THEN("An exception should be thrown") {
                REQUIRE_THROWS_AS(subscriber1->unsubscribe(publisher.get(), Event::NullEvent), ObserverException);
            }
        }
    }
    GIVEN("Objects that can be de-initialized at runtime"){
        auto publisher1 = new TestEventPublisher();
        auto publisher2 = new TestEventPublisher();
        auto subscriber1 = new TestEventSubscriber();
        auto subscriber2 = new TestEventSubscriber();

        WHEN("Subscriber is de-initialized") {
            subscriber1->subscribe(publisher1, Event::KeyPressed);
            subscriber1->subscribe(publisher2, Event::NullEvent);
            subscriber2->subscribe(publisher1, Event::KeyPressed);

            THEN("Subscriber should be removed from the pool") {
                REQUIRE_NOTHROW(delete subscriber1);
                REQUIRE_NOTHROW(publisher1->notifyEventSubscribers(Event::KeyPressed));
                REQUIRE(publisher1->notification_count == 1);
                REQUIRE_NOTHROW(publisher2->notifyEventSubscribers(Event::NullEvent));
                REQUIRE(publisher2->notification_count == 0);
            }
        }

        WHEN("Publisher is de-initialized") {
            subscriber1->subscribe(publisher1, Event::KeyPressed);
            subscriber2->subscribe(publisher1, Event::NullEvent);

            REQUIRE(subscriber1->getSubscribedSubjectsCount(Event::KeyPressed) == 1);
            REQUIRE(subscriber2->getSubscribedSubjectsCount(Event::NullEvent) == 1);

            THEN("Publisher should be removed from subscribers' pool") {
                REQUIRE_NOTHROW(delete publisher1);
                REQUIRE(subscriber1->getSubscribedSubjectsCount(Event::KeyPressed) == 0);
                REQUIRE(subscriber2->getSubscribedSubjectsCount(Event::NullEvent) == 0);
            }

        }
    }
}