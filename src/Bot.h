// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

// C++
#include <vector>
#include <string>
#include <queue>

// Project
#include "Settings.h"
#include "boost/noncopyable.hpp"

// Frwd decls
class Parser;

class Bot : private boost::noncopyable
{
public:
    friend class ScoreComputer;

    Bot();

    /**
     * Plays a single game of Warlight
     */
    void play();

    /**
     * Handles requests from the game
     */
    void handle_request(Request request);

    void pick_starting_region();

    /**
     * Returns a (source, destination) pair for an attack
     */
    std::pair<int, int> plan_moves();

    void place_armies();
    void make_moves();
    void handle_opp_moves(const Placements& pls, const Movements& movs);

    /**
     * INITIAL starting positions!!
     * Gets the indexes of the starting regions and does some computation based
     * on them. E.g. computes the `scores` for each one of them to easily make a
     * choice in the pick starting regions phase.
     */
    void handle_initial_starting_regions(const std::vector<int> &regions);

    /**
     * Gets the indexes of the opponents starting regions and does some
     * computation based on them.
     */
    void handle_opp_starting_regions(const std::vector<int>& region);

    /// Interface for settings
    void add_region(int region, int super);
    void add_super_region(int super, int reward);
    void add_neighbor(int region, int neigh);
    void add_wasteland(int region);

    void set_name(const std::string& name);
    void set_opp_name(const std::string& opp_name);
    void set_avail_armies(int armies);
    void set_timebank(int timebank);
    void set_time_per_move(int time);
    void set_max_rounds(int rounds);

    /**
     * Sets the starting regions that are currently possible.
     */
    void set_possible_starting_regions(const std::vector<int>& region);

    void start_delay(int delay);

    /**
     * Updates the regions from the game engine
     * @param region region identifier
     * @param name player who owns it
     * @param armies number of armies he gets
     */
    void update_region(int region, const std::string& name, int armies);
    void reset_owned_regions();

private:
    // Lista de adiacență; aka Graful; Pe poziția i se afla lista cu vecinii
    // regiunii i.
    AdjencyList adj_list;

    // Rewardurile fiecărei super regiuni; Pe poziția i, rewardul super regiunii
    // i.
    std::vector<int> super_rewards;

    // Lista care conține super regiunile din care fac parte regiunile. Pe
    // poziția i se află super regiunea din care face parte regiunea i.
    std::vector<int> regs_super;

    // Lista care conține listele de regiuni dintr-o super regiune. Pe poziția
    // i se află lista cu regiunile care fac parte din super regiunea i.
    std::vector<std::vector<int>> super_table;

    // Lista conține numărul de armate pe fiecare regiune despre care botul
    // nostru știe.
    std::vector<int> armies_cnt;

    // Lista conține pe poziția i ownerul regiunii cu indexul i.
    std::vector<Player> regs_owner;

    // Lista care conține indicii wasteland-urilor. Pentru a evita iterarea prin
    // ea, ar trebui ținut un set, însă nu cred că are rost pentru inputurile
    // din situația de față.
    std::vector<int> wastelands;

    std::string name;
    std::string opp_name;

    // numărul de armate pe tură
    int avail_armies;

    int timebank;
    int time_per_move;
    int max_rounds;

    std::vector<int> possible_starting_regions;
    std::vector<int> owned_regions;

    ScoreQueue super_scores;
};

#endif // BOT_H_INCLUDED
