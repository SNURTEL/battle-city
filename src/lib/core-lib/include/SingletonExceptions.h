//
// Created by tomek on 28.04.2022.
//

#ifndef PROI_PROJEKT_SINGLETONEXCEPTIONS_H
#define PROI_PROJEKT_SINGLETONEXCEPTIONS_H


#include <exception>

/**
 * An exception thrown when trying to instantiate a singleton
 * class before initializing it (if required)
 */
class SingletonNotInitializedException : public std::exception {
    [[nodiscard]] const char * what () const noexcept override;
};


#endif //PROI_PROJEKT_SINGLETONEXCEPTIONS_H
