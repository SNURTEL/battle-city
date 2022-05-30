#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "FrameGraphic.h"

#ifndef PROI_PROJEKT_BAREFRAMEGRPHIC_H
#define PROI_PROJEKT_BAREFRAMEGRPHIC_H


/**
 * @brief Class responsible for bare frame render
 *
 * Renders bare frame on the screen
 *
 */
class BareFrameGraphic : AbstractWindow
{
private:

    ActiveStateGraphic::FramePointers frameObjects;

    FrameGraphic::BareFrameSize frameSize;
public:
     /**
     * @brief Renders objects on the screen
     *
     */
    virtual void render() override;


    // /**
    //  * @brief Updates its objects
    //  *
    //  */
    // void update(FrameGraphic::BareFrameSize frameSize);


    /**
     * @brief Construct a new Board View object
     *
     * @param window
     */
    BareFrameGraphic(sf::RenderWindow* window);

};


#endif // PROI_PROJEKT_BAREFRAMEGRPHIC_H