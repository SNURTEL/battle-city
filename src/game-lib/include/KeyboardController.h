//
// Created by tomek on 02.05.2022.
//

#ifndef PROI_PROJEKT_KEYBOARDCONTROLLER_H
#define PROI_PROJEKT_KEYBOARDCONTROLLER_H

#include <memory>
#include <SFML/Graphics.hpp>

#include "../../core-lib/include/SimpleSubscriber.h"

class Event;

template <class E>
class EventQueue;

class KeyboardController: public SimpleSubscriber{
public:
    KeyboardController() = delete;

    explicit KeyboardController(sf::RenderWindow* window);

    void notify(SimplePublisher *pub) override;

    void fetchKeyboardEvent();

    sf::RenderWindow* window_;

    EventQueue<Event>* eventQueue_;
};


#endif //PROI_PROJEKT_KEYBOARDCONTROLLER_H
