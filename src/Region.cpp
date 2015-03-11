// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// C++
#include <stdio.h>
#include <iostream>

// Project
#include "Region.h"
#include "SuperRegion.h"
#include "utils.h"


Region::Region()
    : id(0)
    , superRegion(0)
    , owner(NEUTRAL)
    , armies(0)
{
    UNUSED(id);
}


Region::Region(int pId, int pSuperRegion)
    : id(pId)
    , superRegion(pSuperRegion)
    , owner(NEUTRAL)
    , armies(0)
{
}

Region::~Region()
{
}

void Region::addNeighbor(int neighbor)
{
    neighbors.push_back(neighbor);
}

int Region::getNbNeighbors() const
{
    return neighbors.size();
}

int Region::getNeighbor(size_t index) const
{
    return neighbors.at(index);
}
