// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// Self
#include "Bot.h"

// C++
#include <iostream>
#include <sstream>
#include <cmath>
#include <cassert>

// Project
#include "Parser.h"
#include "StringManipulation.h"
#include "utils.h"


Bot::Bot()
    : adj_list(1)
    , super_rewards(1)
    , regs_super(1)
    , armies_cnt(1)
    , regs_owner(1)
{
    // nothing to do
}

void Bot::play()
{
    Parser(this).parseInput();
}

void Bot::handle_request(Request request)
{
    if (request == Request::PICK_STARTING_REGION)
        pick_starting_region();
    else if (request == Request::PLACE_ARMIES)
        place_armies();
    else if (request == Request::ATTACK_TRANSFER)
        make_moves();
}

void Bot::pick_starting_region()
{
    // TODO
    std::cout << possible_starting_regions[std::rand() % possible_starting_regions.size()] << std::endl;
}

void Bot::place_armies()
{
    // TODO
    auto region = owned_regions[std::rand() % owned_regions.size()];
    std::cout << name << " place_armies " << region << " " << avail_armies
              << std::endl;

    add_armies(region, avail_armies);
}

void Bot::make_moves()
{
    // TODO

    /// Output No moves when you have no time left or do not want to commit any
    /// moves.
    //
    // std::cout << "No moves "  << std::endl;
    //
    /// Anatomy of a single move
    //
    //  std::cout << name << " attack/transfer " << from << " " << to << " "
    //            << armies_moved;
    //
    /// When outputting multiple moves they must be seperated by a comma
    //

    std::vector<std::string> moves;
    for (auto j = 0u; j < owned_regions.size(); ++j) {
        std::stringstream move;

        auto i = owned_regions[j];
        if (armies_cnt[i] <= 1)
            continue;

        auto target = adj_list[i].at(std::rand() % adj_list[i].size());
        // prefer enemy regions
        for (auto k = 0; k < 5; ++k) {
            if (regs_owner[target] != Player::ME)
                break;
            target = adj_list[i].at(std::rand() % adj_list[i].size());
        }
        move << name << " attack/transfer " << i << " " << target << " "
             << (armies_cnt[i] - 1);
        moves.emplace_back(move.str());
    }

    std::cout << StringManipulation::comma_join(moves) << std::endl;
}

void Bot::handle_opp_moves(const Placements& pls, const Movements& movs)
{
    // TODO
    UNUSED(pls);
    UNUSED(movs);
}

void Bot::add_region(int region, int super)
{
    assert(adj_list.size() == static_cast<std::size_t>(region));
    adj_list.emplace_back(std::vector<int>());

    assert(regs_super.size() == static_cast<std::size_t>(region));
    regs_super.emplace_back(super);

    assert(armies_cnt.size() == static_cast<std::size_t>(region));
    armies_cnt.emplace_back(NEUTRAL_ARMIES);

    assert(regs_owner.size() == static_cast<std::size_t>(region));
    regs_owner.emplace_back(Player::NEUTRAL);
}

void Bot::add_neighbor(int region, int neigh)
{
    adj_list[region].emplace_back(neigh);
    adj_list[neigh].emplace_back(region);
}

void Bot::add_wasteland(int region)
{
    wastelands.emplace_back(region);
    armies_cnt[region] = WASTELAND_ARMIES;
}

void Bot::add_super_region(int super, int reward)
{
    assert(super_rewards.size() == static_cast<std::size_t>(super));
    super_rewards.emplace_back(reward);
}

void Bot::set_name(const std::string& _name)
{
    name = _name;
}

void Bot::set_opp_name(const std::string& name)
{
    opp_name = name;
}

void Bot::set_avail_armies(int armies)
{
    avail_armies = armies;
}

void Bot::set_timebank(int _timebank)
{
    timebank = _timebank;
}

void Bot::set_time_per_move(int time)
{
    time_per_move = time;
}

void Bot::set_max_rounds(int rounds)
{
    max_rounds = rounds;
}

void Bot::set_initial_starting_regions(const std::vector<int> &regions)
{
    // TODO
    UNUSED(regions);
}

void Bot::set_possible_starting_regions(const std::vector<int> &regions)
{
    possible_starting_regions = std::move(regions);
}

void Bot::handle_opp_starting_region(const std::vector<int>& regions)
{
    // TODO
    UNUSED(regions);
}

void Bot::start_delay(int delay)
{
    UNUSED(delay);
}

void Bot::update_region(int region, const std::string& player, int armies)
{
    // TODO take the changes into account
    armies_cnt[region] = armies;
    regs_owner[region] = player == name ? Player::ME :
                                          player == opp_name ? Player::ENEMY :
                                                               Player::NEUTRAL;
    if (player == name)
        owned_regions.emplace_back(region);
}

void Bot::reset_owned_regions()
{
    owned_regions.clear();
}

void Bot::add_armies(int region, int armies)
{
    armies_cnt[region] += armies;
}

void Bot::move_armies(int from_reg, int to_reg, int armies)
{
    if (regs_owner[from_reg] == regs_owner[to_reg] &&
        armies_cnt[from_reg] > armies) {

        armies_cnt[from_reg] -= armies;
        armies_cnt[to_reg] += armies;
    } else if (armies_cnt[from_reg] > armies) {
        armies_cnt[from_reg] -= armies;
        if (armies_cnt[to_reg] - std::round(armies * 0.6) <= 0) {
            armies_cnt[to_reg] = armies - std::round(armies_cnt[to_reg] * 0.7);
            regs_owner[to_reg] = regs_owner[from_reg];
        } else {
            armies_cnt[from_reg] += armies - std::round(armies_cnt[to_reg] * 0.7);
            armies_cnt[to_reg] -= std::round(armies * 0.6);
        }
    }
}
