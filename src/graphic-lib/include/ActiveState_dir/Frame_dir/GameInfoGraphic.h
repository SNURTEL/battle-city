#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../../AbstractWindow.h"
#include "FrameGraphic.h"


#ifndef PROI_PROJKET_GAMEINFOGRAPHIC_H
#define PROI_PROJKET_GAMEINFOGRAPHIC_H

class GameInfoGraphic : public AbstractWindow
{
public:

    /**
     * @brief Renders objects on the screen
     *
     */
    virtual void render() override;

    /// @brief Initates GameInfoGraphic object
    GameInfoGraphic(const WindowView& windowView, const FrameGraphic::GameInfo& gameInfo);

private:
    FrameGraphic::GameInfo gameInfo;

    /// @brief Loads font to the font object
    void loadFont();

    /// @brief Renders given text on the screen
    void renderText(std::string rednerText, float leftOffset, float topOffset);

    sf::Font font;
};

#endif // PROI_PROJKET_GAMEINFOGRAPHIC_H