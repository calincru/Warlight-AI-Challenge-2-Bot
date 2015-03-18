// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef SUPERREGION_H_INCLUDED
#define SUPERREGION_H_INCLUDED

// Project
#include "consts.h"

// C++
#include <memory>


namespace warlightAi {

// Fwrd decls
class Region;

class SuperRegion
{
    using RegionPtr = std::shared_ptr<Region>;
    using SuperRegionPtr = std::shared_ptr<SuperRegion>;

public:
    SuperRegion(int id, int reward);

    int id() const;

    void addSubRegion(RegionPtr subRegion);
    bool containsRegion(RegionPtr region) const;
    const std::vector<RegionPtr> &getSubRegions() const;

    bool isOwnedBy(warlightAi::Player player) const;

    int getReward() const;

private:
    int m_id;
    int m_reward;
    std::vector<RegionPtr> m_subRegions;

}; // class SuperRegion


} // namespace warlightAi


#endif // SUPERREGION_H_INCLUDED
