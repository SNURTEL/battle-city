//
// Created by tomek on 26.05.2022.
//

#include "include/GridBuilder.h"
#include "include/Grid.h"

std::unique_ptr<Grid> GridBuilder::BuildLevel(unsigned int level) {
    return std::make_unique<Grid>();
}