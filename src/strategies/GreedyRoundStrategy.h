// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef GREEDY_ROUND_STRATEGY_H_INCLUDED
#define GREEDY_ROUND_STRATEGY_H_INCLUDED

// Project
#include "RoundStrategy.h"

// C++
#include <unordered_set>


namespace warlightAi {

class GreedyRoundStrategy : public RoundStrategy
{
public:
    GreedyRoundStrategy(const World &world, int availableArmies);

    VecOfPairs getDeployments() const override;
    VecOfTuples getAttacks() const override;

private:
    VecOfRegionPtrs getTargetedOppRegions();
    void computeMigrations();
    std::unordered_set<RegionPtr> getRegionsOnBorder();

    VecOfPairs m_deployments;
    VecOfTuples m_attacks;

}; // class GreedyRoundStrategy

} // namespace warlightAi

#endif // GREEDY_ROUND_STRATEGY_H_INCLUDED
