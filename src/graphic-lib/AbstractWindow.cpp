#include "include/AbstractWindow.h"


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

AbstractWindow::~AbstractWindow()
{}