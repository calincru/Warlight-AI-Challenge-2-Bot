// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <vector>
#include <tuple>

/**
 * This file contains gereral configuration and thing whoch should be in the
 * global scope
 */

using AdjencyList = std::vector<std::vector<int>>;
using Placements = std::vector<std::pair<int, int>>;
using Movements = std::vector<std::tuple<int, int, int>>;

#define NEUTRAL_ARMIES 2
#define WASTELAND_ARMIES 10

enum class Player {
    ME,
    ENEMY,
    NEUTRAL
};

enum class State {
    NONE,
    PICK_STARTING_REGION,
    PLACE_ARMIES,
    ATTACK_TRANSFER
};

#endif // MAIN_H_INCLUDED
