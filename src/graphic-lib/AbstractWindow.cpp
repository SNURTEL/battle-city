#include "include/AbstractWindow.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


WindowView::WindowView(sf::RenderWindow* window, float topOffset, float leftOffset)
: window(window), topOffset(topOffset), leftOffset(leftOffset)
{}


WindowView::WindowView()
{}


AbstractWindow::AbstractWindow(const WindowView& windowView)
: windowView(windowView)
{}


AbstractWindow::AbstractWindow()
{}
