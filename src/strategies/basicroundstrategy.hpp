// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef BASIC_ROUND_STRATEGY_H_INCLUDED
#define BASIC_ROUND_STRATEGY_H_INCLUDED


// Project
#include "globals.hpp"
#include "roundstrategy.hpp"


namespace warlightAi {

// Fwrd decls
class World;

class BasicRoundStrategy : public RoundStrategy
{
public:
    BasicRoundStrategy(const World &world, int availableArmies);

    VecOfRegInt getDeployments() const override;
    VecOfRegRegInt getAttacks() const override;

private:
    VecOfRegInt m_deployments;
    VecOfRegRegInt m_attacks;

}; // class BasicRoundStrategy

} // namespace warlightAi


#endif // BASIC_ROUND_STRATEGY_H_INCLUDED
