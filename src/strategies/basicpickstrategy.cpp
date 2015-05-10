// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "basicpickstrategy.hpp"

// Project
#include "utils.hpp"
#include "globals.hpp"
#include "world.hpp"
#include "common/basicscore.hpp"

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
    using common::BasicScore;

    auto maxReg = static_cast<RegionPtr>(nullptr);
    auto maxScore = std::numeric_limits<int>::lowest();

    for (auto &regionPtr : pickableRegions) {
        auto score = BasicScore::simulationScore(regionPtr->getSuperRegion(),
                                                 INIT_AVAILABLE_ARMIES);

        if (score > maxScore) {
            maxScore = score;
            maxReg = regionPtr;
        }
    }

    return maxReg;
}

} // namespace warlightAi
