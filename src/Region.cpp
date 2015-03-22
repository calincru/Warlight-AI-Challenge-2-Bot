// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// Self
#include "Region.h"

// C++
#include <cassert>


namespace warlightAi {

Region::Region(int id, SuperRegionPtr superRegion, int armies,
               warlightAi::Player player)
    : m_id(id)
    , m_superRegion(superRegion)
    , m_armies(armies)
    , m_owner(player)
{}

int Region::id() const
{
    return m_id;
}

void Region::addNeighbor(RegionPtr neighbor)
{
    if (!isNeighbor(neighbor))
        m_neighbors.emplace_back(neighbor);
}

bool Region::isNeighbor(RegionPtr region) const
{
    for (auto &neigh : m_neighbors)
        if (neigh.lock() == region)
            return true;

    return false;
}

std::vector<RegionPtr> Region::getNeighbors() const
{
    std::vector<RegionPtr> neighs;
    for (auto &neigh : m_neighbors)
        neighs.push_back(neigh.lock());

    return neighs;
}

void Region::setOwner(warlightAi::Player player)
{
    m_owner = player;
}

bool Region::isOwnedBy(warlightAi::Player player) const
{
    return m_owner == player;
}

warlightAi::Player Region::getOwner() const
{
    return m_owner;
}

void Region::setArmies(int armies)
{
    m_armies = armies;
}

int Region::getArmies() const
{
    return m_armies;
}

auto Region::getSuperRegion() const -> SuperRegionPtr
{
    return m_superRegion.lock();
}

} // namespace warlightAi
