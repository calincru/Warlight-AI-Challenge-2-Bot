// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef WEIGHTED_PICK_STRATEGY_H
#define WEIGHTED_PICK_STRATEGY_H

// Project
#include "pickstrategy.hpp"

namespace warlightAi {

// Fwrd decls
class World;

// TODO
class WeightedPickStrategy : public PickStrategy
{
public:
    /**
     * @see PickStrategy::PickStrategy
     */
    WeightedPickStrategy(const RegionPtrSet &startingRegions);

    RegionPtr pickNext(const RegionPtrSet &pickableRegions) const override;

private:
    double rewardsBasedScore(SuperRegionPtr superReg) const;
    double distanceToMinesScore(RegionPtr reg) const;
    double distanceToEnemyScore(RegionPtr reg) const;

}; // class QuickPickStrategy

} // namespace warlightAi

#endif // WEIGHTED_PICK_STRATEGY_H
