// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef STATISTICS_H_INCLUDED
#define STATISTICS_H_INCLUDED

// Project
#include "utils.hpp"
#include "globals.hpp"
#include "region.hpp"
#include "superregion.hpp"

// C++
#include <cmath>

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
    static int armiesNeeded(int d)
    {
        if (d <= 1)
            return 2;
        return std::ceil((d - 0.5) * 5 / 3);
    }

    static int revArmiesNeeded(int armies)
    {
        return round(3./5 * armies + 0.001);
    }
};

} // namespace common
} // namespace warlightAi

#endif // STATISTICS_H_INCLUDED
