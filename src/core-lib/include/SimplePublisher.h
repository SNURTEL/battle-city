//
// Created by tomek on 26.04.2022.
//

#ifndef PROI_PROJEKT_SIMPLEPUBLISHER_H
#define PROI_PROJEKT_SIMPLEPUBLISHER_H

#include <memory>
#include <vector>

class SimpleSubscriber;

class SimplePublisher {
public:
    ~SimplePublisher();

    virtual void notifySubscribers();

    void attachSubscriber(SimpleSubscriber*);

    void detachSubscriber(SimpleSubscriber*);

    bool operator==(const SimplePublisher &rhs) const;

    bool operator!=(const SimplePublisher &rhs) const;

protected:
    SimplePublisher()=default;
    std::vector<SimpleSubscriber*> subscribers_;
};

#endif //PROI_PROJEKT_SIMPLEPUBLISHER_H
