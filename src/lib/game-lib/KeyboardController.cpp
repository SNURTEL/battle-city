//
// Created by tomek on 02.05.2022.
//

#include <SFML/Window.hpp>

#include "include/KeyboardController.h"
#include "../core-lib/include/Event.h"
#include "../core-lib/include/EventQueue.h"

KeyboardController::KeyboardController(sf::RenderWindow *window) {
    window_ = window;
    eventQueue_ = EventQueue<Event>::instance();
}

void KeyboardController::fetchKeyboardEvent() const {
    sf::Event event;
    window_->pollEvent(event);

    switch (event.type) {
        case sf::Event::KeyReleased: {
            eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::KeyReleased,
                                                               event.key.code));
            break;
        }
        case sf::Event::KeyPressed: {
            eventQueue_->registerEvent(std::make_unique<Event>(Event::EventType::KeyPressed,
                                                               event.key.code));
            break;
        }
        default: {
            eventQueue_->registerEvent(std::make_unique<Event>(Event::NullEvent));
            break;
        }
    }
}

void KeyboardController::notify(SimplePublisher *pub) {
    fetchKeyboardEvent();
}