// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "greedyroundstrategy.hpp"

// Project
#include "globals.hpp"
#include "world.hpp"
#include "region.hpp"
#include "superregion.hpp"
#include "common/statistics.hpp"

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
    for (auto &reg : m_world.getRegionsOwnedBy(Player::ME))
        m_regToArmiesBfr.emplace(reg, reg->getArmies());

    for (auto &superReg : m_world.getSuperRegionsOwnedBy(Player::ME)) {
        if (m_availArmies <= 0)
            break;

        handleDefendingSuperReg(superReg);
    }

    for (auto &neigh : getSpoilableRegions()) {
        if (m_availArmies <= 0)
            break;

        handleSpoilingAttack(neigh);
    }

    for (auto &neigh : getHostileRegions()) {
        if (m_availArmies <= 0)
            break;

        handleHostileAttack(neigh);
   }

    if (m_availArmies)
        handleRemainingArmies();

    migrate();
    std::reverse(m_attacks.begin(), m_attacks.end());
}

RegIntList GreedyRoundStrategy::getDeployments() const
{
    return m_deployments;
}

RegRegIntList GreedyRoundStrategy::getAttacks() const
{
    return m_attacks;
}

auto GreedyRoundStrategy::getSpoilableRegions() const -> RegRegList
{
    auto pq_cmp = [](const auto &lhs, const auto &rhs) {
        return std::get<0>(lhs) < std::get<0>(rhs);
    };
    std::priority_queue<
                        DoubleRegReg,
                        std::vector<DoubleRegReg>,
                        decltype(pq_cmp)
                       > pq(pq_cmp);
    SuperRegionPtrSet supers;

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
            pq.emplace(spoilingScoreTuple(super));
    }

    RegRegList spoilables;

    while (!pq.empty()) {
        auto top = pq.top();

        pq.pop();
        spoilables.emplace_back(std::get<1>(top), std::get<2>(top));
    }

    return spoilables;
}

