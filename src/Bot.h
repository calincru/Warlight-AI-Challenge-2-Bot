// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

// Project
#include "boost/noncopyable.hpp"
#include "globals.h"
#include "World.h"
#include "PickStrategy.h"

// C++
#include <vector>
#include <string>


namespace warlightAi {


class Bot : private boost::noncopyable
{
    using PickStrategyPtr = std::unique_ptr<PickStrategy>;

public:
    void play();
    void handleRequest(Request request);

    void addRegion(int newRegion, int superOfRegion);
    void addSuperRegion(int superRegion, int superRegionReward);
    void addNeighbor(int region, int regionNeigh);
    void addWasteland(int targetRegion);
    void addStartingRegion(int startingRegion);
    void addPickableRegion(int pickableRegion);
    void addOpponentAttack(int fromRegion, int toRegion, int armiesCount);
    void addOpponentDeployment(int destRegion, int armiesCount);
    void addOpponentStartingRegion(int startingRegion);

    void setName(const std::string &name);
    void setOppName(const std::string &oppName);
    void setAvailArmies(int availableArmies);
    void setTimebank(int timebank);
    void setTimePerMove(int timePerMove);
    void setMaxRounds(int maxRounds);

    void startDelay(int delay);
    void updateRegion(int region, const std::string &playerName, int armiesCnt);

private:
    void pick();
    void deploy();
    void attack();
    void checkStartingRegions();
    void checkOpponentStartingRegions();
    void checkOpponentMoves();

    std::pair<RegionPtr, RegionPtr> planMoves();
    int computeScore(SuperRegionPtr superRegion);


    World m_world;

    std::string m_name;
    std::string m_oppName;

    int m_availableArmies;

    int m_timebank;
    int m_timePerMove;
    int m_maxRounds;

    VecOfRegionPtrs m_pickableRegions;
    VecOfRegionPtrs m_startingRegions;

    VecOfRegionPtrs m_opponentStartingRegions;
    VecOfTuples m_opponentAttacks;
    VecOfPairs m_opponentDeployments;

    PickStrategyPtr m_pickStrategy;

}; // class Bot

} // namespace warlightAi

#endif // BOT_H_INCLUDED
