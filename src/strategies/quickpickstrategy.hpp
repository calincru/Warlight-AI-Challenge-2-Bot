// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef QUICK_PICK_STRATEGY_H
#define QUICK_PICK_STRATEGY_H

// Project
#include "pickstrategy.hpp"


namespace warlightAi {

// Fwrd decls
class World;

/**
 * One of our the picking strategies. This is "Quick" because what is does is
 * to check the number of wastelands and pick the ones which have the least.
 */
class QuickPickStrategy : public PickStrategy
{
public:
    /**
     * @see PickStrategy::PickStrategy
     */
    QuickPickStrategy(const VecOfRegionPtrs &startingRegions);

    /**
     * Returns a reference to the Region to be picked at this picking phase,
     * based on the "Quick" algorithm, briefly described above.
     */
    RegionPtr pickNext(const VecOfRegionPtrs &pickableRegions) const override;

private:
    /**
     * Computes a score for the given super region. This is used in the decision
     * made in the pickNext method.
     */
    double superRegionScore(SuperRegionPtr superReg) const;

}; // class QuickPickStrategy

} // namespace warlightAi

#endif // QUICK_PICK_STRATEGY_H
