// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef BASIC_SCORE_H_INCLUDED
#define BASIC_SCORE_H_INCLUDED

// Project
#include "region.hpp"
#include "superregion.hpp"
#include "statistics.hpp"

namespace warlightAi { namespace common {

class BasicScore
{
public:
    static double simulationScore(SuperRegionPtr superRegion,
                                  int availableArmies)
    {
        auto subRegs = superRegion->getSubRegions();
        double sum = -subRegs.size();

        for (auto &reg : subRegs)
            if (reg->getOwner() == Player::ME)
                sum += reg->getArmies();
            else
                sum -= Statistics::armiesNeeded(reg->getArmies());

        if (sum > 0)
            sum = 0.;

        return (10 - (-sum)/availableArmies) * superRegion->getReward();
    }

}; // namespace BasicScore

} // namespace common
} // namespace warlightAi

#endif // BASIC_SCORE_H_INCLUDED
