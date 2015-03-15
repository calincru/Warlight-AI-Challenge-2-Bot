// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

// C++
#include <vector>
#include <string>

// Project
#include "Settings.h"
#include "boost/noncopyable.hpp"



// Frwd decls
class Parser;

class Bot : private boost::noncopyable
{
public:
    Bot();

    /**
     * Plays a single game of Warlight
     */
    void play();

    /**
     * Prints to the stdout the index of the picked region
     */
    void pick_starting_region();

    /**
     * Makes armies placements for a single turn
     */
    void place_armies();

    /**
     * Makes moves for a single turn
     */
    void make_moves();

    /**
     * Evaluates the current state and in turn figures out the proper action for
     * the current move Hook in your army placements and attack moves here
     */
    void eval();

    /**
     * Gets the placements and movements of the opponent (those ones our bot can
     * `see`).
     */
    void handle_opp_moves(const Placements& pls, const Movements& movs);

    /**
     * Gets the indexes of the starting regions and does some computation based
     * on them. E.g. computes the `scores` for each one of them to easily make a
     * choose in the pick starting regions phase.
     */
    void handle_starting_regions(const std::vector<int> &regions);

    /// Interface for settings
    void add_region(int region, int super);
    void add_super_region(int super, int reward);
    void add_neighbor(int region, int neigh);
    void add_wasteland(int region);

    void set_name(const std::string& name);
    void set_opp_name(const std::string& opp_name);
    void set_initial_armies(int armies);
    void set_timebank(int timebank);
    void set_time_per_move(int time);
    void set_max_rounds(int rounds);

    /**
     * Adds a new starting region among which our bot will have to choose from
     * in the PICK_STARTING_REGION phase. It might use some precomputations done
     * when the starting regions have first been announced (when the method
     * handle_starting_regions has been called).
     */
    void add_starting_region(int region);

    /**
     * Gets the indexes of the opponents starting regions and does some
     * computation based on them.
     */
    void handle_opp_starting_region(const std::vector<int>& region);

    void start_delay(int delay);
    void set_state(State state);

    /**
     * Updates the regions from the game engine
     * @param noRegion region identifier
     * @param playerName player who owns it
     * @param nbArmies number of armies he gets
     */
    void update_region(int region, const std::string& name, int armies);

    void reset_owned_regions();

private:
    /**
     * Adds armies to a region
     * @param region region to add to
     * @param armies number of armies
     */
    void add_armies(int region, int armies);

    /**
     * Moves armies on the map
     * @param from_reg starting region
     * @param to_reg target region
     * @param armies number of armies
     */
    void move_armies(int from_reg, int to_reg, int armies);


    // Lista de adiacență; aka Graful; Pe poziția i se afla lista cu vecinii
    // regiunii i.
    AdjencyList adj_list;

    // Rewardurile fiecărei super regiuni; Pe poziția i, rewardul super regiunii
    // i.
    std::vector<int> super_rewards;

    // Lista care conține super regiunile din care fac parte regiunile. Pe
    // poziția i se află supra regiunea din care face parte regiunea i.
    std::vector<int> regs_super;

    // Lista conține numărul de armate pe fiecare regiune despre care botul
    // nostru știe.
    std::vector<int> armies_cnt;

    // Lista conține pe poziția i ownerul regiunii cu indexul i.
    std::vector<int> regs_owner;

    // Lista care conține indicii wasteland-urilor. Pentru a evita iterarea prin
    // ea, ar trebui ținut un set, însă nu cred că are rost pentru inputurile
    // din situația de față.
    std::vector<int> wastelands;

    std::string name;
    std::string opp_name;
    int init_armies_cnt;
    int timebank;
    int time_per_move;
    int max_rounds;

    std::vector<int> starting_regions;
    std::vector<int> owned_regions;

    State state;
};

#endif // BOT_H_INCLUDED
