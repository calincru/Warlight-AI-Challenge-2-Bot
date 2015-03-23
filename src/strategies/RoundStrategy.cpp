// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "RoundStrategy.h"

// Project
#include "World.h"


namespace warlightAi {

RoundStrategy::RoundStrategy(const World &world, int availableArmies)
    : m_world(world)
    , m_availableArmies(availableArmies)
{
}

RoundStrategy::~RoundStrategy()
{
    // Nothing to do
}

} // namespace warlightAi
