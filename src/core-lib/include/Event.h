//
// Created by tomek on 26.04.2022.
//

#ifndef PROI_PROJEKT_EVENT_H
#define PROI_PROJEKT_EVENT_H

#include <exception>
#include <string>

struct EventConstructionException : public std::exception {
    [[nodiscard]] const char * what () const noexcept override;
};

class Event {
public:
    enum EventType {
        Keypress = 0,
        Aaaaaa,
        NullEvent
    };

    Event(EventType, unsigned int);


    struct KeypressEvent {
        unsigned int keyCode;

    };


    // ####################################################3

    // does not work with union (gcc deletes the default constructor?)
    struct {
        KeypressEvent key;
        std::string someOtherVal;
    } info;

    Event()=delete;
};


#endif //PROI_PROJEKT_EVENT_H
