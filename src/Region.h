// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef REGION_H_INCLUDED
#define REGION_H_INCLUDED

// Project
#include <globals.h>

// C++
#include <vector>
#include <memory>


namespace warlightAi {

// Fwrd decls
class SuperRegion;

class Region
{
    using RegionWkPtr = std::weak_ptr<Region>;
    using SuperRegionWkPtr = std::weak_ptr<SuperRegion>;

public:
    Region(int id, SuperRegionPtr superRegion, int armies,
           warlightAi::Player player);

    int id() const;

    void addNeighbor(RegionPtr neighbor);
    bool isNeighbor(RegionPtr region) const;
    std::vector<RegionPtr> getNeighbors() const;

    void setOwner(warlightAi::Player player);
    bool isOwnedBy(warlightAi::Player player) const;
    warlightAi::Player getOwner() const;

    void setArmies(int armies);
    int getArmies() const;

    SuperRegionPtr getSuperRegion() const;

private:
    int m_id;
    const SuperRegionWkPtr m_superRegion;
    std::vector<RegionWkPtr> m_neighbors;
    int m_armies;
    warlightAi::Player m_owner;

}; // class Region

} // namespace warlightAi

#endif // REGION_H_INCLUDED
