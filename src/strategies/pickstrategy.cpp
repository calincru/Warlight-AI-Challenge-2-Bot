// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "pickstrategy.hpp"

namespace warlightAi {

PickStrategy::PickStrategy(const RegionPtrSet &startingRegions)
    : m_startingRegions(startingRegions)
{}

PickStrategy::~PickStrategy()
{
    // nothing to do
}

} // namespace warlightAi
