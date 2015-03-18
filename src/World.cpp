// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

// Self
#include "World.h"

// Project
#include "SuperRegion.h"
#include "Region.h"

// C++
#include <exception>


namespace warlightAi {

void World::addRegion(int regionId, int superRegionId)
{
    auto regionSuper = getSuperRegionById(superRegionId);
    if (!regionSuper)
        throw std::runtime_error("Couldn't find region's superRegion");

    m_regions.push_back(
                std::make_shared<Region>(regionId,
                                         regionSuper,
                                         warlightAi::neutralArmies,
                                         warlightAi::Player::NEUTRAL)
    );
}

void World::addSuperRegion(int superRegionId, int award)
{
    m_superRegions.push_back(
                std::make_shared<SuperRegion>(superRegionId, award));
}

void World::addLink(int region1, int region2)
{
    auto region1Ptr = getRegionById(region1);
    auto region2Ptr = getRegionById(region2);

    if (!region1Ptr || !region2Ptr)
        throw std::runtime_error("Couldn't find regions by id");

    region1Ptr->addNeighbor(region2Ptr);
    region1Ptr->addNeighbor(region1Ptr);
}

void World::addWasteland(int id)
{
    getRegionById(id)->setArmies(warlightAi::wastelandArmies);
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
            ownedRegions.push_back(region);

    return ownedRegions;
}

auto World::getRegionById(int regionId) const -> RegionPtr
{
    for (auto &region : m_regions)
        if (region->id() == regionId)
            return region;

    return nullptr;
}

auto World::getSuperRegionById(int superRegionId) const -> SuperRegionPtr
{
    for (auto &super : m_superRegions)
        if (super->id() == superRegionId)
            return super;

    return nullptr;
}

} // namespace warlightAi
