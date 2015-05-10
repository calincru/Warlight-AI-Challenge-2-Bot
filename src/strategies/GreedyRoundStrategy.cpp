// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "GreedyRoundStrategy.hpp"

// Project
#include "globals.hpp"
#include "World.hpp"
#include "Region.hpp"
#include "SuperRegion.hpp"
#include "common/Statistics.hpp"

// C++
#include <queue>
#include <limits>
#include <iostream>
#include <algorithm>


namespace warlightAi {

GreedyRoundStrategy::GreedyRoundStrategy(const World &world,
                                         int availableArmies)
    : RoundStrategy(world, availableArmies)
    , m_availArmies(availableArmies)
{
    for (auto &neigh : getSpoilableRegions()) {
        if (m_availArmies <= 0)
            break;

        handleSpoilingAttack(neigh);
    }

    for (auto &neigh : getTargetedOppRegions()) {
        if (m_availArmies <= 0)
            break;

        handleExpandingAttack(neigh);
   }

    if (m_availArmies)
        handleRemainingArmies();

    computeMigrations();
    std::reverse(m_attacks.begin(), m_attacks.end());
}

VecOfPairs GreedyRoundStrategy::getDeployments() const
{
    return m_deployments;
}

VecOfTuples GreedyRoundStrategy::getAttacks() const
{
    return m_attacks;
}

auto GreedyRoundStrategy::getSpoilableRegions() const
    -> std::vector<std::pair<RegionPtr, RegionPtr>>
{
    auto pq_cmp = [](const auto &lhs, const auto &rhs) -> bool {
        return std::get<0>(lhs) < std::get<0>(rhs);
    };
    std::priority_queue<
                        std::tuple<double, RegionPtr, RegionPtr>,
                        std::vector<std::tuple<double, RegionPtr, RegionPtr>>,
                        decltype(pq_cmp)
                       > pq(pq_cmp);
    std::unordered_set<SuperRegionPtr> supers;

    for (auto &myReg : m_world.getRegionsOwnedBy(Player::ME))
        for (auto &neigh : myReg->getNeighbors())
            supers.emplace(neigh->getSuperRegion());

    for (auto &super : supers) {
        bool isSpoilable = true;

        for (auto &reg : super->getSubRegions())
            if (!m_world.isInFogOf(reg, Player::ME)
                    && reg->getOwner() != Player::OPPONENT) {
                isSpoilable = false;
                break;
            }

        if (isSpoilable)
            pq.emplace(spoilingScore(super));
    }

    std::vector<std::pair<RegionPtr, RegionPtr>> spoilables;

    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();

        spoilables.emplace_back(std::get<1>(top), std::get<2>(top));
    }

    return spoilables;
}

VecOfRegionPtrs GreedyRoundStrategy::getTargetedOppRegions() const
{
    auto pq_cmp = [](const auto &lhs, const auto &rhs) -> bool {
        return lhs.first < rhs.first;
    };
    std::priority_queue<
                        std::pair<double, SuperRegionPtr>,
                        std::vector<std::pair<double, SuperRegionPtr>>,
                        decltype(pq_cmp)
                       > pq(pq_cmp);
    std::unordered_set<SuperRegionPtr> supers;

    for (auto &myReg : m_world.getRegionsOwnedBy(Player::ME))
        for (auto &neigh : myReg->getNeighbors()) {
            if (supers.count(neigh->getSuperRegion()))
                continue;

            pq.emplace(
                wastelandsBasedScore(neigh->getSuperRegion()),
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
            if (reg->getOwner() == Player::ME || addedRegs.count(reg))
                continue;

            for (auto &regNeigh : reg->getNeighbors())
                if (regNeigh->getOwner() == Player::ME) {
                    addedRegs.emplace(reg);
                    neighs.emplace_back(reg);
                    break;
                }
        }
    }

    return neighs;
}

void GreedyRoundStrategy::handleSpoilingAttack(
        const std::pair<RegionPtr, RegionPtr> &meToOp)
{
    using common::Statistics;

    auto opp = meToOp.second;
    auto mine = meToOp.first;

    auto armiesNeeded = Statistics::armiesNeeded(opp->getArmies(), 0.7) + 1;
    auto diff = armiesNeeded - mine->getArmies();

    if (diff > m_availArmies) {
        return;
    } else if (diff > 0) {
        mine->setArmies(mine->getArmies() + diff);
        m_deployments.emplace_back(mine, diff);
        m_availArmies -= diff;
    }

    m_attacks.emplace_back(mine, opp, mine->getArmies() - 1);
    mine->setArmies(1);
}

