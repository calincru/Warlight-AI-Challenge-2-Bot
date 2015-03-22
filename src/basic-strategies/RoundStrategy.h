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

class RoundStrategy
{
public:
    RoundStrategy(const World &world, int availableArmies);
    virtual ~RoundStrategy();

    virtual VecOfPairs getDeployments() const = 0;
    virtual VecOfTuples getAttacks() const = 0;

protected:
    const World &m_world;
    int m_availableArmies;

}; // class RoundStrategy

} // namespace warlightAi


#endif // ROUND_STRATEGY_H_INCLUDED
