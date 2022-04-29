//
// Created by tomek on 28.04.2022.
//

#include "include/EventPublisher.h"
#include "include/ObserverExceptions.h"

// TODO T E S T S

EventSubscriber::~EventSubscriber() {
    for (auto[eventType, publishers]: subscribedSubjects_) {
        for (EventPublisher *pub: publishers) {
            try {
                unsubscribe(pub, eventType);
            } catch (ObserverException &e) {}
        }
    }
}

void EventSubscriber::subscribe(EventPublisher *pub, Event::EventType eventType) {
    if (!subscribedSubjects_.contains(eventType)) {
        subscribedSubjects_.insert(std::make_pair(eventType, std::list<EventPublisher *>{pub}));
    } else {
        auto iter = std::find(subscribedSubjects_[eventType].begin(), subscribedSubjects_[eventType].end(), pub);

        if (iter != subscribedSubjects_[eventType].end()) {
            return;
        }

        subscribedSubjects_[eventType].push_back(pub);

    }
    pub->attachEventSubscriber(this, eventType);
}

void EventSubscriber::unsubscribe(EventPublisher *pub, Event::EventType eventType) {
    pub->detachEventSubscriber(this, eventType);

    ensureEntryExists(eventType);

    auto iter = std::find(subscribedSubjects_[eventType].begin(), subscribedSubjects_[eventType].end(), pub);

    if (iter == subscribedSubjects_[eventType].end()) {
        throw ObserverException("Unsubscribed from publisher which is not in subscriber's pool");
    }

    subscribedSubjects_[eventType].erase(iter);
}

void EventSubscriber::removeDeletedSubject(EventPublisher *pub, Event::EventType eventType) {
    ensureEntryExists(eventType);

    std::list<EventPublisher *> *pubs = &(subscribedSubjects_[eventType]);
    auto iter = std::find(pubs->begin(), pubs->end(), pub);

    if (iter == pubs->end()) {
        return;
    }

    pubs->erase(iter);
}

void EventSubscriber::ensureEntryExists(Event::EventType eventType) {
    if (!subscribedSubjects_.contains(eventType)) {
        subscribedSubjects_.insert(std::make_pair(eventType, std::list<EventPublisher *>{}));
    }
}

bool EventSubscriber::operator==(const EventSubscriber &rhs) const {
    return subscribedSubjects_ == rhs.subscribedSubjects_;
}

bool EventSubscriber::operator!=(const EventSubscriber &rhs) const {
    return !(rhs == *this);
}
