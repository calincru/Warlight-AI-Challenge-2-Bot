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

    /**
     * Use
     *
     *      decltype(auto) subRegs = supraRegion->getSubRegions();
     *
     *      or
     *
     *      const auto &subRegs = supraRegion->getSubRegions();
     *
     *  to get the constant reference to the internal neighbors vector.
     *  If one does
     *
     *      auto subRegs = supraRegion->getSubRegions();
     *
     *  a copy of the vector will be made.
     */
    const auto &getSubRegions() const
    {
        return m_subRegions;
    }

    bool isOwnedBy(warlightAi::Player player) const;

    int getReward() const;

private:
    int m_id;
    int m_reward;
    std::vector<const RegionPtr> m_subRegions;

}; // class SuperRegion


} // namespace warlightAi


#endif // SUPERREGION_H_INCLUDED
