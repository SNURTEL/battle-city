//
// Created by tomek on 28.04.2022.
//

#include "include/EventPublisher.h"
#include "include/ObserverExceptions.h"

EventPublisher::~EventPublisher() {
    for(auto [eventType, subscribers]: event_subscribers_){
        for(EventSubscriber* sub: subscribers){
            try{
                sub->removeDeletedSubject(this, eventType);
            }catch(ObserverException& e) {}
        }
    }
}

void EventPublisher::notifyEventSubscribers(Event::EventType eventType) {
    for(EventSubscriber* sub: event_subscribers_[eventType]){
        sub->notify(this, eventType);
    }
}

void EventPublisher::attachEventSubscriber(EventSubscriber *sub, Event::EventType eventType) {
    std::list<EventSubscriber*>* subs = &event_subscribers_[eventType];
    if(*(std::find(subs->begin(), subs->end(), sub)) != *(subs->end())){
        return;
    }

    subs->push_back(sub);
}

void EventPublisher::detachEventSubscriber(EventSubscriber *sub, Event::EventType eventType) {
    std::list<EventSubscriber*>* subs = &event_subscribers_[eventType];
    auto iter = std::find(subs->begin(), subs->end(), sub);
    if(iter == subs->end()){
        throw ObserverException("Trying to detach a non-existent subscriber");
    }

    subs->erase(iter);
}


void EventPublisher::ensureEntryExists(Event::EventType eventType) {
    if (!event_subscribers_.contains(eventType)) {
        event_subscribers_.insert(std::make_pair(eventType, std::list<EventSubscriber *>{}));
    }
}

bool EventPublisher::operator==(const EventPublisher &rhs) const {
    return event_subscribers_ == rhs.event_subscribers_;
}

bool EventPublisher::operator!=(const EventPublisher &rhs) const {
    return !(rhs == *this);
}
