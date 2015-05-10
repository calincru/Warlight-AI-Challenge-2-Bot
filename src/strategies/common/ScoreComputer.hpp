// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef SCORE_COMPUTER_H_INCLUDED
#define SCORE_COMPUTER_H_INCLUDED

// Project
#include "utils.hpp"
#include "globals.hpp"
#include "Region.hpp"
#include "SuperRegion.hpp"


namespace warlightAi { namespace common {

/**
 * Class containing static functions used by both picking and round strategies.
 */
class ScoreComputer
{
public:
    /**
     * Older function used for ranking super regions.
     */
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

    /**
     * Function used to determine the number of armies our Bot has to attack
     * the opponent, to conquer the given region with the given probability.
     */
    static int armiesNeeded(int oppArmies, double probability)
    {
        UNUSED(probability);
        if (oppArmies == 1)
            return 2.;
        if (oppArmies == 2)
            return 3.;

        return 3./2.*oppArmies + 1;
    }

    /**
     * Our latest super region ranking function. Inhouse built formula :-).
     */
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

        /* Before */
        /* auto threshold = (superRegion->getReward() * 1.)/(std::min(1, oppCount - 1)) * */
        /*                  (oppSum * 1.)/oppCount; */
        /* return score + threshold; */
        /* After */
        return score / (minesCount + oppCount);
    }

    // Others
};

} // namespace common
} // namespace warlightAi

#endif // SCORE_COMPUTER_H_INCLUDED
