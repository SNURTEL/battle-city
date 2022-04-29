//
// Created by tomek on 28.04.2022.
//

#ifndef PROI_PROJEKT_EVENTSUBSCRIBER_H
#define PROI_PROJEKT_EVENTSUBSCRIBER_H

#include <map>
#include <vector>
#include <list>

#include "Event.h"
#include "Event.h"

class EventPublisher;

class EventSubscriber {
public:
    ~EventSubscriber();

    void subscribe(EventPublisher*, Event::EventType);

    void unsubscribe(EventPublisher*, Event::EventType);

    void removeDeletedSubject(EventPublisher *pub, Event::EventType);

    virtual void notify(EventPublisher *, Event::EventType eventType) =0;

    bool operator==(const EventSubscriber &rhs) const;

    bool operator!=(const EventSubscriber &rhs) const;

protected:
    EventSubscriber()=default;
    std::map<Event::EventType, std::list<EventPublisher*>> subscribedSubjects_;

private:
    void ensureEntryExists(Event::EventType);
};


#endif //PROI_PROJEKT_EVENTSUBSCRIBER_H
