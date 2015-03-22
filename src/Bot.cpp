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
#include "BasicRoundStrategy.h"

// C++
#include <iostream>
#include <cmath>
#include <algorithm>
#include <memory>



namespace warlightAi {

Bot::Bot()
    : m_pickStrategy(nullptr)
    , m_roundStrategy(nullptr)
{
}

void Bot::play()
{
    Parser(*this).parseInput();
}

void Bot::handleRequest(Request request)
{
    if (request == Request::CHECK_STARTING_REGIONS) {
        checkStartingRegions();
    } else if (request == Request::PICK_STARTING_REGION) {
        pick();
    } else if (request == Request::PLACE_ARMIES) {
        m_roundStrategy.reset(
            new BasicRoundStrategy(m_world, m_availableArmies)
        );
        deploy();
    } else if (request == Request::ATTACK_TRANSFER) {
        attack();
    } else if (request == Request::CHECK_OPPONENT_STARTING_REGIONS) {
        checkOpponentStartingRegions();
    } else if (request == Request::CHECK_OPPONENT_MOVES) {
        checkOpponentMoves();
    }
}

void Bot::pick()
{
    std::cout << m_pickStrategy->pickNext(m_pickableRegions)->id() << std::endl;
}

void Bot::deploy()
{
    auto deployments = m_roundStrategy->getDeployments();
    for (auto &entry : deployments) {
        std::cout << m_name << " deploy " << entry.first->id() << " "
                  << entry.second << std::endl;

        entry.first->setArmies(entry.first->getArmies() + entry.second);
    }
}

void Bot::attack()
{
    auto attacks = m_roundStrategy->getAttacks();
    for (auto &attack : attacks)
        std::cout << m_name << " attack/transfer " << std::get<0>(attack)->id()
                  << " " << std::get<1>(attack)->id() << " "
                  << std::get<2>(attack) << std::endl;
}

void Bot::checkStartingRegions()
{
    m_pickStrategy.reset(new BasicPickStrategy(m_startingRegions));
}

void Bot::checkOpponentStartingRegions()
{
    // Correctness guaranteed by commands order. Just because it's not needed
    // anymore.
    m_pickStrategy.reset();
    m_startingRegions.clear();
    m_pickableRegions.clear();

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

void Bot::setAvailableArmies(int availableArmies)
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
