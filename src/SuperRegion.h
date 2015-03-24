// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef SUPERREGION_H_INCLUDED
#define SUPERREGION_H_INCLUDED

// Project
#include "globals.h"

// C++
#include <memory>


namespace warlightAi {

// Fwrd decls
class Region;

/**
 * Class to represent a single super region on the map of the game.
 */
class SuperRegion
{
    using RegionWkPtr = std::weak_ptr<Region>;

public:
    /**
     * The sole constructor which creates a new super region given its id and
     * the reward it gives when owned.
     */
    SuperRegion(int id, int reward);

    /**
     * Returns the id of this super region. This is an unique integer based on
     * the input received from the engine.
     */
    int id() const;

    /**
     * Adds the given region as one of this super region's sub regions.
     */
    void addSubRegion(RegionPtr subRegion);

    /**
     * Returns true if this super region contains the given region.
     */
    bool containsRegion(RegionPtr region) const;

    /**
     * Returns a list of references to this super region's sub regions.
     */
    std::vector<RegionPtr> getSubRegions() const;

    /**
     * Returns a list of references to regions which are not part of this
     * super region but are surrounding it.
     */
    std::vector<RegionPtr> getSurroundingRegions() const;

    /**
     * Checks if this super region is owned by the player given as argument.
     */
    bool isOwnedBy(warlightAi::Player player) const;

    /**
     * Returns the reward given by this super region when entirely owned by a
     * a player.
     */
    int getReward() const;

private:
    int m_id;
    int m_reward;
    std::vector<RegionWkPtr> m_subRegions;

}; // class SuperRegion

} // namespace warlightAi

#endif // SUPERREGION_H_INCLUDED