RegionPtrList GreedyRoundStrategy::getHostileRegions() const
{
    auto pq_cmp = [](const auto &lhs, const auto &rhs) {
        return lhs.first < rhs.first;
    };
    std::priority_queue<
                        std::pair<double, SuperRegionPtr>,
                        std::vector<std::pair<double, SuperRegionPtr>>,
                        decltype(pq_cmp)
                       > pq(pq_cmp);
    SuperRegionPtrSet supers;

    for (auto &myReg : m_world.getRegionsOwnedBy(Player::ME))
        for (auto &neigh : myReg->getNeighbors()) {
            if (supers.count(neigh->getSuperRegion()))
                continue;

            pq.emplace(
                superRegionsScore(neigh->getSuperRegion()),
                neigh->getSuperRegion()
            );
            supers.emplace(neigh->getSuperRegion());
        }

    RegionPtrSet addedRegs;
    RegionPtrList neighs;

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

void GreedyRoundStrategy::handleDefendingSuperReg(SuperRegionPtr superReg)
{
    using common::Statistics;

    auto pq_cmp = [](const auto &lhs, const auto &rhs) {
        return lhs.second->getArmies() < rhs.second->getArmies();
    };
    std::priority_queue<
                        std::pair<RegionPtr, RegionPtr>,
                        std::vector<std::pair<RegionPtr, RegionPtr>>,
                        decltype(pq_cmp)
                       > pq(pq_cmp);

    for (auto &mine : superReg->getSubRegions()) {
        auto maxOppArmies = 0;
        auto oppReg = static_cast<RegionPtr>(nullptr);

        for (auto &his : mine->getNeighbors())
            if (his->getOwner() == Player::OPPONENT
                    && his->getArmies() > maxOppArmies) {
                maxOppArmies = his->getArmies();
                oppReg = his;
            }

        if (!oppReg)
            continue;

        pq.emplace(mine, oppReg);
    }

    while (!pq.empty() && m_availArmies > 0) {
        auto mine = pq.top().first;
        auto his = pq.top().second;

        pq.pop();

        auto myArmies = mine->getArmies();
        auto armies = Statistics::armiesNeeded(myArmies);
        auto diff = his->getArmies() - armies - 1;

        if (diff <= 0) {
            m_deployments.emplace_back(mine, 1);
            m_availArmies -= 1;

            // Tested with not "telling" the following steps about the change.
            // It resulted in not a significant improvement.
            //
            // The same applies for below.
            mine->setArmies(myArmies + 1);
        } else {
            auto inverse = Statistics::revArmiesNeeded(his->getArmies()) + 1;

            if (inverse - myArmies <= m_availArmies) {
                m_deployments.emplace_back(mine, inverse - myArmies);
                m_availArmies -= inverse - myArmies;
                mine->setArmies(inverse);
            }
        }
    }
}

void GreedyRoundStrategy::handleSpoilingAttack(const RegRegPair &meToOp)
{
    using common::Statistics;

    auto opp = meToOp.second;
    auto mine = meToOp.first;

    auto armiesNeeded = Statistics::armiesNeeded(opp->getArmies()) + 1;
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

void GreedyRoundStrategy::handleHostileAttack(RegionPtr reg)
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

    auto armiesNeeded = Statistics::armiesNeeded(reg->getArmies()) + 1;
    auto diff = armiesNeeded - maxArmies;

    if (diff > m_availArmies) {
        return;
    } else if (diff > 0) {
        biggestReg->setArmies(biggestReg->getArmies() + diff);
        m_deployments.emplace_back(biggestReg, diff);
        m_availArmies -= diff;
    }

    // Check if the attack already exists and acccumulate it.
    bool found = false;

    for (auto &attack : m_attacks)
        if (std::get<0>(attack) == biggestReg && std::get<1>(attack) == reg) {
            std::get<2>(attack) += biggestReg->getArmies() - 1;
            found = true;
            break;
        }

    if (!found)
        m_attacks.emplace_back(biggestReg, reg, biggestReg->getArmies() - 1);

    biggestReg->setArmies(1);
}

void GreedyRoundStrategy::handleRemainingArmies()
{
    remainingArmiesDefStrategy();
    remainingArmiesNvmStrategy();
}

void GreedyRoundStrategy::remainingArmiesDefStrategy()
{
    using common::Statistics;

    auto oppRegsSet = m_world.getRegionsOwnedBy(Player::OPPONENT);
    RegionPtrList oppRegs{oppRegsSet.begin(), oppRegsSet.end()};

    auto regSort = [](const auto &lhs, const auto &rhs) {
        return lhs->getArmies() > rhs->getArmies();
    };
    std::sort(oppRegs.begin(), oppRegs.end(), regSort);

    for (auto &reg : oppRegs)
        for  (auto &myReg : reg->getNeighbors()) {
            if (m_availArmies <= 0)
                return;

            if (myReg->getOwner() != Player::ME)
                continue;

            auto myRegBfr = m_regToArmiesBfr.at(myReg);
            auto armies = Statistics::armiesNeeded(myRegBfr);
            auto diff = reg->getArmies() - armies - 1;

            if (diff <= 0) {
                m_deployments.emplace_back(myReg, 1);
                m_availArmies -= 1;
                myReg->setArmies(myReg->getArmies() + 1);
            } else {
                auto inverse = Statistics::revArmiesNeeded(reg->getArmies()) + 1;

                if (inverse - myRegBfr <= m_availArmies) {
                    m_deployments.emplace_back(myReg, inverse - myRegBfr);
                    m_availArmies -= inverse - myRegBfr;
                    myReg->setArmies(myReg->getArmies() + inverse - myRegBfr);
                }
            }
        }
}

void GreedyRoundStrategy::remainingArmiesOffStrategy()
{
    // TODO
}

void GreedyRoundStrategy::remainingArmiesNvmStrategy()
{
    if (m_attacks.size() && m_availArmies > 0) {
        m_deployments.emplace_back(std::get<0>(m_attacks.front()), m_availArmies);
        std::get<2>(m_attacks.front()) += m_availArmies;
    } else if (m_availArmies > 0) {
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

void GreedyRoundStrategy::migrate()
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

RegionPtrSet GreedyRoundStrategy::getRegionsOnBorder() const
{
    RegionPtrSet regsOnBorder;

    for (auto &mine : m_world.getRegionsOwnedBy(Player::ME)) {
        auto isOnBorder = false;

        for (auto &neigh : mine->getNeighbors())
            if (neigh->getOwner() == Player::OPPONENT) {
                isOnBorder = true;
                break;
            }

        if (isOnBorder)
            regsOnBorder.emplace(mine);
    }

    return regsOnBorder;
}

double GreedyRoundStrategy::superRegionsScore(SuperRegionPtr superRegion) const
{
    auto score = superRegion->getReward() * 1.;
    auto minesSum = 0;
    auto minesCount = 0;
    auto oppCount = 0;
    auto oppSum = 0;
    auto neutralCount = 0;
    auto neutralSum = 0;

    for (auto &subReg : superRegion->getSubRegions())
        if (subReg->getOwner() == Player::ME) {
            ++minesCount;
            minesSum += subReg->getArmies();
        } else if (subReg->getOwner() == Player::OPPONENT) {
            ++oppCount;
            oppSum += subReg->getArmies();
        } else {
            ++neutralCount;
            neutralSum += subReg->getArmies();
        }

    if (oppCount + neutralCount == 0)
        return -1.;

    if (!minesCount)
        minesCount = minesSum = 1;

    // Added just for trial => 10% improvement lel. Reason for this in the
    // future.
    score *= (minesCount * 1.)/(0.8 * neutralCount + 0.2 * oppCount);
    score *= (minesSum * 1.)/(0.8 * neutralSum + 0.2 * oppSum);

    return score / (minesCount + neutralCount + oppCount);
}

auto GreedyRoundStrategy::spoilingScoreTuple(SuperRegionPtr superRegion) const
    -> DoubleRegReg
{
    auto pq_cmp = [](const auto &lhs, const auto &rhs) {
        return (1. * lhs.second->getArmies()) / lhs.first->getArmies()
                > (1. * rhs.second->getArmies()) / rhs.first->getArmies();
    };
    std::priority_queue<
                        RegRegPair,
                        std::vector<RegRegPair>,
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
