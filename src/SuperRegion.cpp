// Project
#include "SuperRegion.h"
#include "utils.h"


SuperRegion::SuperRegion()
    : reward(0)
{
    UNUSED(reward);
}

SuperRegion::SuperRegion(const int& pReward)
    : reward(pReward)
{
    // nothing to do
}

SuperRegion::~SuperRegion()
{
    // nothing tod o
}

void SuperRegion::addRegion(const int& region)
{
    regions.push_back(region);
}
