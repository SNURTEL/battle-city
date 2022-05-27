#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../ActiveStateGraphic.h"

#ifndef PROI_PROJEKT_FRAMEGRAPHIC_H
#define PROI_PROJEKT_FRAMEGRAPHIC_H


/**
 * @brief Class responsible for frame upadates and render
 *
 * Updates its children attributes
 * Commands its children to render the frame around the board and all the objects connected to it.
 * Sets the right SFML views for them
 */
class FrameGraphic : AbstractWindow
{
public:

    struct BareFrameSize
    {
        sf::FloatRect upRect;
        sf::FloatRect leftRect;
        sf::FloatRect rightRect;
        sf::FloatRect downRect;
    };


    /**
     * @brief Commands its children to render objects on the screen
     *
     */
    virtual void render() const override;


    /**
     * @brief Commands its children to update objects to render
     *
     */
    void update(ActiveStateGraphic::FramePointers frameObjects);


    /**
     * @brief Construct a new Board View object
     *
     * @param window
     */
    FrameGraphic(sf::RenderWindow* window);

private:

    ActiveStateGraphic::FramePointers frameObjects;

    BareFrameSize bareFrameSize;

    void calculatebareFrameSize();

};


#endif // FRAMEGRPHIC_H