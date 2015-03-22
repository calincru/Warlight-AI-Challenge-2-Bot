// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef PICK_STRATEGY_H_INCLUDED
#define PICK_STRATEGY_H_INCLUDED


// Project
#include "globals.h"


namespace warlightAi {

class PickStrategy
{
public:
    PickStrategy(const VecOfRegionPtrs &startingRegions);
    virtual ~PickStrategy();

    virtual RegionPtr pickNext(const VecOfRegionPtrs &pickableRegions) const = 0;

protected:
    const VecOfRegionPtrs &m_startingRegions;

}; // class PickStrategy

} // namespace warlightAi


#endif // PICK_STRATEGY_H_INCLUDED
