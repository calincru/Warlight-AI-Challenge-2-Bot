// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// Self
#include "Bot.h"

// Project
#include "utils.h"
#include "Parser.h"
#include "Region.h"
#include "SuperRegion.h"
#include "BasicPickStrategy.h"

// C++
#include <iostream>
#include <cmath>
#include <algorithm>
#include <memory>



namespace warlightAi {

void Bot::play()
{
    Parser(*this).parseInput();
}

void Bot::handleRequest(Request request)
{
    if (request == Request::CHECK_STARTING_REGIONS)
        checkStartingRegions();
    else if (request == Request::PICK_STARTING_REGION)
        pick();
    else if (request == Request::PLACE_ARMIES)
        deploy();
    else if (request == Request::ATTACK_TRANSFER)
        attack();
    else if (request == Request::CHECK_OPPONENT_STARTING_REGIONS)
        checkOpponentStartingRegions();
    else if (request == Request::CHECK_OPPONENT_MOVES)
        checkOpponentMoves();
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

    return (10 - (-sum)/m_availableArmies) * superRegion->getReward();
}

auto Bot::planMoves() -> std::pair<RegionPtr, RegionPtr>
{
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

void Bot::pick()
{
    std::cout << m_pickStrategy->pickNext(m_pickableRegions)->id() << std::endl;
}

void Bot::deploy()
{
    auto p = planMoves();
    std::cout << m_name << " deploy " << p.first->id()  << " "
              << m_availableArmies << std::endl;

    p.first->setArmies(p.first->getArmies() + m_availableArmies);
}

void Bot::attack()
{
    auto p = planMoves();
    std::cout << m_name << " attack/transfer " << p.first->id() << " "
              << p.second->id() << " " << p.first->getArmies() - 1 << std::endl;
}

void Bot::checkStartingRegions()
{
    m_pickStrategy.reset(
        new BasicPickStrategy(m_startingRegions, m_availableArmies)
    );
}

void Bot::checkOpponentStartingRegions()
{
    // Correctness guaranteed by commands order.
    delete m_pickStrategy.release();

    // TODO
}

void Bot::checkOpponentMoves()
{
    // TODO
}

void Bot::addRegion(int newRegion, int superOfRegion)
{
    m_world.addRegion(newRegion, superOfRegion);
}

void Bot::addSuperRegion(int superRegion, int superRegionReward)
{
    m_world.addSuperRegion(superRegion, superRegionReward);
}

void Bot::addNeighbor(int region, int regionNeigh)
{
    m_world.addLink(region, regionNeigh);
}

void Bot::addWasteland(int targetRegion)
{
    m_world.addWasteland(targetRegion);
}

void Bot::addStartingRegion(int startingRegion)
{
    m_startingRegions.emplace_back(m_world.getRegionById(startingRegion));
}

void Bot::addPickableRegion(int pickableRegion)
{
    m_pickableRegions.emplace_back(m_world.getRegionById(pickableRegion));
}

void Bot::addOpponentAttack(int fromRegion, int toRegion, int armiesCount)
{
    m_opponentAttacks.emplace_back(
                        m_world.getRegionById(fromRegion),
                        m_world.getRegionById(toRegion),
                        armiesCount
    );
}

void Bot::addOpponentDeployment(int destRegion, int armiesCount)
{
    m_opponentDeployments.emplace_back(
                        m_world.getRegionById(destRegion),
                        armiesCount
    );
}

void Bot::addOpponentStartingRegion(int startingRegion)
{
    m_opponentStartingRegions.emplace_back(
                        m_world.getRegionById(startingRegion)
    );
}

void Bot::setName(const std::string &name)
{
    m_name = name;
}

void Bot::setOppName(const std::string &oppName)
{
    m_oppName = oppName;
}

void Bot::setAvailArmies(int availableArmies)
{
    m_availableArmies = availableArmies;
}

void Bot::setTimebank(int timebank)
{
    m_timebank = timebank;
}

void Bot::setTimePerMove(int timePerMove)
{
    m_timePerMove = timePerMove;
}

void Bot::setMaxRounds(int maxRounds)
{
    m_maxRounds = maxRounds;
}

void Bot::startDelay(int delay)
{
    UNUSED(delay);
}

void Bot::updateRegion(int region, const std::string &playerName, int armiesCnt)
{
    Player p = playerName == m_name ?
                                Player::ME :
                                playerName == m_oppName ?
                                                    Player::OPPONENT :
                                                    Player::NEUTRAL;
    m_world.updateRegion(region, p, armiesCnt);
}

} // namespace warlightAi
