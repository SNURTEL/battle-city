//
// Created by tomek on 26.04.2022.
//

#ifndef PROI_PROJEKT_SIMPLEPUBLISHER_H
#define PROI_PROJEKT_SIMPLEPUBLISHER_H

#include <memory>
#include <vector>

class SimpleSubscriber;

/**
 * Base subscriber class. Does not distinguish different publisher aspects.
 */
class SimplePublisher {
public:
    /**
     * Removes references to self from every subscriber
     */
    ~SimplePublisher();

    /**
     * Notifies all subscribers (calls ::notify for every subscriber)
     */
    virtual void notifySubscribers();

    /**
     * Attaches a subscriber to self
     */
    void attachSubscriber(SimpleSubscriber*);

    /**
     * Detatches a subscriber from self. Throws an ObserverException if does not exist
     */
    void detachSubscriber(SimpleSubscriber*);

    bool operator==(const SimplePublisher &rhs) const;

    bool operator!=(const SimplePublisher &rhs) const;

protected:
    SimplePublisher()=default;
    std::vector<SimpleSubscriber*> subscribers_;
};

#endif //PROI_PROJEKT_SIMPLEPUBLISHER_H
