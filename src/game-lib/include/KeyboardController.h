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

//FIXME Delayed response, repeat character delay, unexpected KeyReleased events

/**
 * Translates SFML's window events (or, more precisely, KeyPressed, KeyReleased and NullEvent events)
 * to game's internal events.
 * Derived from SimpleSubscriber -> can (and should) be connected to Clock to trigger key event pooling
 */
class KeyboardController: public SimpleSubscriber{
public:
    KeyboardController() = delete;

    /**
     * Inits class KeyboardController
     * @param window sf::RenderWindow to pool events from
     */
    explicit KeyboardController(sf::RenderWindow* window);

    /**
     * Fetches a keyboard event to global event pool
     * @param pub Publisher which triggered the response
     */
    void notify(SimplePublisher *pub) override;

    /**
     * Pools an event from SFML's window, translates it to game's internal Event equivalent,
     * and appends it to global event queue
     * Can fetch either:
     * - KeyPressed (from sf::Event::KeyPressed)
     * - KeyReleased (from sf::Event::KeyReleased)
     * - NullEvent (if none of the above was registered)
     */
    void fetchKeyboardEvent();

    sf::RenderWindow* window_;

    EventQueue<Event>* eventQueue_;
};


#endif //PROI_PROJEKT_KEYBOARDCONTROLLER_H