void GreedyRoundStrategy::handleExpandingAttack(RegionPtr reg)
{
    using common::Statistics;

    auto biggestReg = static_cast<RegionPtr>(nullptr);
    auto maxArmies = std::numeric_limits<int>::lowest();

    for (auto &mine : reg->getNeighbors()) {
        if (mine->getOwner() != Player::ME)
            continue;

        if (mine->getArmies() > maxArmies) {
            maxArmies = mine->getArmies();
            biggestReg = mine;
        }
    }

    auto armiesNeeded = Statistics::armiesNeeded(reg->getArmies(), 0.7) + 1;
    auto diff = armiesNeeded - maxArmies;

    if (diff > m_availArmies) {
        return;
    } else if (diff > 0) {
        biggestReg->setArmies(biggestReg->getArmies() + diff);
        m_deployments.emplace_back(biggestReg, diff);
        m_availArmies -= diff;
    }

    m_attacks.emplace_back(biggestReg, reg, biggestReg->getArmies() - 1);
    biggestReg->setArmies(1);
}

void GreedyRoundStrategy::handleRemainingArmies()
{
    if (m_attacks.size()) {
        m_deployments.emplace_back(std::get<0>(m_attacks.front()), m_availArmies);
        std::get<2>(m_attacks.front()) += m_availArmies;
    } else {
        auto maxArmies = std::numeric_limits<int>::lowest();
        auto maxReg = static_cast<RegionPtr>(nullptr);

        for (auto &mine : m_world.getRegionsOwnedBy(Player::ME))
            if (mine->getArmies() > maxArmies) {
                maxArmies = mine->getArmies();
                maxReg = mine;
            }

        m_deployments.emplace_back(maxReg, m_availArmies);
    }
}

void GreedyRoundStrategy::computeMigrations()
{
    auto visitedRegs = getRegionsOnBorder();
    std::queue<RegionPtr> Q;

    for (auto &reg : visitedRegs)
        Q.emplace(reg);

    while (!Q.empty()) {
        auto nextReg = Q.front();

        Q.pop();
        for (auto &neigh : nextReg->getNeighbors()) {
            if (neigh->getOwner() != Player::ME || visitedRegs.count(neigh))
                continue;

            visitedRegs.emplace(neigh);
            Q.emplace(neigh);
            if (neigh->getArmies() > 1)
                m_attacks.emplace_back(neigh, nextReg, neigh->getArmies() - 1);
        }
    }
}

std::unordered_set<RegionPtr> GreedyRoundStrategy::getRegionsOnBorder() const
{
    std::unordered_set<RegionPtr> regsOnBorder;

    for (auto &mine : m_world.getRegionsOwnedBy(Player::ME)) {
        auto isOnBorder = false;

        for (auto &neigh : mine->getNeighbors())
            if (neigh->getOwner() != Player::ME) {
                isOnBorder = true;
                break;
            }

        if (isOnBorder)
            regsOnBorder.emplace(mine);
    }

    return regsOnBorder;
}

double GreedyRoundStrategy::wastelandsBasedScore(SuperRegionPtr superRegion) const
{
    auto score = superRegion->getReward() * 1.;
    auto minesSum = 0;
    auto minesCount = 0;
    auto oppCount = 0;
    auto oppSum = 0;

    for (auto &subReg : superRegion->getSubRegions())
        if (subReg->getOwner() == Player::ME) {
            ++minesCount;
            minesSum += subReg->getArmies();
        } else {
            ++oppCount;
            oppSum += subReg->getArmies();
        }

    if (!oppCount)
        return -1.;

    score *= (minesCount * 1.)/oppCount;
    score *= (minesSum * 1.)/oppSum;

    return score / (minesCount + oppCount);
}

auto GreedyRoundStrategy::spoilingScore(SuperRegionPtr superRegion) const
    -> std::tuple<double, RegionPtr, RegionPtr>
{
    auto pq_cmp = [](const auto &lhs, const auto &rhs) -> bool {
        return (1. * lhs.second->getArmies()) / lhs.first->getArmies()
                > (1. * rhs.second->getArmies()) / rhs.first->getArmies();
    };
    std::priority_queue<
                        std::pair<RegionPtr, RegionPtr>,
                        std::vector<std::pair<RegionPtr, RegionPtr>>,
                        decltype(pq_cmp)
                       > pq(pq_cmp);

    for (auto &reg : superRegion->getSubRegions()) {
        if (m_world.isInFogOf(reg, Player::ME))
            continue;

        for (auto &neigh : reg->getNeighbors())
            if (neigh->getOwner() == Player::ME)
                pq.emplace(neigh, reg);
    }

    auto top = pq.top();
    auto ratio = (1. * top.second->getArmies()) / top.first->getArmies();

    return std::make_tuple(ratio, top.first, top.second);
}

} // namespace warlightAi
