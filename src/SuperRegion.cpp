// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// Project
#include "SuperRegion.h"
#include "utils.h"


SuperRegion::SuperRegion()
    : reward(0)
{
    UNUSED(reward);
}

SuperRegion::SuperRegion(int pReward)
    : reward(pReward)
{
    // nothing to do
}

SuperRegion::~SuperRegion()
{
    // nothing to do
}

void SuperRegion::addRegion(int region)
{
    regions.push_back(region);
}
