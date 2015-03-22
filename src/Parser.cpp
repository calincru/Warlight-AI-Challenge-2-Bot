// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// Self
#include "Parser.h"

// C++
#include <iostream>
#include <stdexcept>
#include <limits>

// Project
#include "Bot.h"
#include "StringManipulation.h"
#include "utils.h"



namespace warlightAi {

Parser::Parser(Bot &bot)
    : m_bot(bot)
{
    // nothing to do
}

bool Parser::lineEnds() const
{
    return std::cin.peek() == '\n';
}

void Parser::parseInput()
{
    std::string inputType;
    while (std::cin >> inputType) {
        if (inputType == "setup_map")
            parseSetupMap();
        else if (inputType == "settings")
            parseSettings();
        else if (inputType == "update_map")
            parseUpdateMap();
        else if (inputType == "opponent_moves")
            parseOppMoves();
        else if (inputType == "pick_starting_region")
            parsePickStartingRegion();
        else if (inputType == "go")
            parseGo();
        else {
            std::string line;
            getline(std::cin, line);
            std::cerr << "BAD TOKEN: " << inputType << " in line " << line
                      << std::endl;
        }
    }
}

void Parser::parseSetupMap()
{
    std::string setupType;
    std::cin >> setupType;
    if (setupType == "super_regions")
        parseSuperRegions();
    else if (setupType == "regions")
        parseRegions();
    else if (setupType == "neighbors")
        parseNeighbors();
    else if (setupType == "wastelands")
        parseWastelands();
    else if (setupType == "opponent_starting_regions")
        parseOppStartingRegions();
}

void Parser::parseSettings()
{
    std::string settingType;
    std::cin >> settingType;

    if (settingType == "timebank") {
        int timebank;
        std::cin >> timebank;
        m_bot.setTimebank(timebank);
    } else if (settingType == "time_per_move") {
        int timePerMove;
        std::cin >> timePerMove;
        m_bot.setTimePerMove(timePerMove);
    } else if (settingType == "max_rounds") {
        int maxRounds;
        std::cin >> maxRounds;
        m_bot.setMaxRounds(maxRounds);
    } else if (settingType == "your_bot") {
        std::string botName;
        std::cin >> botName;
        m_bot.setName(botName);
    } else if (settingType == "opponent_bot") {
        std::string botName;
        std::cin >> botName;
        m_bot.setOppName(botName);
    } else if (settingType == "starting_armies") {
        int availArmies;
        std::cin >> availArmies;
        m_bot.setAvailArmies(availArmies);
    } else if (settingType == "starting_regions") {
        int startingRegion;
        while (!lineEnds() && std::cin >> startingRegion)
            m_bot.addStartingRegion(startingRegion);

       m_bot.handleRequest(Request::CHECK_STARTING_REGIONS);
    } else if (settingType == "starting_pick_amount") {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void Parser::parseUpdateMap()
{
    std::string player;
    int region;
    int armies;

    while (!lineEnds() && std::cin >> region >> player >> armies)
        m_bot.updateRegion(region, player, armies);
}

void Parser::parseOppMoves()
{
    std::string player, action;
    while (!lineEnds() && std::cin >> player >> action) {
        int fromReg, toReg;
        int armies;

        if (action == "placeArmies") {
            std::cin >> toReg >> armies;
            m_bot.addOpponentDeployment(toReg, armies);
        } else if (action == "attack/transfer") {
            std::cin >> fromReg >> toReg >> armies;
            m_bot.addOpponentAttack(fromReg, toReg, armies);
        }
    }

    m_bot.handleRequest(Request::CHECK_OPPONENT_MOVES);
}

void Parser::parseGo()
{
    std::string request;
    int delay;
    std::cin >> request >> delay;

    m_bot.startDelay(delay);
    if (request == "place_armies")
        m_bot.handleRequest(Request::PLACE_ARMIES);
    else if (request == "attack/transfer")
        m_bot.handleRequest(Request::ATTACK_TRANSFER);
    else
        throw std::invalid_argument("Cannot handle " + request + "correctly");
}

void Parser::parseSuperRegions()
{
    int super;
    int reward;
    while (!lineEnds() && std::cin >> super >> reward)
        m_bot.addSuperRegion(super, reward);
}

void Parser::parseRegions()
{
    int super, region;
    while (!lineEnds() && std::cin >> region >> super)
        m_bot.addRegion(region, super);
}

void Parser::parsePickStartingRegion()
{
    int delay;
    std::cin >> delay;
    m_bot.startDelay(delay);

    int pickableRegion;
    while (!lineEnds() && std::cin >> pickableRegion)
        m_bot.addPickableRegion(pickableRegion);

    m_bot.handleRequest(Request::PICK_STARTING_REGION);
}

void Parser::parseOppStartingRegions()
{
    int region;
    while (!lineEnds() && std::cin >> region)
        m_bot.addOpponentStartingRegion(region);

    m_bot.handleRequest(Request::CHECK_OPPONENT_STARTING_REGIONS);
}

void Parser::parseNeighbors()
{
    int region;
    std::string neighbors;

    while (!lineEnds() && std::cin >> region >> neighbors) {
        auto neighborsFlds = StringManipulation::comma_split(neighbors);
        for (auto &neigh : neighborsFlds)
            m_bot.addNeighbor(region, std::stoi(neigh));
    }
}

void Parser::parseWastelands()
{
    int region;
    while (!lineEnds() && std::cin >> region)
        m_bot.addWasteland(region);
}

} // namespace warlightAi
