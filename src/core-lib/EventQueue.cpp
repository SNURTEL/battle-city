//
// Created by tomek on 26.04.2022.
//

#include "include/EventQueue.h"
#include "include/Event.h"

template <class E>
void EventQueue<E>::registerEvent(std::unique_ptr<E> event) {
    events_.push(std::move(event));
}

template <class E>
std::unique_ptr<E> EventQueue<E>::pop() {
    return std::move(events_.pop());
}

template <class E>
bool EventQueue<E>::isEmpty() {
    return events_.empty();
}

template <class E>
EventQueue<E> *EventQueue<E>::instance() {
    static std::unique_ptr<EventQueue<E>> self(new EventQueue<E>);
    return self.get();
}