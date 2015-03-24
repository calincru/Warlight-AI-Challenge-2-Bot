// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "QuickPickStrategy.h"

// Project
#include "globals.h"
#include "Region.h"
#include "SuperRegion.h"

// C++
#include <queue>
#include <limits>
#include <iostream>
#include <cassert>

namespace warlightAi {

QuickPickStrategy::QuickPickStrategy(const VecOfRegionPtrs &startingRegions)
    : PickStrategy(startingRegions)
{
}

RegionPtr QuickPickStrategy::pickNext(const VecOfRegionPtrs &pickableRegions) const
{
    auto pq_cmp = [](const auto &lhs, const auto &rhs) {
        return lhs.first > rhs.first;
    };
    std::priority_queue<
                        std::pair<int, RegionPtr>,
                        std::vector<std::pair<int, RegionPtr>>,
                        decltype(pq_cmp)
                       > pq(pq_cmp);

    for (auto &reg : pickableRegions) {
        auto wastelands = 0;
        for (auto &subReg : reg->getSuperRegion()->getSubRegions())
            if (subReg->getArmies() == WASTELAND_ARMY_COUNT)
                ++wastelands;

        pq.emplace(wastelands, reg);
    }

    auto minimumWastelands = pq.top().first;

    auto maximum = std::numeric_limits<double>::lowest();
    auto pickedReg = static_cast<RegionPtr>(nullptr);

    while (!pq.empty() && pq.top().first == minimumWastelands) {
        auto currEntry = pq.top();
        pq.pop();

        auto currentMax = superRegionScore(currEntry.second->getSuperRegion());
        if (currentMax > maximum) {
            maximum = currentMax;
            pickedReg = currEntry.second;
        }
    }

    return pickedReg;
}

double QuickPickStrategy::superRegionScore(const SuperRegionPtr superReg) const
{
    auto subRegs = superReg->getSubRegions();

    if (subRegs.size() == 1)
        return 2.;
    return (superReg->getReward() * 1.) / (subRegs.size() - 1);
}

} // namespace warlightAi
