#include "include/ActiveState_dir/Borad_dir/BulletsGraphic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <string>


BulletsGraphic::BulletsGraphic(const WindowView& windowView, std::vector<Bullet*>* bullets)
: AbstractWindow(windowView), bullets(bullets)
{}


void BulletsGraphic::render()
{}