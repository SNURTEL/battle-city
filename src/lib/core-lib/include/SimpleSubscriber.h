//
// Created by tomek on 26.04.2022.
//

#ifndef PROI_PROJEKT_SIMPLESUBSCRIBER_H
#define PROI_PROJEKT_SIMPLESUBSCRIBER_H

#include <vector>
#include <memory>

class SimplePublisher;

class SimpleSubscriber {
public:
    /**
     * Unsubscribes from all publishers before de-initializing
     */
    ~SimpleSubscriber();

    /**
     * Subscribes to a publisher
     * @param pub
     */
    void subscribe(SimplePublisher* pub);

    /**
     * Unsubscribes from a publisher. Throws an ObserverException if does not exist.
     * @param pub
     */
    void unsubscribe(SimplePublisher* pub);

    /**
     * Tries to remove a publisher from an internal publisher pool, quietly returns if does not exist
     * @param pub
     */
    void removeDeletedSubject(SimplePublisher* pub);

    /**
     * Called when a notification is sent
     * @param pub
     */
    virtual void notify(SimplePublisher* pub)=0;

protected:
    SimpleSubscriber()=default;
    std::vector<SimplePublisher*> subscribedSubjects_;
};


#endif //PROI_PROJEKT_SIMPLESUBSCRIBER_H
