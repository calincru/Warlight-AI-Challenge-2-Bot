// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "quickpickstrategy.hpp"

// Project
#include "globals.hpp"
#include "region.hpp"
#include "superregion.hpp"

// C++
#include <queue>
#include <limits>

namespace warlightAi {

QuickPickStrategy::QuickPickStrategy(const RegionPtrSet &startingRegions)
    : PickStrategy(startingRegions)
{
}

// Cum arata cum functia, ar trebui sa fie tranformata complet in functia
// care da scorul bazat pe raportul dintre numarul de subregiuni si reward,
// insa ar trebui sa facem cumva ca scorul sa tina cont de nr de armate din
// super regiune. Astfel, vor fi defavorizate cele cu wastelandsuri si fix
// asta vrem.
RegionPtr QuickPickStrategy::pickNext(const RegionPtrSet &pickableRegions) const
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

double QuickPickStrategy::superRegionScore(SuperRegionPtr superReg) const
{
    auto subRegs = superReg->getSubRegions();

    if (subRegs.size() == 1)
        return 2.;
    return (superReg->getReward() * 1.) / (subRegs.size() - 1);
}

} // namespace warlightAi
