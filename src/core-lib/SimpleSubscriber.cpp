//
// Created by tomek on 26.04.2022.
//

#include "include/SimpleSubscriber.h"
#include "include/SimplePublisher.h"
#include "include/ObserverExceptions.h"

SimpleSubscriber::~SimpleSubscriber() {
    for (SimplePublisher *pub: subscribedSubjects_) {
        try{
            unsubscribe(pub);
        }catch(ObserverException& e){}

    };
}

void SimpleSubscriber::subscribe(SimplePublisher *pub) {
    auto iter = std::find(subscribedSubjects_.begin(), subscribedSubjects_.end(), pub);

    if(iter != subscribedSubjects_.end()){
        return;
    }

    pub->attachSubscriber(this);

    subscribedSubjects_.push_back(pub);
}

void SimpleSubscriber::unsubscribe(SimplePublisher *pub) {
    pub->detachSubscriber(this);

    auto iter = std::find(subscribedSubjects_.begin(), subscribedSubjects_.end(), pub);

    if(iter == subscribedSubjects_.end()){
        throw ObserverException("Unsubscribed from publisher which is not in subscriber's pool");
    }

    subscribedSubjects_.erase(iter);
}

void SimpleSubscriber::removeDeletedSubject(SimplePublisher *pub) {
    auto iter = std::find(subscribedSubjects_.begin(), subscribedSubjects_.end(), pub);

    if (iter == subscribedSubjects_.end()) {
        return;
    }

    subscribedSubjects_.erase(iter);
}
