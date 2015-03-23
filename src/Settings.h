// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <vector>
#include <tuple>
#include <queue>

/**
 * This file contains general configuration and things which should be in the
 * global scope
 */

using AdjencyList = std::vector<std::vector<int>>;
using Placements = std::vector<std::pair<int, int>>;
using Movements = std::vector<std::tuple<int, int, int>>;
using ScoreQueue = std::priority_queue<std::pair<int, int>>;

#define NEUTRAL_ARMIES 2
#define WASTELAND_ARMIES 6

enum class Player {
    ME,
    ENEMY,
    NEUTRAL
};

enum class Request {
    NONE,
    PICK_STARTING_REGION,
    PLACE_ARMIES,
    ATTACK_TRANSFER
};

#endif // SETTINGS_H_INCLUDED
