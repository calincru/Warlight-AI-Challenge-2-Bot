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
#include <algorithm>

// Project
#include "Parser.h"
#include "StringManipulation.h"
#include "utils.h"

Bot::Bot()
    : adj_list(1)
    , super_rewards(1)
    , regs_super(1)
    , super_table(1)
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

int Bot::compute_score(int super_region) {
    auto regs = super_table[super_region];

    auto nr_regs = regs.size();
    auto sum = -(static_cast<int>(nr_regs));

    for (auto i = 0u; i < nr_regs; ++i)
        if (regs_owner[regs[i]] == Player::ME)
            sum += armies_cnt[regs[i]];
        else
            sum -= 3./2. * armies_cnt[regs[i]];

    if (sum > 0)
        sum = 0;

    return (10 - (-sum)/avail_armies) * super_rewards[super_region];
}

std::pair<int, int> Bot::plan_moves()
{
    // TODO
    //
    // Because deployments are dependent on planned attacks/transfers, there
    // should be a function that decides all the actions for the second phase
    // and deploys accordingly. This function should not return a pair, but
    // rather compute all possible pairs needed for the attack/transfer phase
    // and call the "place_armies" and "make_moves" functions (which should take
    // as parameters either a list of pairs or one pair at a time.

    std::vector<std::pair<int, int>> my_reg_other_reg_pairs;

    // Finds all the adjacent super regions and keep all the (src, dest) pairs
    // for possible attacks
    for (auto my_reg: owned_regions)
        for (auto other_reg: adj_list[my_reg])
            if (regs_owner[other_reg] != Player::ME)
                my_reg_other_reg_pairs.emplace_back(my_reg, other_reg);

    std::pair<int, int> max_pair;
    auto max_score = -1;
    for (auto pair: my_reg_other_reg_pairs)
        if (compute_score(regs_super[pair.second]) > max_score) {
            max_score = compute_score(regs_super[pair.second]);
            max_pair = pair;
        }
    return max_pair;
}

void Bot::pick_starting_region()
{
    // There is no need for a priority queue, since the score of the starting
    // regions might vary based on the previous choices of either botm, so this
    // function just picks the region with the maximum score.

    auto max_reg = -1;
    auto max_score = -1;
    for (auto reg: possible_starting_regions)
        if (compute_score(regs_super[reg]) > max_score) {
            max_score = compute_score(regs_super[reg]);
            max_reg = reg;
        }
    std::cout << max_reg << std::endl;
}

void Bot::place_armies()
{
    // TODO: Use the queue to place the armies; e.g. check if we have enough
    // armies to attack the highest ranked super region. If not, place some
    // armies. If there are remaining armies to be placed, take the next
    // highest ranked super region, and so on. Just an example.

    // Use this vector to store the popped super regions and add them back into
    // the priority queue at the end of this method.
    // std::vector<std::pair<int, int>> visited_super_regions;

    // More code..

    // E.g.
    // for (auto &entry : visited_super_regions)
    //    super_scores.push(entry);


    auto p = plan_moves();

    std::cout << name << " place_armies " << p.first  << " " << avail_armies
              << std::endl;

    // Update armies count
    armies_cnt[p.first] += avail_armies;
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

    auto p = plan_moves();
    std::cout << name << " attack/transfer " << p.first << " " << p.second
              << " " << (armies_cnt[p.first] - 1) << std::endl;
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

    super_table[super].emplace_back(region);
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
    super_table.emplace_back();
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
    // Nu știu dacă mai e utilă funcția asta, dacă calculezi scorul regiunilor
    // la fiecare alegere
    //
    // ScoreComputer(*this).compute_initial_scores(regions);

    UNUSED(regions);
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
