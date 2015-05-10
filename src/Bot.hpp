// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//



#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

// Project
#include "boost/noncopyable.hpp"
#include "globals.hpp"
#include "World.hpp"
#include "PickStrategy.hpp"
#include "RoundStrategy.hpp"

// C++
#include <vector>
#include <string>


namespace warlightAi {

/**
 * This class is meant to be instantiated only once and holds the interaction
 * with the parser and the logic behind our Bot's play.
 */
class Bot : private boost::noncopyable
{
    using PickStrategyPtr = std::unique_ptr<PickStrategy>;
    using RoundStrategyPtr = std::unique_ptr<RoundStrategy>;

public:
    /**
     * Creates and initializes a new and unique Bot.
     */
    Bot();

    /**
     * Tells the Bot to start playing.
     */
    void play();

    /**
     * This method is used in the interaction with the parser. Sometimes the
     * parser needs to tell the Bot to handle some situations after forwarding
     * all information to it; e.g.: picking phase, attack/transfer phase,
     * dealing with opponents' moves, etc.
     */
    void handleRequest(Request request);

    // The following methods are entirely used in the communication with the
    // Parser instance.
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
    void setAvailableArmies(int availableArmies);
    void setTimebank(int timebank);
    void setTimePerMove(int timePerMove);
    void setMaxRounds(int maxRounds);

    void startDelay(int delay);
    void updateRegion(int region, const std::string &playerName, int armiesCnt);

private:
    // The following methods are called from the handleRequest method and are
    // dealing with specific requests from the Parser.
    /**
     * This method is called in the picking phase. It forwards the picking
     * decision to the PickStrategy instance the bot holds.
     */
    void pick();

    /**
     * This method is called in the deployment phase. This forwards the
     * deployment decision to the RoundStrategy instance the bot holds.
     */
    void deploy();

    /**
     * This method is called in the attacks phase. This forwards the deployment
     * decision to the RoundStrategy instance the bot holds.
     */
    void attack();

    /**
     * This method is called after the parser has already forwarded the initial
     * starting regions the two bots will have to pick from.
     */
    void checkStartingRegions();

    /**
     * This method is called after the parser has already forwarded the
     * opponents starting regions to our Bot.
     */
    void checkOpponentStartingRegions();

    /**
     * This method is called after each round, after the parser has already
     * forwarded the opponent's movements from the last round.
     */
    void checkOpponentMoves();


    World m_world;

    std::string m_name;
    std::string m_oppName;

    int m_timebank;
    int m_timePerMove;
    int m_maxRounds;
    int m_availableArmies;

    VecOfRegionPtrs m_pickableRegions;
    VecOfRegionPtrs m_startingRegions;
    VecOfRegionPtrs m_opponentStartingRegions;

    VecOfTuples m_opponentAttacks;
    VecOfPairs m_opponentDeployments;

    PickStrategyPtr m_pickStrategy;
    RoundStrategyPtr m_roundStrategy;

}; // class Bot

} // namespace warlightAi

#endif // BOT_H_INCLUDED
