//
// Created by tomek on 28.04.2022.
//

#ifndef PROI_PROJEKT_EVENTPUBLISHER_H
#define PROI_PROJEKT_EVENTPUBLISHER_H

#include <map>
#include <vector>

#include "Event.h"
#include "EventSubscriber.h"

/**
 * Publisher class with multiple subscriber pools
 *
 * Subscribers can subscribe a particular event, and subscriber notification method calls can be done for each
 * event separately.
 */
class EventPublisher {
public:
    /**
     * Removes references to self from every subscriber
     */
    ~EventPublisher();

    /**
     * Notifies all subscribers subscribed to an event of type eventType (calls ::notify for every subscriber)
     * @param eventType Indicates which subscribers should be notified
     */
    virtual void notifyEventSubscribers(Event::EventType eventType);

    /**
     * Attaches a subscriber to eventType aspect of self
     * @param sub
     * @param eventType
     */
    void attachEventSubscriber(EventSubscriber *sub, Event::EventType eventType);

    /**
     * Detaches a subscriber from eventType aspect of self. Throws an ObserverException if does not exist
     * @param sub
     * @param eventType
     */
    void detachEventSubscriber(EventSubscriber *sub, Event::EventType eventType);

    bool operator==(const EventPublisher &rhs) const;

    bool operator!=(const EventPublisher &rhs) const;


protected:
    EventPublisher()=default;
    std::map<Event::EventType, std::list<EventSubscriber*>> event_subscribers_;

private:
    /**
     * Creates an entry in event_subscribers if it does not exist yet
     *
     * @param eventType
     */
    void ensureEntryExists(Event::EventType eventType);
};

#endif //PROI_PROJEKT_EVENTPUBLISHER_H
