//
// Created by tomek on 28.04.2022.
//

#include "include/EventHandler.h"

InvalidEventException::InvalidEventException(std::string msg) : std::exception() {
    what_message = std::move(msg);
}

const char *InvalidEventException::what() const noexcept {
    return what_message.c_str();
}

void PublisherEventHandler::handleEvent(std::unique_ptr<Event> event) {
    notifyEventSubscribers(event->type);
    return processEvent(std::move(event));
}