// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

// C++
#include <vector>
#include <string>
#include <queue>

// Project
#include "boost/noncopyable.hpp"
#include "consts.h"
#include "World.h"


namespace warlightAi {

class Bot : private boost::noncopyable
{
    typedef World::RegionPtr RegionPtr;
    typedef World::SuperRegionPtr SuperRegionPtr;

public:
    using VecOfPairs = std::vector<std::pair<int, int>>;
    using VecOfTuples = std::vector<std::tuple<int, int, int>>;


    Bot() = default;

    /**
     * Plays a single game of Warlight
     */
    void play();

    /**
     * Handles requests from the game
     */
    void handleRequest(Request request);
    void pickStartingRegion();

    /**
     * Returns a (source, destination) pair for an attack
     */
    std::pair<RegionPtr, RegionPtr> planMoves();
    int computeScore(SuperRegionPtr superRegion);

    void placeArmies();
    void makeMoves();
    void handleOppMoves(const VecOfPairs& pls, const VecOfTuples& movs);

    /**
     * INITIAL starting positions!!
     * Gets the indexes of the starting regions and does some computation based
     * on them. E.g. computes the `scores` for each one of them to easily make a
     * choice in the pick starting regions phase.
     */
    void handleInitialStartingRegions(const std::vector<int> &regions);

    /**
     * Gets the indexes of the opponents starting regions and does some
     * computation based on them.
     */
    void handleOppStartingRegions(const std::vector<int>& region);

    /// Interface for settings
    void addRegion(int region, int super);
    void addSuperRegion(int super, int reward);
    void addNeighbor(int region, int neigh);
    void addWasteland(int region);

    void setName(const std::string& name);
    void setOppName(const std::string& oppName);
    void setAvailArmies(int armies);
    void setTimebank(int timebank);
    void setTimePerMove(int time);
    void setMaxRounds(int rounds);

    /**
     * Sets the starting regions that are currently possible.
     */
    void setPossibleStartingRegions(const std::vector<int>& region);

    void startDelay(int delay);

    /**
     * Updates the regions from the game engine
     * @param region region identifier
     * @param name player who owns it
     * @param armies number of armies he gets
     */
    void updateRegion(int region, const std::string& name, int armies);
    void resetOwnedRegions();

private:
    World m_world;

    std::string name;
    std::string oppName;

    int availArmies;

    int timebank;
    int timePerMove;
    int maxRounds;

    std::vector<int> possibleStartingRegions;
    std::vector<int> ownedRegions;

}; // class Bot

} // namespace warlightAi

#endif // BOT_H_INCLUDED
