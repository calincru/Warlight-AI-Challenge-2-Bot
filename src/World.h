// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

// Project
#include "globals.h"

// C++
#include <vector>
#include <memory>


namespace warlightAi {

// Fwrd decls
class Region;
class SuperRegion;

class World
{
public:
    World() = default;
    ~World() = default;


    void addRegion(int regionId, int superRegionId);
    void addSuperRegion(int superRegionId, int award);
    void addLink(int region1, int region2);
    void addWasteland(int id);
    void updateRegion(int region, Player owner, int armies);


    RegionPtr getRegionById(int regionId) const;
    SuperRegionPtr getSuperRegionById(int superRegionId) const;
    VecOfRegionPtrs getRegionsOwnedBy(warlightAi::Player player) const;

private:
    VecOfRegionPtrs m_regions;
    VecOfSuperRegionPtrs m_superRegions;

}; // class warlightAi


} // namespace warlightAi

#endif // WORLD_H_INCLUDED
