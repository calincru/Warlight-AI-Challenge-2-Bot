// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

// C++
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

// Project
#include "main.h"
#include "Parser.h"
#include "Region.h"
#include "SuperRegion.h"



class Bot : private boost::noncopyable
{
public:
    enum Phase {
        NONE,
        PICK_STARTING_REGION,
        PLACE_ARMIES,
        ATTACK_TRANSFER
    };

    Bot();
    virtual ~Bot();

    void playGame();    ///< plays a single game of Warlight

    void pickStartingRegion();
    void placeArmies();
    void makeMoves();   ///< makes moves for a single turn

    void addRegion(unsigned noRegion, unsigned noSuperRegion);
    void addSuperRegion(unsigned noSuperRegion, int reward);
    void addNeighbors(unsigned noRegion, unsigned Neighbors);
    void addWasteland(unsigned noRegion);

    /// Setters for settings
    void setBotName(const std::string& name);
    void setOpponentBotName(const std::string& name);
    void setArmiesLeft(int nbArmies);
    void setTimebank(int newTimebank);
    void setTimePerMove(int newTimePerMove);
    void setMaxRounds(int newMaxRounds);

    /**
     * Adds armies to a region
     * @param noRegion region to add to
     * @param nbArmies number of Armies
     */
    void addArmies(unsigned noRegion, int nbArmies);
    /**
     * Moves armies on the map
     * @param noRegion starting region
     * @param toRegion target region
     * @param nbArmies number of Armies
     */
    void moveArmies(unsigned noRegion, unsigned toRegion,
                    int nbArmies);

    void clearStartingRegions();
    void addStartingRegion(unsigned noRegion);

    void addOpponentStartingRegion(unsigned noRegion);

    void opponentPlacement(unsigned noRegion, int nbArmies);
    void opponentMovement(unsigned noRegion, unsigned toRegion,
                          int nbArmies);

    void startDelay(int delay);

    void setPhase(const Phase phase);

    /**
     * Evaluates the current phase and in turn figures out the proper action for
     * the current move Hook in your army placements and attack moves here
     */
    void executeAction();
    /**
     * Updates the regions from the game engine
     * @param noRegion region identifier
     * @param playerName player who owns it
     * @param nbArmies number of armies he gets
     */
    void updateRegion(unsigned noRegion, const std::string& playerName,
                      int nbArmies);

    void resetRegionsOwned();

private:
    std::ifstream in;
    std::vector<Region> regions;
    std::vector<SuperRegion> superRegions;
    std::string botName;
    std::string opponentBotName;
    std::vector<int> startingRegionsreceived;
    std::vector<unsigned> opponentStartingRegions;
    std::vector<int> ownedRegions;
    std::vector<int> wastelands;
    int armiesLeft;
    int timebank;
    int timePerMove;
    int maxRounds;
    Parser parser;
    Phase phase;
};

#endif // BOT_H_INCLUDED
