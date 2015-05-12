// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef PICK_STRATEGY_H_INCLUDED
#define PICK_STRATEGY_H_INCLUDED

// Project
#include "globals.hpp"

namespace warlightAi {

/**
 * Base clase for strategies deciding the picks of our Bot.
 */
class PickStrategy
{
public:
    /**
     * Creates a new pick strategy given the initial starting regions the
     * game engine selects.
     */
    PickStrategy(const RegionPtrSet &startingRegions);

    virtual ~PickStrategy();

    /**
     * Virtual method which is to be implemented by the derived classes. This
     * should consist of the whole logic behind our Bot's picks. This is called
     * each time the engine gives the pick_starting_region command.
     */
    virtual RegionPtr pickNext(const RegionPtrSet &pickableRegions) const = 0;

protected:
    const RegionPtrSet &m_startingRegions;

}; // class PickStrategy

} // namespace warlightAi

#endif // PICK_STRATEGY_H_INCLUDED
