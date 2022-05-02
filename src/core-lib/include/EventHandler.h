//
// Created by tomek on 28.04.2022.
//

#ifndef PROI_PROJEKT_EVENTHANDLER_H
#define PROI_PROJEKT_EVENTHANDLER_H

#include <memory>

#include "Event.h"
#include "EventPublisher.h"

class InvalidEventException : public std::exception{
public:
    InvalidEventException()=default;

    explicit InvalidEventException(std::string msg);  // FIXME message not showing up

    const char* what();

private:
    std::string what_message;
};

/**
 * Base class for event handlers.
 *
 * @tparam E Event type
 */
template <class E>
class EventHandler {
public:
    /**
     * Handles an event. Most of the time method implementation will contain a switch-case which responds to different
     * events differentiated by some kind of enum, and throws an exception if an event cannot be handled.
     * @param event The event that should be handed
     */
    virtual void handleEvent(std::unique_ptr<E> event)=0;

protected:
    EventHandler()=default;
};

/**
 * \brief Extended EventHandler for Event class events with publisher capabilities.
 *
 * Notifies event subscribers at every ::handleEvent call.
 */
class PublisherEventHandler: public EventPublisher, public EventHandler<Event> {
public:
    /**
     * Template method used for handling events. Calls EventPublisher::notifyEventSubscribers at every call,
     * and then proceeds with ::processEvent.
     * @param event The event that should be handed
     */
    void handleEvent(std::unique_ptr<Event> event) override;

    /**
     * Called by ::handleEvent, does the actual processing. This is the method that should be overloaded in
     * derived classes instead of ::handleEvent
     * @param event The event that should be handed
     */
    virtual void processEvent(std::unique_ptr<Event> event)=0;

protected:
    PublisherEventHandler()=default;
};

#endif //PROI_PROJEKT_EVENTHANDLER_H
