#include "include/AbstractWindow.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


WindowView::WindowView(sf::RenderWindow* window, float topOfset, float leftOfset)
: window(window), topOfset(topOfset), leftOfset(leftOfset)
{}


WindowView::WindowView()
{}


AbstractWindow::AbstractWindow(const WindowView& windowView)
: windowView(windowView)
{}


AbstractWindow::AbstractWindow()
{}
