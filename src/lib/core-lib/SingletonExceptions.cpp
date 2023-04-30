//
// Created by tomek on 28.04.2022.
//

#include "include/SingletonExceptions.h"

const char *SingletonNotInitializedException::what() const noexcept {
    return "Singleton instance needs to be initialized before using it.";
}