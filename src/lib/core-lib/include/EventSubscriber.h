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

/**
 * Subscriber class which can subscribe to multiple aspects of the same object
 *
 */
class EventSubscriber {
public:
    /**
     * Unsubscribes all publishers before de-initializing
     */
    ~EventSubscriber();

    /**
     * Subscribes to events of a particular type from the given publisher
     * @param pub
     * @param eventType
     */
    void subscribe(EventPublisher* pub, Event::EventType eventType);

    /**
     * Unsubscribes from events of a particular type from the given publisher.
     * Throws and ObserverException if it does not exist
     * @param pub
     * @param eventType
     */
    void unsubscribe(EventPublisher*, Event::EventType);

    /**
     * Tries to unsubscribe from a publisher, quietly returns if it does not exist
     * @param pub
     * @param eventType
     */
    void removeDeletedSubject(EventPublisher *pub, Event::EventType eventType);

    /**
     * Called when notifying the subscriber about an event
     * @param pub Publisher which sent the notification
     * @param eventType Event type
     */
    virtual void notify(EventPublisher * pub, Event::EventType eventType) =0;

    bool operator==(const EventSubscriber &rhs) const;

    bool operator!=(const EventSubscriber &rhs) const;

protected:
    EventSubscriber()=default;
    std::map<Event::EventType, std::list<EventPublisher*>> subscribedSubjects_;

private:
    /**
     * Creates an entry in event_subscribers if it does not exist yet
     *
     * @param eventType
     */
    void ensureEntryExists(Event::EventType eventType);
};


#endif //PROI_PROJEKT_EVENTSUBSCRIBER_H
