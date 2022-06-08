// Created by Michał

#ifndef PROI_PROJEKT_TEXTURELOADINGERROR_H
#define PROI_PROJEKT_TEXTURELOADINGERROR_H

#include <exception>
#include <string>

/**
 * An Exceptions thrown during not succesfull loading of texutres
 */
class TextureLoadingError : public std::exception{
public:

    /**
     * @brief Construct a new Texture Loading Error object
     *
     * @param msg
     */
    TextureLoadingError(std::string msg="Failed to load textures");

    const char* what() const throw();

private:
    std::string whatMessage;
};

#endif //PROI_PROJEKT_TEXTURELOADINGERROR_H
