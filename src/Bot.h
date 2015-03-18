// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

// Project
#include "boost/noncopyable.hpp"
#include "consts.h"
#include "World.h"

// C++
#include <vector>
#include <string>


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
    void handleOppStartingRegions(const std::vector<int>& regions);

    /// Interface for settings
    void addRegion(int region, int super);
    void addSuperRegion(int super, int reward);
    void addNeighbor(int region, int neigh);
    void addWasteland(int region);

    void setName(const std::string& name);
    void setOppName(const std::string& oppName);
    void setAvailArmies(int availableArmies);
    void setTimebank(int timebank);
    void setTimePerMove(int timePerMove);
    void setMaxRounds(int maxRounds);

    /**
     * Sets the starting regions that are currently possible.
     */
    void setPossibleStartingRegions(const std::vector<int> &regions);

    void startDelay(int delay);

    /**
     * Updates the regions from the game engine
     * @param region region identifier
     * @param name player who owns it
     * @param armies number of armies he gets
     */
    void updateRegion(int region, const std::string &name, int armies);

private:
    World m_world;

    std::string m_name;
    std::string m_oppName;

    int m_availableArmies;

    int m_timebank;
    int m_timePerMove;
    int m_maxRounds;

    std::vector<int> m_possibleStartingRegions;

}; // class Bot

} // namespace warlightAi

#endif // BOT_H_INCLUDED
