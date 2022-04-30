//
// Created by tomek on 26.04.2022.
//

#ifndef PROI_PROJEKT_EVENTQUEUE_H
#define PROI_PROJEKT_EVENTQUEUE_H

#include <queue>
#include <memory>

// /////////////////////////////
//
// Encountered problems with linking template class implementation, now it sits here
//
// /////////////////////////////


template <class E>
class EventQueue {
public:
    EventQueue(EventQueue&) = delete;

    bool operator=(const EventQueue&) = delete;

    void registerEvent(std::unique_ptr<E> event){
        events_.push(std::move(event));
    };


    std::unique_ptr<E> pop(){
        auto first = std::move(events_.front());
        events_.pop();
        return std::move(first);
    };

    bool isEmpty(){
        return events_.empty();
    };

    static EventQueue* instance(){
        static std::unique_ptr<EventQueue<E>> self(new EventQueue<E>);
        return self.get();
    };

protected:
    std::queue<std::unique_ptr<E>> events_;

    static std::unique_ptr<EventQueue> self_;

private:
    EventQueue()=default;

};


#endif //PROI_PROJEKT_EVENTQUEUE_H
