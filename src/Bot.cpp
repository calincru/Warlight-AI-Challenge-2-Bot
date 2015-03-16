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


class ScoreComputer
{
public:
    ScoreComputer(const Bot &b)
        : m_bot(b)
    {
        // If you decide to make a copy of the bot, change the type of m_bot
        // from const Bot& to Bot.
    }

    ScoreQueue compute_scores()
    {
        // TODO
        return  ScoreQueue();
    }

    ScoreQueue compute_initial_scores(const std::vector<int> &starting_regions)
    {
        // TODO
        UNUSED(starting_regions);
        return  ScoreQueue();
    }

private:
    // Add any helper method here.

    const Bot &m_bot;
};

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
    else
        throw std::invalid_argument("Unknown request");
}

void Bot::pick_starting_region()
{
    // Pick some region based on super_scores.top() maybe.
}

void Bot::place_armies()
{
    super_scores = ScoreComputer(*this).compute_scores();

    // TODO: Use the queue to place the armies; e.g. check if we have enough
    // armies to attack the highest ranked super region. If not, place some
    // armies. If there are remaining armies to be placed, take the next
    // highest ranked super region, and so on. Just an example.

    // Use this vector to store the popped super regions and add them back into
    // the priority queue at the end of this method.
    std::vector<std::pair<int, int>> visited_super_regions;

    // More code..

    // E.g.
    for (auto &entry : visited_super_regions)
        super_scores.push(entry);




    // Random - will be removed - kept for inspiration
    auto region = owned_regions[std::rand() % owned_regions.size()];
    std::cout << name << " place_armies " << region << " " << avail_armies
              << std::endl;

    // Update armies count
    armies_cnt[region] += avail_armies;
}

void Bot::make_moves()
{
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

    // TODO: Use the super_scores priority queue to take the highest ranked super
    // regions and get your armies towards it.


    // Random - will be removed - kept for inspiration
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

void Bot::handle_initial_starting_regions(const std::vector<int> &regions)
{
    ScoreComputer(*this).compute_initial_scores(regions);
}

void Bot::set_possible_starting_regions(const std::vector<int> &regions)
{
    possible_starting_regions = std::move(regions);
}

void Bot::handle_opp_starting_regions(const std::vector<int>& regions)
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
