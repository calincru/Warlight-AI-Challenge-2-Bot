// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// Self
#include "Parser.h"

// C++
#include <iostream>
#include <vector>
#include <stdexcept>

// Project
#include "Bot.h"
#include "StringManipulation.h"


Parser::Parser(Bot* _bot)
    : bot(_bot)
{
    // nothing to do
}

bool Parser::lineEnds() const
{
    return std::cin.peek() == '\n';
}

void Parser::parseInput()
{
    std::string input_type;
    while (std::cin >> input_type) {
        if (input_type == "setup_map")
            parse_setup_map();
        else if (input_type == "settings")
            parse_settings();
        else if (input_type == "update_map")
            parse_update_map();
        else if (input_type == "opponent_moves")
            parse_opp_moves();
        else if (input_type == "pick_starting_region")
            parse_pick_starting_region();
        else if (input_type == "go")
            parse_go();
        else {
            std::string line;
            getline(std::cin, line);
            std::cerr  << input_type << " " << line << std::endl;
        }

        bot->eval();
    }
}

void Parser::parse_setup_map()
{
    std::string setupType;
    std::cin >> setupType;
    if (setupType == "super_regions")
        parse_super_regions();
    else if (setupType == "regions")
        parse_regions();
    else if (setupType == "neighbors")
        parse_neighbors();
    else if (setupType == "wastelands")
        parse_wastelands();
    else if (setupType == "opponent_starting_regions")
        parse_opp_starting_regions();
}

void Parser::parse_settings()
{
    std::string setting_type;
    std::cin >> setting_type;

    if (setting_type == "timebank") {
        int timebank;
        std::cin >> timebank;
        bot->set_timebank(timebank);
    } else if (setting_type == "time_per_move") {
        int time_per_move;
        std::cin >> time_per_move;
        bot->set_time_per_move(time_per_move);
    } else if (setting_type == "max_rounds") {
        int max_rounds;
        std::cin >> max_rounds;
        bot->set_max_rounds(max_rounds);
    } else if (setting_type == "your_bot") {
        std::string bot_name;
        std::cin >> bot_name;
        bot->set_name(bot_name);
    } else if (setting_type == "opponent_bot") {
        std::string bot_name;
        std::cin >> bot_name;
        bot->set_opp_name(bot_name);
    } else if (setting_type == "starting_armies") {
        int starting_armies;
        std::cin >> starting_armies;
        bot->set_initial_armies(starting_armies);
    } else if (setting_type == "starting_regions") {
        std::size_t starting_region;
        while (!lineEnds() && std::cin >> starting_region)
            bot->add_starting_region(starting_region);
    }
}

void Parser::parse_update_map()
{
    std::string player;
    std::size_t region;
    int armies;

    bot->reset_owned_regions();
    while (!lineEnds() && std::cin >> region >> player >> armies)
        bot->update_region(region, player, armies);
}

void Parser::parse_opp_moves()
{
    Placements pls;
    Movements movs;

    std::string player, action;
    while (!lineEnds() && std::cin >> player >> action) {
        unsigned from_reg, to_reg;
        int armies;

        if (action == "place_armies") {
            std::cin >> to_reg >> armies;
            pls.emplace_back(to_reg, armies);
        } else if (action == "attack/transfer") {
            std::cin >> from_reg >> to_reg >> armies;
            movs.emplace_back(from_reg, to_reg, armies);
        }
    }

    bot->handle_opp_moves(pls, movs);
}

void Parser::parse_go()
{
    std::string state;
    int delay;
    std::cin >> state >> delay;

    bot->start_delay(delay);
    if (state == "place_armies") {
        bot->set_state(State::PLACE_ARMIES);
        return;
    } else if (state == "attack/transfer") {
        bot->set_state(State::ATTACK_TRANSFER);
        return;
    }

    throw std::invalid_argument("Cannot handle " + state + "correctly");
}

void Parser::parse_super_regions()
{
    std::size_t super;
    int reward;
    while (!lineEnds() && std::cin >> super >> reward)
        bot->add_super_region(super, reward);
}

void Parser::parse_regions()
{
    std::size_t super, region;
    while (!lineEnds() && std::cin >> region >> super)
        bot->add_region(region, super);
}

void Parser::parse_pick_starting_region()
{
    int delay;
    std::cin >> delay;
    bot->start_delay(delay);

    int region;
    while (!lineEnds() && std::cin >> region)
        bot->add_starting_region(region);

    bot->set_state(State::PICK_STARTING_REGION);
}

void Parser::parse_opp_starting_regions()
{
    std::size_t region;
    while (!lineEnds() && std::cin >> region)
        bot->add_opp_starting_region(region);
}

void Parser::parse_neighbors()
{
    std::size_t region;
    std::string neighbors;

    while (!lineEnds() && std::cin >> region >> neighbors) {
        std::vector<std::string> neighbors_flds;

        string::split(neighbors_flds, neighbors);
        for (auto i = 0u; i < neighbors_flds.size(); i++)
            bot->add_neighbor(region, atoi(neighbors_flds[i].c_str()));
    }

    // TODO:
    // bot->set_state(State::FIND_BORDERS);
}

void Parser::parse_wastelands()
{
    unsigned region;
    while (!lineEnds() && std::cin >> region)
        bot->add_wasteland(region);
}
