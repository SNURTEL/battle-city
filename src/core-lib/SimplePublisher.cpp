//
// Created by tomek on 26.04.2022.
//

#include "include/SimplePublisher.h"
#include "include/SimpleSubscriber.h"
#include "include/ObserverExceptions.h"

SimplePublisher::~SimplePublisher() {
    for(SimpleSubscriber* sub: subscribers_){
        try{
            sub->removeDeletedSubject(this);
        }catch(std::exception& e) {}
    }
}

void SimplePublisher::notifySubscribers() {
    for(SimpleSubscriber* sub: subscribers_){
        sub->notify(this);
    }
}

void SimplePublisher::attachSubscriber(SimpleSubscriber* sub) {
    if(std::find(subscribers_.begin(), subscribers_.end(), sub) != subscribers_.end()){
        return;
    }

    subscribers_.push_back(sub);
}

void SimplePublisher::detachSubscriber(SimpleSubscriber* sub) {
    auto iter = std::find(subscribers_.begin(), subscribers_.end(), sub);
    if(iter == subscribers_.end()){
        throw ObserverException("Trying to detach a non-existent subscriber");
    }

    subscribers_.erase(iter);
}

bool SimplePublisher::operator==(const SimplePublisher &rhs) const {
    return subscribers_ == rhs.subscribers_;
}

bool SimplePublisher::operator!=(const SimplePublisher &rhs) const {
    return !(rhs == *this);
}
