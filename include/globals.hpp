// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <vector>
#include <unordered_set>
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

using RegionPtrList = std::vector<RegionPtr>;
using SuperRegionPtrList = std::vector<SuperRegionPtr>;

using RegionPtrSet = std::unordered_set<RegionPtr>;
using SuperRegionPtrSet = std::unordered_set<SuperRegionPtr>;

using RegIntPair = std::pair<RegionPtr, int>;
using RegIntList = std::vector<RegIntPair>;

using RegRegIntTuple = std::tuple<RegionPtr, RegionPtr, int>;
using RegRegIntList = std::vector<RegRegIntTuple>;

struct RegIntEq;
struct RegIntHash;
using RegIntSet = std::unordered_set<
                            std::pair<RegionPtr, int>,
                            RegIntHash,
                            RegIntEq
                  >;

struct RegRegIntEq;
struct RegRegIntHash;
using RegRegIntSet = std::unordered_set<
                            RegRegIntTuple,
                            RegRegIntHash,
                            RegRegIntEq
                     >;
struct RegIntEq
{
    bool operator()(const RegIntPair &lhs, const RegIntPair &rhs) const
    {
        return !lhs.first.owner_before(rhs.first)
                && !rhs.first.owner_before(lhs.first)
                && lhs.second == rhs.second;
    }
};

struct RegIntHash
{
    std::size_t operator()(const RegIntPair &e) const
    {
        return std::hash<RegionPtr>()(e.first) * 141 + e.second * 17;
    }
};

struct RegRegIntEq
{
    bool operator()(const RegRegIntTuple &lhs, const RegRegIntTuple &rhs) const
    {
        const auto &lfst = std::get<0>(lhs);
        const auto &lsnd = std::get<1>(lhs);
        const auto &ltrd = std::get<2>(lhs);
        const auto &rfst = std::get<0>(rhs);
        const auto &rsnd = std::get<1>(rhs);
        const auto &rtrd = std::get<2>(rhs);

        return !lfst.owner_before(rfst) && !rfst.owner_before(lfst)
                && !lsnd.owner_before(rsnd) && !rsnd.owner_before(lsnd)
                && ltrd == rtrd;
    }
};

struct RegRegIntHash
{
    std::size_t operator()(const RegRegIntTuple &e) const
    {
        const auto &fst = std::get<0>(e);
        const auto &snd = std::get<1>(e);
        const auto &trd = std::get<2>(e);

        return std::hash<RegionPtr>()(fst) * 149
                + std::hash<RegionPtr>()(snd) * 71
                + trd * 37;
    }
};

} // namespace warlightAi

#endif // GLOBALS_H_INCLUDED
