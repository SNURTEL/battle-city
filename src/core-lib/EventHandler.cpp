//
// Created by tomek on 28.04.2022.
//

#include "include/EventHandler.h"

void PublisherEventHandler::handleEvent(std::unique_ptr<Event> event) {
    notifyEventSubscribers(event->type);
    processEvent(std::move(event));
}