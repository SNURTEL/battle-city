#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../ActiveStateGraphic.h"
// #include "BareFrameGraphic.h"

#ifndef PROI_PROJEKT_FRAMEGRAPHIC_H
#define PROI_PROJEKT_FRAMEGRAPHIC_H

// class BareFrameGrafic;


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

    struct BareFrameSize
    {
        sf::FloatRect upRect;
        sf::FloatRect leftRect;
        sf::FloatRect rightRect;
        sf::FloatRect downRect;
    };

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


    // /**
    //  * @brief Commands its children to update objects to render
    //  *
    //  */
    // void update(ActiveStateGraphic::FramePointers frameObjects);


    const ActiveStateGraphic::FramePointers& getPointers() const;

    /**
     * @brief Construct a new Board View object
     *
     * @param window
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

    std::shared_ptr<Tank*> playerTank;

    /// @brief Initiates game info shared pointers
    void initiatesGameInfoPointers();

    /// @brief Makes game pointers struct
    void makeGamePointers();

    GameInfo gameInfoPointers;

    WindowView infoWindowView;

};


#endif // FRAMEGRPHIC_H