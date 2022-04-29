//
// Created by tomek on 27.04.2022.
//

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_all.hpp"

#include "../include/SimplePublisher.h"
#include "../include/SimpleSubscriber.h"
#include "../include/ObserverExceptions.h"

class TestSimplePublisher : public SimplePublisher {
public:
    int notification_count = 0;

    void notifySubscribers() override {
        for (SimpleSubscriber *sub: subscribers_) {
            sub->notify(this);
            notification_count++;
        }
    }
};

class TestSimpleSubscriber : public SimpleSubscriber {
public:
    bool notified = false;

    [[nodiscard]] bool isNotified() const {
        return notified;
    }

    void notify(SimplePublisher *pub) override {
        notified = true;
    }

    unsigned int getSubscribedSubjectsCount() {
        return subscribedSubjects_.size();
    }
};


SCENARIO("Simple observer - Regular behavior") {
    GIVEN("A simple publisher - subscriber setup") {
        std::shared_ptr<TestSimplePublisher> publisher = std::make_shared<TestSimplePublisher>();
        std::shared_ptr<TestSimpleSubscriber> subscriber1 = std::make_shared<TestSimpleSubscriber>();
        std::shared_ptr<TestSimpleSubscriber> subscriber2 = std::make_shared<TestSimpleSubscriber>();

        WHEN("Subscribing to publisher") {
            subscriber1->subscribe(publisher.get());
            subscriber2->subscribe(publisher.get());
            THEN("Subscribers should be notified correctly") {

                publisher->notifySubscribers();
                REQUIRE(subscriber1->isNotified());
                REQUIRE(subscriber2->isNotified());
                REQUIRE(subscriber1->getSubscribedSubjectsCount()==1);
            }

        }

        WHEN("Unsubscribing") {
            subscriber1->subscribe(publisher.get());
            subscriber2->subscribe(publisher.get());
            REQUIRE_NOTHROW(subscriber1->unsubscribe(publisher.get()));
            THEN("Unsubscribed object should not be notified") {
                publisher->notifySubscribers();
                REQUIRE_FALSE(subscriber1->isNotified());
                REQUIRE(subscriber2->isNotified());
                REQUIRE(subscriber1->getSubscribedSubjectsCount()==0);
            }
        }

        WHEN("There are no subscribers"){
            THEN("Nothing should happen (duh)"){
                REQUIRE_NOTHROW(publisher->notifySubscribers());
            }
        }
    }
}

SCENARIO("Simple observer -  Edge cases") {
    GIVEN("A simple publisher - subscriber setup") {
        std::shared_ptr<TestSimplePublisher> publisher = std::make_shared<TestSimplePublisher>();
        std::shared_ptr<TestSimpleSubscriber> subscriber1 = std::make_shared<TestSimpleSubscriber>();
        std::shared_ptr<TestSimpleSubscriber> subscriber2 = std::make_shared<TestSimpleSubscriber>();

        WHEN("Subscribing the same publisher twice") {
            subscriber1->subscribe(publisher.get());
            REQUIRE_NOTHROW(subscriber1->subscribe(publisher.get()));

            THEN("Subscriber should only be notified once") {
                publisher->notifySubscribers();
                REQUIRE(publisher->notification_count == 1);
                REQUIRE(subscriber1->getSubscribedSubjectsCount()==1);
            }
        }

        WHEN("Trying to unsubscribe when not subscribed"){
            THEN("An exception should be thrown") {
                REQUIRE_THROWS_AS(subscriber1->unsubscribe(publisher.get()), ObserverException);
            }
        }
    }
    GIVEN("Objects that can be de-initialized at runtime"){
        auto publisher1 = new TestSimplePublisher();
        auto publisher2 = new TestSimplePublisher();
        auto subscriber1 = new TestSimpleSubscriber();
        auto subscriber2 = new TestSimpleSubscriber();

        WHEN("Subscriber is de-initialized") {
            subscriber1->subscribe(publisher1);
            subscriber1->subscribe(publisher2);
            subscriber2->subscribe(publisher1);

            THEN("Subscriber should be removed from the pool") {
                REQUIRE_NOTHROW(delete subscriber1);
                REQUIRE_NOTHROW(publisher1->notifySubscribers());
                REQUIRE(publisher1->notification_count == 1);
                REQUIRE_NOTHROW(publisher2->notifySubscribers());
                REQUIRE(publisher2->notification_count == 0);
            }
        }

        WHEN("Publisher is de-initialized") {
            subscriber1->subscribe(publisher1);
            subscriber2->subscribe(publisher1);

            REQUIRE(subscriber1->getSubscribedSubjectsCount() == 1);
            REQUIRE(subscriber2->getSubscribedSubjectsCount() == 1);

            THEN("Publisher should be removed from subscribers' pool") {
                REQUIRE_NOTHROW(delete publisher1);
                REQUIRE(subscriber1->getSubscribedSubjectsCount() == 0);
                REQUIRE(subscriber2->getSubscribedSubjectsCount() == 0);
            }

        }
    }
}