// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef QUICK_PICK_STRATEGY_H
#define QUICK_PICK_STRATEGY_H

// Project
#include "PickStrategy.h"


namespace warlightAi {

// Fwrd decls
class World;

class QuickPickStrategy : public PickStrategy
{
public:
    QuickPickStrategy(const VecOfRegionPtrs &startingRegions);

    RegionPtr pickNext(const VecOfRegionPtrs &pickableRegions) const override;

private:
    double superRegionScore(const SuperRegionPtr superReg) const;

}; // class QuickPickStrategy

} // namespace warlightAi

#endif // QUICK_PICK_STRATEGY_H
