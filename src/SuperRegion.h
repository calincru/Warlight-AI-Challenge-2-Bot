// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef SUPERREGION_H_INCLUDED
#define SUPERREGION_H_INCLUDED

// C++
#include <vector>

// Project
#include "main.h"
#include "Region.h"


class SuperRegion
{
public:
    SuperRegion();
    SuperRegion(int pReward);
    virtual ~SuperRegion();

    void addRegion(int region);

    // inline functions
    size_t size()
    {
        return regions.size();
    }

private:
    std::vector<int> regions;
    int reward;
};

#endif // SUPERREGION_H_INCLUDED
