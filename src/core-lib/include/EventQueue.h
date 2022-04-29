//
// Created by tomek on 26.04.2022.
//

#ifndef PROI_PROJEKT_EVENTQUEUE_H
#define PROI_PROJEKT_EVENTQUEUE_H

#include <queue>
#include <memory>

template <class E>
class EventQueue {
public:
    EventQueue()=default;

    EventQueue(EventQueue&) = delete;

    bool operator=(const EventQueue&) = delete;

    void registerEvent(std::unique_ptr<E>);

    std::unique_ptr<E> pop();

    bool isEmpty();

    static EventQueue* instance();

protected:
    std::queue<E> events_;

    static std::unique_ptr<EventQueue> self_;
};


#endif //PROI_PROJEKT_EVENTQUEUE_H
