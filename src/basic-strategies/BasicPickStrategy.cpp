// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "BasicPickStrategy.h"

// Project
#include "utils.h"
#include "World.h"
#include "common/ScoreComputer.h"


namespace warlightAi {

BasicPickStrategy::BasicPickStrategy(const VecOfRegionPtrs &startingRegions,
                                     int availableArmies)
    : PickStrategy(startingRegions)
    , m_availableArmies(availableArmies)
{
    UNUSED(startingRegions);
}

RegionPtr BasicPickStrategy::pickNext(const VecOfRegionPtrs &pickableRegions) const
{
    using common::ScoreComputer;

    auto maxReg = static_cast<RegionPtr>(nullptr);
    auto maxScore = -1;
    for (auto &regionPtr : pickableRegions) {
        auto score = ScoreComputer::simulationScore(
                                            regionPtr->getSuperRegion(),
                                            m_availableArmies);
        if (score > maxScore) {
            maxScore = score;
            maxReg = regionPtr;
        }
    }

    return maxReg;
}

} // namespace warlightAi
