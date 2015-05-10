// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

// Project
#include "globals.hpp"

// C++
#include <vector>
#include <memory>


namespace warlightAi {

// Fwrd decls
class Region;
class SuperRegion;

/**
 * This class represents the map of one Warlight 2 game.
 */
class World
{
public:
    /**
     * Default constructor and destructor.
     */
    World() = default;
    ~World() = default;

    /**
     * Creates and adds a new region to the map of the game, given the id of
     * the region and the id of its super region.
     */
    void addRegion(int regionId, int superRegionId);

    /**
     * Creates and adds a new super region to the map of the game, given its
     * id and reward.
     */
    void addSuperRegion(int superRegionId, int award);

    /**
     * Adds a new link to the map of the game, given two regions by their id's.
     */
    void addLink(int region1, int region2);

    /**
     * Adds a new wasteland to the map of the game.
     */
    void addWasteland(int id);

    /**
     * Updates the region given by its id to its new owner and number of armies.
     */
    void updateRegion(int region, Player owner, int armies);

    /**
     * Returns a reference to the Region object uniquely identified by the
     * given id.
     */
    RegionPtr getRegionById(int regionId) const;

    /**
     * Returns a reference to the SuperRegion object uniquely identified by the
     * given id.
     */
    SuperRegionPtr getSuperRegionById(int superRegionId) const;

    /**
     * Returns a list of references to all regions owned by the given player.
     */
    VecOfRegionPtrs getRegionsOwnedBy(warlightAi::Player player) const;

    // TODO
    VecOfRegionPtrs getRegionsInFogOf(warlightAi::Player player) const;

    // TODO
    bool isInFogOf(RegionPtr reg, warlightAi::Player player) const;

private:
    VecOfRegionPtrs m_regions;
    VecOfSuperRegionPtrs m_superRegions;

}; // class warlightAi


} // namespace warlightAi

#endif // WORLD_H_INCLUDED
