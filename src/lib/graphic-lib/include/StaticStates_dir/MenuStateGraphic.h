// Created by Michał



#ifndef PROI_PROJEKT_MENUSTATEGRAPHIC_H
#define PROI_PROJEKT_MENUSTATEGRAPHIC_H

#include "../AbstractWindow.h"
#include "StaticGraphic.h"
#include "../Window.h"



/**
 * @brief Class responsible for rendering static states graphic
 *
 * Renders on the screen grafic of: menu game state, finished game state and pause game state
 *
 */
class MenuStateGraphic : public AbstractWindow
{
public:

    /**
     * @brief Construct a new Menu State Graphic object
     *
     * @param windowView
     * @param buttonPos
     * @param staticPointers
     */
    MenuStateGraphic(const WindowView& windowView, const StaticGraphic::ButtonsPositionMenu& buttonPos,
                     const Window::StaticStatePointers& staticPointers);


    /// @brief Renders objects on the screen
    virtual void render() override;

    /**
     * @brief Get the Static Pointers object
     *
     * @return const Window::StaticStatePointers&
     */
    const Window::StaticStatePointers& getStaticPointers();

protected:

    StaticGraphic::ButtonsPositionMenu buttonPos;

    Window::StaticStatePointers staticPointers;

    sf::Color color;

    /// @brief Load appropriate font
    void loadFont();

    sf::Font font;


    Text menuStateText
    {
        .text1 = "START GAME",
        .text2 = "      QUIT"
    };


    Text pauseStateText
    {
        .text1 = "    RESUME",
        .text2 = "GO TO MENU"
    };


    Text menuStateHeader
    {
        .text1 = "       MENU",
        .text2 = ""
    };


    Text pauseStateHeader
    {
        .text1 = "       PAUSE",
    };


    Text finishStateHeader
    {
        .text1 = "GAME FINISHED"
    };


    Text finishStateText
    {
        .text1 = "      QUIT"
    };

    /**
     * @brief Renders text on the screen
     *
     * Additional function for other render functions
     *
     * @param renderText
     * @param leftOffset
     * @param topOffset
     * @param fontSize
     * @param color
     */
    void renderText(std::string renderText, float leftOffset,
                    float topOffset, float fontSize=25,
                    sf::Color color=sf::Color::Black);

    /**
     * @brief Initiates points line in headers
     *
     * Initiates line in header of pause state and finished state
     * that depens on amount of player's points
     *
     */
    void initiatePointsStateHeader();

    /**
     * @brief Renders headers on the screen
     *
     * @param gameState
     */
    void renderHeader(Window::GameStateGraphic gameState);

    /**
     * @brief Renders buttons on the screen
     *
     * @param gameState
     */
    void renderButtons(Window::GameStateGraphic gameState);
};


#endif // PROI_PROJEKT_MENUSTATEGRAPHIC_H
