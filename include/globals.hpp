// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <vector>
#include <tuple>
#include <memory>

namespace warlightAi {

// Consts
const int INIT_AVAILABLE_ARMIES= 5;
const int NEUTRAL_ARMY_COUNT = 2;
const int WASTELAND_ARMY_COUNT = 6;

// Enums
enum class Player {
    ME,
    OPPONENT,
    NEUTRAL,
    UNKNOWN
};

enum class Request {
    CHECK_STARTING_REGIONS,
    PICK_STARTING_REGION,
    PLACE_ARMIES,
    ATTACK_TRANSFER,
    CHECK_OPPONENT_MOVES,
    CHECK_OPPONENT_STARTING_REGIONS
};

// Aliases
class Region;
class SuperRegion;

using RegionPtr = std::shared_ptr<Region>;
using SuperRegionPtr = std::shared_ptr<SuperRegion>;
using VecOfRegionPtrs = std::vector<RegionPtr>;
using VecOfSuperRegionPtrs = std::vector<SuperRegionPtr>;
using VecOfPairs = std::vector<std::pair<RegionPtr, int>>;
using VecOfTuples = std::vector<std::tuple<RegionPtr, RegionPtr, int>>;

}

#endif // GLOBALS_H_INCLUDED
