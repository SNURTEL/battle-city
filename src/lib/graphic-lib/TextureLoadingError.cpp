#include "include/TextureLoadingError.h"

TextureLoadingError::TextureLoadingError(std::string msg)
: std::exception()
{
    whatMessage = msg;
}

const char* TextureLoadingError::what() const throw()
{
    return whatMessage.c_str();
}