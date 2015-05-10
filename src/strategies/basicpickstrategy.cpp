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
#include "common/statistics.hpp"

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
    auto maxReg = static_cast<RegionPtr>(nullptr);
    auto maxScore = std::numeric_limits<int>::lowest();

    for (auto &regionPtr : pickableRegions) {
        auto score = simulationScore(regionPtr->getSuperRegion(),
                                     INIT_AVAILABLE_ARMIES);

        if (score > maxScore) {
            maxScore = score;
            maxReg = regionPtr;
        }
    }

    return maxReg;
}

double BasicPickStrategy::simulationScore(SuperRegionPtr superRegion,
                                          int availableArmies) const
{
    using common::Statistics;

    auto subRegs = superRegion->getSubRegions();
    double sum = -subRegs.size();

    for (auto &reg : subRegs)
        if (reg->getOwner() == Player::ME)
            sum += reg->getArmies();
        else
            sum -= Statistics::armiesNeeded(reg->getArmies(), 0.7);

    if (sum > 0)
        sum = 0.;

    return (10 - (-sum)/availableArmies) * superRegion->getReward();
}

} // namespace warlightAi
