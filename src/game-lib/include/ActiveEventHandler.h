//
// Created by tomek on 02.05.2022.
//

#ifndef PROI_PROJEKT_ACTIVEEVENTHANDLER_H
#define PROI_PROJEKT_ACTIVEEVENTHANDLER_H

#include <memory>

#include "../../core-lib/include/EventHandler.h"

class Event;

class Game;


class InvalidEventException : public std::exception{
public:
    InvalidEventException()=default;

    explicit InvalidEventException(std::string msg);  // FIXME message not showing up

    const char* what();

private:
    std::string what_message;
};

class ActiveEventHandler: public PublisherEventHandler{
public:
    ActiveEventHandler()=delete;

    explicit ActiveEventHandler(Game* game);

    void processEvent(std::unique_ptr<Event> event) override;

private:
    Game* game_;
};


#endif //PROI_PROJEKT_ACTIVEEVENTHANDLER_H
