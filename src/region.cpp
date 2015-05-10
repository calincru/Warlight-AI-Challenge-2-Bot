// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// Self
#include "region.hpp"

// C++
#include <cassert>


namespace warlightAi {

Region::Region(int id, SuperRegionPtr superRegion)
    : m_id(id)
    , m_superRegion(superRegion)
    , m_armies(NEUTRAL_ARMY_COUNT)
    , m_owner(Player::NEUTRAL)
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
        neighs.emplace_back(neigh.lock());

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

SuperRegionPtr Region::getSuperRegion() const
{
    return m_superRegion.lock();
}

} // namespace warlightAi
