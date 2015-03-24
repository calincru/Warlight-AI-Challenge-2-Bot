// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef ROUND_STRATEGY_H_INCLUDED
#define ROUND_STRATEGY_H_INCLUDED


// Project
#include "globals.h"


namespace warlightAi {

// Fwrd decls
class World;

/**
 * Base class for strategies deciding the deployments and attacks of our Bot.
 */
class RoundStrategy
{
public:
    /**
     * Creates a new round strategy given a reference to the map of the game
     * (the World parameter) and the number of armies available this round.
     */
    RoundStrategy(const World &world, int availableArmies);
    virtual ~RoundStrategy();

    /**
     * Pure virtual methods returning the deployments and the attacks our Bot
     * should do this round.
     */
    virtual VecOfPairs getDeployments() const = 0;
    virtual VecOfTuples getAttacks() const = 0;

protected:
    const World &m_world;
    int m_availableArmies;

}; // class RoundStrategy

} // namespace warlightAi


#endif // ROUND_STRATEGY_H_INCLUDED
