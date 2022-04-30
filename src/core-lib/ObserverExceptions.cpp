//
// Created by tomek on 27.04.2022.
//

#include "include/ObserverExceptions.h"

#include <utility>

ObserverException::ObserverException(std::string msg) {
    what_message = std::move(msg);
}

const char *ObserverException::what() {
    return what_message.c_str();
}