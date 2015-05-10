// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "basicroundstrategy.hpp"

// Project
#include "world.hpp"
#include "region.hpp"
#include "common/statistics.hpp"

// C++
#include <limits>


namespace warlightAi {

BasicRoundStrategy::BasicRoundStrategy(const World &world, int availableArmies)
    : RoundStrategy(world, availableArmies)
{
    using common::Statistics;

    std::vector<std::pair<RegionPtr, RegionPtr>> myRegOtherRegPairs;

    // Finds all the adjacent super regions and keep all the (src, dest) pairs
    // for possible attacks
    for (auto &myReg : m_world.getRegionsOwnedBy(Player::ME))
        for (auto &otherReg : myReg->getNeighbors())
            if (otherReg->getOwner() != Player::ME)
                myRegOtherRegPairs.emplace_back(myReg, otherReg);

    std::pair<RegionPtr, RegionPtr> maxPair;
    auto maxScore = std::numeric_limits<int>::lowest();

    for (auto &p : myRegOtherRegPairs) {
        auto score = simulationScore(p.second->getSuperRegion(), availableArmies);

        if (score > maxScore) {
            maxScore = score;
            maxPair = p;
        }
    }

    m_deployments.emplace_back(maxPair.first, availableArmies);
    m_attacks.emplace_back(maxPair.first,
                           maxPair.second,
                           maxPair.first->getArmies() - 1);
}

VecOfPairs BasicRoundStrategy::getDeployments() const
{
    return m_deployments;
}

VecOfTuples BasicRoundStrategy::getAttacks() const
{
    return m_attacks;
}

double BasicRoundStrategy::simulationScore(SuperRegionPtr superRegion,
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
