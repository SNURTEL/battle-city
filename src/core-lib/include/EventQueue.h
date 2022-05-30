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

/**
 * \brief Singleton class wrapper for std::queue<E>.
 *
 * Stores and queues events wrapped in std::unique_ptr.
 * Singleton instance can be accessed with ::instance(). Different instances are created for every each type (E).
 * Cannot be inherited from (::instance() would always return the base class)
 *
 * NOTE: When testing event-generating objects, call EventQueue::instance()->clear() or assert EventQueue::instance()->isEmpty() to guarantee starting and ending the test with an empty queue
 *
 * @tparam E Event type
 */
template <class E>
class EventQueue {
public:
    EventQueue(EventQueue&) = delete;

    bool operator=(const EventQueue&) = delete;

    /**
     * Moves the unique_ptr containing the event to the back of the queue.
     * @param event Event instance wrapped in std::unique_ptr
     */
    void registerEvent(std::unique_ptr<E> event){
        events_.push(std::move(event));
    };

    /**
     * Pops the first element in the queue.
     * Popping from an empty queue will result in undefined behavior
     * @return Event instance wrapped in a std::unique_ptr
     */
    std::unique_ptr<E> pop(){
        auto first = std::move(events_.front());
        events_.pop();
        return std::move(first);
    };

    /**
     * Returns true it the queue is empty
     *
     * @return Whether the queue is empty
     */
    bool isEmpty(){
        return events_.empty();
    };

    unsigned int size(){
        return events_.size();
    }

    /**
     * Removes all elements from the queue
     */
    void clear(){
        std::queue<std::unique_ptr<E>>().swap(events_);
    }

    /**
     * Provides access to singleton instance
     *
     * @return A pointer to singleton instance
     */
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
