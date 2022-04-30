//
// Created by tomek on 28.04.2022.
//

#ifndef PROI_PROJEKT_EVENTHANDLER_H
#define PROI_PROJEKT_EVENTHANDLER_H

#include <memory>

#include "Event.h"

template <class E>
class EventHandler {
public:
    virtual void handleEvent(std::unique_ptr<E>)=0;

protected:
    EventHandler()=default;
};


#endif //PROI_PROJEKT_EVENTHANDLER_H
