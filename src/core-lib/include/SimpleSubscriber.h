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
    ~SimpleSubscriber();

    void subscribe(SimplePublisher*);

    void unsubscribe(SimplePublisher*);

    void removeDeletedSubject(SimplePublisher*);

    virtual void notify(SimplePublisher*)=0;

protected:
    SimpleSubscriber()=default;
    std::vector<SimplePublisher*> subscribedSubjects_;
};


#endif //PROI_PROJEKT_SIMPLESUBSCRIBER_H
