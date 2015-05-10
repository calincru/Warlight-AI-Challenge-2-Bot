// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef GREEDY_ROUND_STRATEGY_H_INCLUDED
#define GREEDY_ROUND_STRATEGY_H_INCLUDED

// Project
#include "RoundStrategy.hpp"

// C++
#include <unordered_set>


namespace warlightAi {

/**
 * The main round strategy of our Bot at this phase. This is based on a greedy
 * algorithm.
 */
class GreedyRoundStrategy : public RoundStrategy
{
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
    VecOfPairs getDeployments() const override;
    VecOfTuples getAttacks() const override;

private:
    /**
     * Return a list of references to Regions which should be in our Bot's
     * target during this round.
     */
    VecOfRegionPtrs getTargetedOppRegions();

    /**
     * After computing the attacks, this strategy is trying to get all our
     * armies from regions which do not have any enemy neighbor in the
     * direction where there are enemy armies.
     */
    void computeMigrations();

    /**
     * Helper function, used by the above method, which returns a set
     * containing the regions our Bot owns and have at least one enemy
     * neighbor.
     */
    std::unordered_set<RegionPtr> getRegionsOnBorder();

    VecOfPairs m_deployments;
    VecOfTuples m_attacks;

}; // class GreedyRoundStrategy

} // namespace warlightAi

#endif // GREEDY_ROUND_STRATEGY_H_INCLUDED
