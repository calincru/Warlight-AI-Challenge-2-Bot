// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef REGION_H_INCLUDED
#define REGION_H_INCLUDED

// Project
#include "globals.hpp"

// C++
#include <vector>
#include <memory>

namespace warlightAi {

// Fwrd decls
class SuperRegion;

/**
 * Class to represent a single region on the map of the game.
 */
class Region
{
    using RegionWkPtr = std::weak_ptr<Region>;
    using SuperRegionWkPtr = std::weak_ptr<SuperRegion>;

public:
    /**
     * The sole constructor which creates a new region given its id, a reference
     * to its super region, the initial number of armies and its owner.
     */
    Region(int id, SuperRegionPtr superRegion);

    /**
     * Returns the id of this region. It is an unique integer based on the input
     * received from the engine.
     */
    int id() const;

    /**
     * Adds the given region reference as this region's neighbor.
     */
    void addNeighbor(RegionPtr neighbor);

    /**
     * Tests if the given region reference is one of the neighbors of this
     * region.
     */
    bool isNeighbor(RegionPtr region) const;

    /**
     * Returns a list of references to this region's neighbors.
     */
    std::vector<RegionPtr> getNeighbors() const;

    /**
     * Sets the owner of this region.
     */
    void setOwner(warlightAi::Player player);

    /**
     * Checks if this region is owned by the player given as argument.
     */
    bool isOwnedBy(warlightAi::Player player) const;

    /**
     * Returns the owner of this region.
     */
    warlightAi::Player getOwner() const;

    /**
     * Sets the number of armies on this region.
     */
    void setArmies(int armies);

    /**
     * Returns the current number of armies on this region.
     */
    int getArmies() const;

    /**
     * Returns a reference to the super region of this region.
     */
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
