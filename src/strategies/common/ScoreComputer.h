// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef SCORE_COMPUTER_H_INCLUDED
#define SCORE_COMPUTER_H_INCLUDED

// Project
#include "utils.h"
#include "globals.h"
#include "Region.h"
#include "SuperRegion.h"


namespace warlightAi { namespace common {

class ScoreComputer
{
public:
    // Basic brain impl score
    static double simulationScore(const SuperRegionPtr &superRegion,
                                  int availableArmies)
    {
        auto subRegs = superRegion->getSubRegions();
        double sum = -subRegs.size();

        for (auto &reg : subRegs)
            if (reg->getOwner() == Player::ME)
                sum += reg->getArmies();
            else
                sum -= armiesNeeded(reg->getArmies(), 0.7);

        if (sum > 0)
            sum = 0.;

        return (10 - (-sum)/availableArmies) * superRegion->getReward();
    }

    static int armiesNeeded(int oppArmies, double probability)
    {
        UNUSED(probability);
        if (oppArmies == 1)
            return 2.;
        if (oppArmies == 2)
            return 3.;

        return 3./2.*oppArmies + 1;
    }

    static double wastelandsBasedScore(const SuperRegionPtr &superRegion)
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

        auto threshold = (superRegion->getReward() * 1.)/(std::min(1, oppCount - 1)) *
                         (oppSum * 1.)/oppCount;
        return score + threshold;
    }

    // Others
};

} // namespace common
} // namespace warlightAi

#endif // SCORE_COMPUTER_H_INCLUDED
