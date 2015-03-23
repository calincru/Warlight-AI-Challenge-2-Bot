// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef SUPERREGION_H_INCLUDED
#define SUPERREGION_H_INCLUDED

// Project
#include "globals.h"

// C++
#include <memory>


namespace warlightAi {

// Fwrd decls
class Region;

class SuperRegion
{
    using RegionWkPtr = std::weak_ptr<Region>;

public:
    SuperRegion(int id, int reward);

    int id() const;

    void addSubRegion(RegionPtr subRegion);
    bool containsRegion(RegionPtr region) const;
    std::vector<RegionPtr> getSubRegions() const;

    std::vector<RegionPtr> getSurroundingRegions() const;

    bool isOwnedBy(warlightAi::Player player) const;

    int getReward() const;

private:
    int m_id;
    int m_reward;
    std::vector<RegionWkPtr> m_subRegions;

}; // class SuperRegion

} // namespace warlightAi

#endif // SUPERREGION_H_INCLUDED
