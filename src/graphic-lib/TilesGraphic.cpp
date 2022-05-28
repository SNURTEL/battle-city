#include "include/ActiveState_dir/Borad_dir/TilesGraphic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <string>


TilesGraphic::TilesGraphic(WindowView windowView, Grid* grid)
: AbstractWindow(windowView), grid(grid)
{}


void TilesGraphic::render()
{}