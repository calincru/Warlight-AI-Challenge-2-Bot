// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "BasicPickStrategy.h"

// Project
#include "utils.h"
#include "globals.h"
#include "World.h"
#include "common/ScoreComputer.h"

// C++
#include <limits>


namespace warlightAi {

BasicPickStrategy::BasicPickStrategy(const VecOfRegionPtrs &startingRegions)
    : PickStrategy(startingRegions)
{
}

BasicPickStrategy::~BasicPickStrategy()
{
}

RegionPtr BasicPickStrategy::pickNext(const VecOfRegionPtrs &pickableRegions) const
{
    using common::ScoreComputer;

    auto maxReg = static_cast<RegionPtr>(nullptr);
    auto maxScore = std::numeric_limits<int>::lowest();
    for (auto &regionPtr : pickableRegions) {
        auto score = ScoreComputer::simulationScore(
                                            regionPtr->getSuperRegion(),
                                            INIT_AVAILABLE_ARMIES);
        if (score > maxScore) {
            maxScore = score;
            maxReg = regionPtr;
        }
    }

    return maxReg;
}

} // namespace warlightAi
