// Created by Michał



#ifndef PROI_PROJEKT_FRAMEGRAPHIC_H
#define PROI_PROJEKT_FRAMEGRAPHIC_H

#include "../ActiveStateGraphic.h"


/**
 * @brief Class responsible for frame upadates and render
 *
 * Updates its children attributes
 * Commands its children to render the frame around the board and all the objects connected to it.
 * Sets the right SFML views for them
 */
class FrameGraphic : public AbstractWindow
{
public:

    /// @brief Struct containing params which dictates how to draw the bare frame
    struct BareFrameSize
    {
        sf::FloatRect upRect;
        sf::FloatRect leftRect;
        sf::FloatRect rightRect;
        sf::FloatRect downRect;
    };

    /// @brief Struct contains pointers for GameInfoGraphic class
    struct GameInfo
    {
        std::shared_ptr<int> playerLives;
        std::shared_ptr<int> level;
    };


    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render() override;


    const ActiveStateGraphic::FramePointers& getPointers() const;


    /**
     * @brief Construct a new Board View object
     *
     * Constructs next composite branches
     */
    FrameGraphic(const WindowView& windowView, const ActiveStateGraphic::FramePointers& frameObjects);

private:

    ActiveStateGraphic::FramePointers frameObjects;


    BareFrameSize bareFrameSize;


    /// @brief Creates appropriate children
    void conscructComposite();


    /// @brief Calculates size of a frame around the game board
    void calculateBareFrameSize();


    /// @brief Calculates where draw the game info
    void calculateInfoTextPoistion();


    /// @brief Initiates game info shared pointers
    void initiatesGameInfoPointers();


    /// @brief Makes game pointers struct
    void makeGamePointers();


    GameInfo gameInfoPointers;

    /// @brief Stores WindowView object for the GameInfoGraphic class
    WindowView infoWindowView;

};


#endif // FRAMEGRPHIC_H