// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "weightedpickstrategy.hpp"

// Project
#include "globals.hpp"
#include "utils.hpp"
#include "region.hpp"
#include "superregion.hpp"

// C++
#include <queue>
#include <limits>

namespace warlightAi {

WeightedPickStrategy::WeightedPickStrategy(const RegionPtrSet &startingRegions)
    : PickStrategy(startingRegions)
{
}

RegionPtr WeightedPickStrategy::pickNext(const RegionPtrSet &pickableRegions) const
{
    // TODO
    UNUSED(pickableRegions);

    return RegionPtr{};
}

double WeightedPickStrategy::rewardsBasedScore(SuperRegionPtr superReg) const
{
    // TODO
    UNUSED(superReg);

    return 0.;
}

double WeightedPickStrategy::distanceToMinesScore(RegionPtr reg) const
{
    // TODO
    UNUSED(reg);

    return 0.;
}

double WeightedPickStrategy::distanceToEnemyScore(RegionPtr reg) const
{
    // TODO
    UNUSED(reg);

    return 0.;
}

} // namespace warlightAi
