// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef REGION_H_INCLUDED
#define REGION_H_INCLUDED

// C++
#include <vector>
#include <string>

// Project
#include "main.h"


class Region
{
public:
    Region();
    Region(int pId, int superRegion);
    virtual ~Region();

    void addNeighbor(int neighbor);
    int getNeighbor(size_t index) const;
    int getNbNeighbors() const;

    // inline functions
    void setArmies(int nbArmies)
    {
        armies = nbArmies;
    }

    void setOwner(Player pOwner)
    {
        owner = pOwner;
    }

    int getArmies() const
    {
        return armies;
    }

    inline Player getOwner() const
    {
        return owner;
    }

    inline int getSuperRegion() const
    {
        return superRegion;
    }

private:
    std::vector<int> neighbors;
    int id;
    int superRegion;
    Player owner;
    int armies;
};

#endif // REGION_H_INCLUDED
