// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "PickStrategy.h"

namespace warlightAi {

PickStrategy::PickStrategy(const VecOfRegionPtrs &startingRegions)
    : m_startingRegions(startingRegions)
{}

PickStrategy::~PickStrategy()
{
    // nothing to do
}

} // namespace warlightAi
