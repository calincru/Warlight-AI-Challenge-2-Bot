// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef GREEDY_ROUND_STRATEGY_H_INCLUDED
#define GREEDY_ROUND_STRATEGY_H_INCLUDED

// Project
#include "roundstrategy.hpp"

// C++
#include <unordered_set>
#include <unordered_map>

namespace warlightAi {

/**
 * The main round strategy of our Bot at this phase. This is based on a greedy
 * algorithm.
 *
 *
 */
class GreedyRoundStrategy : public RoundStrategy
{
    using RegRegPair = std::pair<RegionPtr, RegionPtr>;
    using RegRegList = std::vector<RegRegPair>;
    using DoubleRegReg = std::tuple<double, RegionPtr, RegionPtr>;
    using RegToIntMap = std::unordered_map<RegionPtr, int>;

public:
    /**
     * @see RoundStrategy::RoundStrategy
     */
    GreedyRoundStrategy(const World &world, int availableArmies);

    /**
     * Return the deployments and the attacks our Bot should do during this
     * round.
     */
    RegIntList getDeployments() const override;
    RegRegIntList getAttacks() const override;

private:
    /**
     * Returns the list of "spoilable" regions. The term is defined in the
     * description of the class.
     */
     RegRegList getSpoilableRegions() const;

    /**
     * Returns a list of regions in a non-decreasing order (by importance) which
     * should be attacked by our bot in order to expand.
     */
    RegionPtrList getHostileRegions() const;

    /**
     * Takes a pair (myReg, oppReg), meaning that we should attack from `myReg`,
     * the opponent's region `oppReg` in order to spoil its super region. This
     * function decides how many armies we should attack with.
     *
     * More about this in the description of the class.
     */
    void handleSpoilingAttack(const RegRegPair &meToOp);

    /**
     * Takes a region of the opponent which our bot should attack in order to
     * expand.
     */
    void handleHostileAttack(RegionPtr reg);

    /**
     * If after the previous attack/transfer function handlers we still have
     * available armies, they are taken care of in these functions.
     *
     * We have two different strategies for this phase, but only one is
     * currently implemented - the deffensive one.
     *
     * The `Nvm` strategy is the "last resort" function, in case we still have
     * available armies.
     */
    void handleRemainingArmies();
    void remainingArmiesDefStrategy();
    void remainingArmiesOffStrategy();
    void remainingArmiesNvmStrategy();

    /**
     * After computing the attacks, this strategy is trying to get all our
     * armies from regions which do not have any enemy neighbor in the
     * direction where there are enemy armies.
     */
    void migrate();

    /**
     * Helper function, used by the above method, which returns a set
     * containing the regions our Bot owns and have at least one enemy
     * neighbor.
     */
    RegionPtrSet getRegionsOnBorder() const;

    /**
     * Returns a score associated to the given super region. This score is used
     * with the intent to conquer this super region and the score represents
     * the priority our bot should do this with.
     */
    double superRegionsScore(SuperRegionPtr superRegion) const;

    /**
     * Returns a tuple (score, myReg, oppReg) meaning that the pair
     * (myReg, oppReg) should be considered as a spoiling move with the `score`
     * priority.
     */
    DoubleRegReg spoilingScoreTuple(SuperRegionPtr superRegion) const;


    int m_availArmies;
    RegToIntMap m_regToArmiesBfr;

    RegIntList m_deployments;
    RegRegIntList m_attacks;

}; // class GreedyRoundStrategy

} // namespace warlightAi

#endif // GREEDY_ROUND_STRATEGY_H_INCLUDED
