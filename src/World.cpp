// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "World.hpp"

// Project
#include "SuperRegion.hpp"
#include "Region.hpp"

// C++
#include <exception>


namespace warlightAi {

void World::addRegion(int regionId, int superRegionId)
{
    auto regionSuper = getSuperRegionById(superRegionId);
    if (!regionSuper)
        throw std::runtime_error("Couldn't find region's superRegion");

    auto newReg = std::make_shared<Region>(
                        regionId,
                        regionSuper,
                        NEUTRAL_ARMY_COUNT,
                        Player::NEUTRAL
    );
    m_regions.emplace_back(newReg);
    regionSuper->addSubRegion(newReg);
}

void World::addSuperRegion(int superRegionId, int award)
{
    m_superRegions.emplace_back(
                std::make_shared<SuperRegion>(superRegionId, award));
}

void World::addLink(int region1, int region2)
{
    auto region1Ptr = getRegionById(region1);
    auto region2Ptr = getRegionById(region2);

    if (!region1Ptr || !region2Ptr)
        throw std::runtime_error("Couldn't find regions by id");

    region1Ptr->addNeighbor(region2Ptr);
    region2Ptr->addNeighbor(region1Ptr);
}

void World::addWasteland(int id)
{
    getRegionById(id)->setArmies(WASTELAND_ARMY_COUNT);
}

void World::updateRegion(int region, Player owner, int armies)
{
    auto targetReg = getRegionById(region);
    targetReg->setOwner(owner);
    targetReg->setArmies(armies);
}

auto World::getRegionsOwnedBy(warlightAi::Player player) const
    -> decltype(m_regions)
{
    decltype(m_regions) ownedRegions;

    for (auto &region : m_regions)
        if (region->getOwner() == player)
            ownedRegions.emplace_back(region);

    return ownedRegions;
}

RegionPtr World::getRegionById(int regionId) const
{
    for (auto &region : m_regions)
        if (region->id() == regionId)
            return region;

    return nullptr;
}

SuperRegionPtr World::getSuperRegionById(int superRegionId) const
{
    for (auto &super : m_superRegions)
        if (super->id() == superRegionId)
            return super;

    return nullptr;
}

} // namespace warlightAi
