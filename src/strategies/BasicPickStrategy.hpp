// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef BASIC_PICK_STRATEGY_H_INCLUDED
#define BASIC_PICK_STRATEGY_H_INCLUDED

// Project
#include "PickStrategy.hpp"
#include "globals.hpp"


namespace warlightAi {

class BasicPickStrategy : public PickStrategy
{
public:
    BasicPickStrategy(const VecOfRegionPtrs &startingRegions);
    virtual ~BasicPickStrategy();

    RegionPtr pickNext(const VecOfRegionPtrs &pickableRegions) const override;

private:

}; // class BasicPickStrategy

} // namespace warlightAi


#endif // BASIC_PICK_STRATEGY_H_INCLUDED
