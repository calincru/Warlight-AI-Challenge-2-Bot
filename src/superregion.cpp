// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "superregion.hpp"

// Project
#include "region.hpp"

namespace warlightAi {

SuperRegion::SuperRegion(int id, int reward)
    : m_id(id)
    , m_reward(reward)
{}

int SuperRegion::id() const
{
    return m_id;
}

void SuperRegion::addSubRegion(RegionPtr subRegion)
{
    if (!containsRegion(subRegion))
        m_subRegions.emplace_back(subRegion);
}

bool SuperRegion::containsRegion(RegionPtr region) const
{
    for (auto &reg : m_subRegions)
        if (reg.lock() == region)
            return true;

    return false;
}

std::vector<RegionPtr> SuperRegion::getSubRegions() const
{
    std::vector<RegionPtr> subRegs;

    for (auto &subReg : m_subRegions)
        subRegs.emplace_back(subReg);

    return subRegs;
}

std::vector<RegionPtr> SuperRegion::getSurroundingRegions() const
{
    std::vector<RegionPtr> retRegs;

    for (auto &subReg : m_subRegions)
        for (auto &subRegNeigh : subReg.lock()->getNeighbors())
            if (subRegNeigh->getSuperRegion().get() != this)
                retRegs.emplace_back(subRegNeigh);

    return retRegs;
}

bool SuperRegion::isOwnedBy(warlightAi::Player player) const
{
    for (auto &reg : m_subRegions)
        if (reg.lock()->getOwner() != player)
            return false;

    return true;
}

int SuperRegion::getReward() const
{
    return m_reward;
}

} // namespace warlightAi
