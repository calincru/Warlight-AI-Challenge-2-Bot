// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// Self
#include "Bot.h"

// Project
#include "Parser.h"
#include "StringManipulation.h"
#include "utils.h"
#include "consts.h"
#include "Region.h"
#include "SuperRegion.h"

// C++
#include <iostream>
#include <sstream>
#include <cmath>
#include <cassert>
#include <algorithm>



namespace warlightAi {

void Bot::play()
{
    Parser(*this).parseInput();
}

void Bot::handleRequest(Request request)
{
    if (request == Request::PICK_STARTING_REGION)
        pickStartingRegion();
    else if (request == Request::PLACE_ARMIES)
        placeArmies();
    else if (request == Request::ATTACK_TRANSFER)
        makeMoves();
    else
        throw std::invalid_argument("Unknown request");
}

int Bot::computeScore(SuperRegionPtr superRegion)
{
    auto subRegs = superRegion->getSubRegions();
    auto sum = -(static_cast<int>(subRegs.size()));

    for (auto &reg : subRegs)
        if (reg->getOwner() == Player::ME)
            sum += reg->getArmies();
        else
            sum -= 3./2. * reg->getArmies();

    if (sum > 0)
        sum = 0;

    return (10 - (-sum)/availArmies) * superRegion->getReward();
}

auto Bot::planMoves() -> std::pair<RegionPtr, RegionPtr>
{
    // TODO
    //
    // Because deployments are dependent on planned attacks/transfers, there
    // should be a function that decides all the actions for the second phase
    // and deploys accordingly. This function should not return a pair, but
    // rather compute all possible pairs needed for the attack/transfer phase
    // and call the "placeArmies" and "makeMoves" functions (which should take
    // as parameters either a list of pairs or one pair at a time.

    std::vector<std::pair<RegionPtr, RegionPtr>> myRegOtherRegPairs;

    // Finds all the adjacent super regions and keep all the (src, dest) pairs
    // for possible attacks
    for (auto &myReg : m_world.getRegionsOwnedBy(Player::ME))
        for (auto &otherReg : myReg->getNeighbors())
            if (otherReg->getOwner() != Player::ME)
                myRegOtherRegPairs.emplace_back(myReg, otherReg);

    std::pair<RegionPtr, RegionPtr> maxPair;
    auto maxScore = std::numeric_limits<int>::min();
    for (auto &p : myRegOtherRegPairs) {
        auto score = computeScore(p.second->getSuperRegion());
        if (score > maxScore) {
            maxScore = score;
            maxPair = p;
        }
    }

    return maxPair;
}

void Bot::pickStartingRegion()
{
    // There is no need for a priority queue, since the score of the starting
    // regions might vary based on the previous choices of either botm, so this
    // function just picks the region with the maximum score.

    auto maxReg = -1;
    auto maxScore = -1;
    for (auto &reg : possibleStartingRegions) {
        auto score = computeScore(m_world.getRegionById(reg)->getSuperRegion());
        if (score > maxScore) {
            maxScore = score;
            maxReg = reg;
        }
    }

    std::cout << maxReg << std::endl;
}

void Bot::placeArmies()
{
    auto p = planMoves();
    std::cout << name << " placeArmies " << p.first->id()  << " " << availArmies
              << std::endl;

    p.first->setArmies(p.first->getArmies() + availArmies);
}

void Bot::makeMoves()
{
    /// Output No moves when you have no time left or do not want to commit any
    /// moves.
    //
    // std::cout << "No moves "  << std::endl;
    //
    /// Anatomy of a single move
    //
    //  std::cout << name << " attack/transfer " << from << " " << to << " "
    //            << armiesMoved;
    //
    /// When outputting multiple moves they must be seperated by a comma
    //

    auto p = planMoves();
    std::cout << name << " attack/transfer " << p.first->id() << " "
              << p.second->id() << " " << p.first->getArmies() - 1 << std::endl;
}

void Bot::handleOppMoves(const VecOfPairs& pls, const VecOfTuples& movs)
{
    // TODO
    UNUSED(pls);
    UNUSED(movs);
}

void Bot::addRegion(int region, int super)
{
    m_world.addRegion(region, super);
}

void Bot::addNeighbor(int region, int neigh)
{
    m_world.addLink(region, neigh);
}

void Bot::addWasteland(int region)
{
    m_world.addWasteland(region);
}

void Bot::addSuperRegion(int super, int reward)
{
    m_world.addSuperRegion(super, reward);
}

void Bot::setName(const std::string& _name)
{
    name = _name;
}

void Bot::setOppName(const std::string& name)
{
    oppName = name;
}

void Bot::setAvailArmies(int armies)
{
    availArmies = armies;
}

void Bot::setTimebank(int _timebank)
{
    timebank = _timebank;
}

void Bot::setTimePerMove(int time)
{
    timePerMove = time;
}

void Bot::setMaxRounds(int rounds)
{
    maxRounds = rounds;
}

void Bot::handleInitialStartingRegions(const std::vector<int> &regions)
{
    UNUSED(regions);
}

void Bot::setPossibleStartingRegions(const std::vector<int> &regions)
{
    possibleStartingRegions = std::move(regions);
}

void Bot::handleOppStartingRegions(const std::vector<int>& regions)
{
    // TODO
    UNUSED(regions);
}

void Bot::startDelay(int delay)
{
    UNUSED(delay);
}

void Bot::updateRegion(int region, const std::string& player, int armies)
{
    Player p = player == name ? Player::ME :
                                player == oppName ? Player::OPPONENT :
                                                    Player::NEUTRAL;
    m_world.updateRegion(region, p, armies);
}

void Bot::resetOwnedRegions()
{
    ownedRegions.clear();
}

} // namespace warlightAi
