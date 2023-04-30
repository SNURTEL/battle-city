// Created by Michał


#ifndef PROI_PROJKET_GAMEINFOGRAPHIC_H
#define PROI_PROJKET_GAMEINFOGRAPHIC_H

#include "../../AbstractWindow.h"
#include "FrameGraphic.h"


/**
 * @brief Class responsible for writing game state info on the frame
 *
 * Draws on the frame "Player's lives" and "Level" writings
 */
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