// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "GreedyRoundStrategy.h"

// Project
#include "globals.h"
#include "World.h"
#include "Region.h"
#include "SuperRegion.h"
#include "common/ScoreComputer.h"

// C++
#include <queue>
#include <unordered_set>
#include <limits>
#include <iostream>


namespace warlightAi {

GreedyRoundStrategy::GreedyRoundStrategy(const World &world,
                                         int availableArmies)
    : RoundStrategy(world, availableArmies)
{
    using common::ScoreComputer;

    using ScoreSuperPair = std::pair<int, SuperRegionPtr>;
    auto pq_cmp = [](const auto &lhs, const auto &rhs) -> bool {
        return lhs.first < rhs.first;
    };
    std::priority_queue<
                        ScoreSuperPair,
                        std::vector<ScoreSuperPair>,
                        decltype(pq_cmp)
                       > pq(pq_cmp);
    std::unordered_set<SuperRegionPtr> supers;

    for (auto &myReg : world.getRegionsOwnedBy(Player::ME))
        for (auto &neigh : myReg->getNeighbors()) {
            if (supers.count(neigh->getSuperRegion()))
                continue;

            pq.emplace(
                ScoreComputer::superRegionScore(neigh->getSuperRegion()),
                neigh->getSuperRegion()
            );
            supers.emplace(neigh->getSuperRegion());
        }

    std::unordered_set<RegionPtr> addedRegs;
    VecOfRegionPtrs neighs;
    while (!pq.empty()) {
        auto superReg = pq.top().second;
        pq.pop();

        for (auto &reg : superReg->getSubRegions()) {
            if (reg->getOwner() == Player::ME)
                continue;

            for (auto &regNeigh : reg->getNeighbors())
                if (regNeigh->getOwner() == Player::ME) {
                    auto isAlready = false;
                    for (auto &already : neighs)
                        if (already == reg) {
                            isAlready = true;
                            break;
                        }
                    if (!isAlready)
                        neighs.emplace_back(reg);
                    break;
                }
        }
    }

    for (auto &neigh : neighs) {
        if (availableArmies <= 0)
            break;

        auto biggestReg = static_cast<RegionPtr>(nullptr);
        auto maxArmies = std::numeric_limits<int>::min();
        for (auto &mine : neigh->getNeighbors()) {
            if (mine->getOwner() != Player::ME)
                continue;

            if (mine->getArmies() > maxArmies) {
                maxArmies = mine->getArmies();
                biggestReg = mine;
            }
        }

        auto armiesNeeded = ScoreComputer::armiesNeeded(neigh->getArmies(), 0.7) + 1;
        auto diff = armiesNeeded - maxArmies;
        if (diff > availableArmies) {
            continue;
        } else if (diff > 0) {
            biggestReg->setArmies(biggestReg->getArmies() + diff);
            m_deployments.emplace_back(biggestReg, diff);
            availableArmies -= diff;
        }

        m_attacks.emplace_back(biggestReg, neigh, biggestReg->getArmies() - 1);
        biggestReg->setArmies(1);
    }

    if (availableArmies && m_attacks.size()) {
        m_deployments.emplace_back(std::get<0>(m_attacks.front()), availableArmies);
        std::get<2>(m_attacks.front()) += availableArmies;
    } else if (availableArmies) {
        auto maxArmies = std::numeric_limits<int>::min();
        auto maxReg = static_cast<RegionPtr>(nullptr);
        for (auto &mine : world.getRegionsOwnedBy(Player::ME))
            if (mine->getArmies() > maxArmies) {
                maxArmies = mine->getArmies();
                maxReg = mine;
            }

        m_deployments.emplace_back(maxReg, availableArmies);
    }
}

VecOfPairs GreedyRoundStrategy::getDeployments() const
{
    return m_deployments;
}

VecOfTuples GreedyRoundStrategy::getAttacks() const
{
    return m_attacks;
}


} // namespace warlightAi
