//
// Created by tomek on 26.04.2022.
//

// ///////////////////////////////
//
// Implementation moved to ./include/EventQueue.h
// due to problems with template class linking
//
// ///////////////////////////////


//#include "include/Event.h"
//
//#include "include/EventQueue.h"
//class ExtendedEvent: public Event{};
//
//template <class E>
//void EventQueue<E>::registerEvent(std::unique_ptr<E> event) {
//    events_.push(std::move(event));
//}
//
//template <class E>
//std::unique_ptr<E> EventQueue<E>::pop() {
//    auto first = std::move(events_.front());
//    events_.pop();
//    return std::move(first);
//}
//
//template <class E>
//bool EventQueue<E>::isEmpty() {
//    return events_.empty();
//}
//
//template <class E>
//EventQueue<E> *EventQueue<E>::instance() {
//    static std::unique_ptr<EventQueue<E>> self(new EventQueue<E>);
//    return self.get();
//}

//template class EventQueue<Event>;
//template class EventQueue<ExtendedEvent>;

#include "./include/EventQueue.h"