// Created by Michał


#ifndef PROI_PROJEKT_BAREFRAMEGRPHIC_H
#define PROI_PROJEKT_BAREFRAMEGRPHIC_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "FrameGraphic.h"


/**
 * @brief Class responsible for bare frame render
 *
 * Renders bare frame on the screen
 *
 */
class BareFrameGraphic : public AbstractWindow
{
private:

    sf::Color color;

    FrameGraphic::BareFrameSize frameSize;
public:
     /**
     * @brief Renders objects on the screen
     *
     */
    virtual void render() override;


    /**
     * @brief Construct a new Board View object
     *
     * @param window
     */
    BareFrameGraphic(const WindowView& windowView, const FrameGraphic::BareFrameSize& bareFrameSize);

};


#endif // PROI_PROJEKT_BAREFRAMEGRPHIC_H