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

namespace warlightAi {

/**
 * The main round strategy of our Bot at this phase. This is based on a greedy
 * algorithm.
 */
class GreedyRoundStrategy : public RoundStrategy
{
    using RegRegPair = std::pair<RegionPtr, RegionPtr>;
    using VecOfRegReg = std::vector<RegRegPair>;
    using DoubleRegReg = std::tuple<double, RegionPtr, RegionPtr>;

public:
    /**
     * @see RoundStrategy::RoundStrategy
     */
    GreedyRoundStrategy(const World &world, int availableArmies);

    /**
     * Return the deployments and the attacks our Bot should do during this
     * round. As mentioned above, they are computed using a greedy algorithm
     * which tries to conquer all the regions from the best ranked super
     * region based on our ranking score.
     */
    VecOfRegInt getDeployments() const override;
    VecOfRegRegInt getAttacks() const override;

private:
    // TODO
     VecOfRegReg getSpoilableRegions() const;

    /**
     * Return a list of references to Regions which should be in our Bot's
     * target during this round.
     */
    RegionPtrList getHostileRegions() const;

    // TODO
    void handleSpoilingAttack(const RegRegPair &meToOp);

    // TODO
    void handleHostileAttack(RegionPtr reg);

    // TODO
    void handleRemainingArmies();

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

    // TODO
    double superRegionsScore(SuperRegionPtr superRegion) const;

    // TODO
    DoubleRegReg spoilingScoreTuple(SuperRegionPtr superRegion) const;


    int m_availArmies;
    VecOfRegInt m_deployments;
    VecOfRegRegInt m_attacks;

}; // class GreedyRoundStrategy

} // namespace warlightAi

#endif // GREEDY_ROUND_STRATEGY_H_INCLUDED
