// Created by Michał

#ifndef PROI_PROJEKT_STATICGRAPHIC_H
#define PROI_PROJEKT_STATICGRAPHIC_H


#include "../Window.h"
#include <unordered_map>

/// @brief Struct used for text storing
struct Text
    {
        std::string text1;
        std::string text2;
    };


/**
 * @brief Class responsible for static graphic settings
 *
 * Sets menu state, pause state and finished state arrangement
 * Constructs next branch of the comoposite
 *
 */
class StaticGraphic : public AbstractWindow
{
public:

    /**
     * @brief Construct a new Static Graphic object
     *
     * @param windowView
     * @param staticPointers
     */
    StaticGraphic(const WindowView& windowView, const Window::StaticStatePointers& staticPointers);


    /// @brief Creates approprate children
    void constructComposite();


    /// @brief Struct containing children's buttons setup
    struct ButtonsPositionMenu
    {
        sf::Vector2f button1Pos;
        sf::Vector2f button1Size;
        sf::Vector2f button2Pos;
        sf::Vector2f button2Size;
    };


    /// @brief Commands it's children to render objects on the screen
    virtual void render() override;

    /**
     * @brief Get the Children object
     *
     * @return const std::vector<std::shared_ptr<AbstractWindow>>&
     */
    const std::vector<std::shared_ptr<AbstractWindow>>& getChildren();


protected:

    /// @brief Sets WindowView for MenuStateGraphic
    void setMenuView();


    /// @brief Caluculates buttons poistions of class children
    void calculateButtonsPos();


    Window::StaticStatePointers staticPointers;

    std::shared_ptr<Window::GameStateGraphic*> gameState;

    ButtonsPositionMenu buttonsPosition;

    WindowView finishedStateView;

    // std::unordered_map<Window::GameStateGraphic, std::shared_ptr<AbstractWindow>> childrensMap{};

    WindowView menuStatesView;

};

#endif // PROI_PROJEKT_STATICGRAPHIC_H