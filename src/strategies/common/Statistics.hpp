// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef STATISTICS_H_INCLUDED
#define STATISTICS_H_INCLUDED

// Project
#include "utils.hpp"
#include "globals.hpp"
#include "Region.hpp"
#include "SuperRegion.hpp"


namespace warlightAi { namespace common {

/**
 * Class containing static functions used by both picking and round strategies.
 */
class Statistics
{
public:
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
};

} // namespace common
} // namespace warlightAi

#endif // STATISTICS_H_INCLUDED
