//
// Created by tomek on 27.04.2022.
//

#ifndef PROI_PROJEKT_OBSERVEREXCEPTIONS_H
#define PROI_PROJEKT_OBSERVEREXCEPTIONS_H

#include <exception>
#include <string>

/**
 * Represents and exception thrown by a publisher-subscriber setup
 */
class ObserverException : public std::exception{
public:
    ObserverException()=default;

    explicit ObserverException(std::string msg);

    const char* what();

private:
    std::string what_message;
};

#endif //PROI_PROJEKT_OBSERVEREXCEPTIONS_H
