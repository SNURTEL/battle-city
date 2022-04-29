//
// Created by tomek on 28.04.2022.
//

#ifndef PROI_PROJEKT_EVENTPUBLISHER_H
#define PROI_PROJEKT_EVENTPUBLISHER_H

#include <map>
#include <vector>

#include "Event.h"
#include "EventSubscriber.h"

class EventPublisher {
public:
    ~EventPublisher();

    virtual void notifyEventSubscribers(Event::EventType eventType);

    void attachEventSubscriber(EventSubscriber *sub, Event::EventType eventType);

    void detachEventSubscriber(EventSubscriber *sub, Event::EventType eventType);

    void ensureEntryExists(Event::EventType);

    bool operator==(const EventPublisher &rhs) const;

    bool operator!=(const EventPublisher &rhs) const;


protected:
    EventPublisher()=default;
    std::map<Event::EventType, std::list<EventSubscriber*>> event_subscribers_;
};

#endif //PROI_PROJEKT_EVENTPUBLISHER_H
