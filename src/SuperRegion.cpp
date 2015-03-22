// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// Self
#include "SuperRegion.h"

// Project
#include "Region.h"


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
        if (reg == region)
            return true;

    return false;
}

auto SuperRegion::getSubRegions() const -> decltype((m_subRegions))
{
    return m_subRegions;
}

bool SuperRegion::isOwnedBy(warlightAi::Player player) const
{
    for (auto &reg : m_subRegions)
        if (reg->getOwner() != player)
            return false;

    return true;
}

int SuperRegion::getReward() const
{
    return m_reward;
}

} // namespace warlightAi
