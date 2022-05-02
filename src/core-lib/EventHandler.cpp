//
// Created by tomek on 28.04.2022.
//

#include "include/EventHandler.h"

InvalidEventException::InvalidEventException(std::string msg) {
    what_message = std::move(msg);
}

const char *InvalidEventException::what() {
    return what_message.c_str();
}

void PublisherEventHandler::handleEvent(std::unique_ptr<Event> event) {
    notifyEventSubscribers(event->type);
    processEvent(std::move(event));
}