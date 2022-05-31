#include "include/AbstractWindow.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


WindowView::WindowView(sf::RenderWindow* window, sf::View view)
: window(window), view(view)
{}


WindowView::WindowView()
{}


AbstractWindow::AbstractWindow(const WindowView& windowView)
: windowView(windowView)
{}


AbstractWindow::AbstractWindow()
{}


void AbstractWindow::setDefalutView()
{
    sf::View defaultView = windowView.window->getDefaultView();
    windowView.window->setView(defaultView);
}