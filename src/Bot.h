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

    void play();    ///< plays a single game of Warlight

    void pick_starting_region();
    void place_armies();
    void make_moves();   ///< makes moves for a single turn

    /**
     * Evaluates the current state and in turn figures out the proper action for
     * the current move Hook in your army placements and attack moves here
     */
    void eval();

    void handle_opp_moves(const Placements& pls, const Movements& movs);


    /// Interface for settings
    void add_region(std::size_t region, std::size_t super);
    void add_super_region(std::size_t super, int reward);
    void add_neighbors(std::size_t region, std::size_t neigh);
    void add_wasteland(std::size_t region);

    void set_name(const std::string& name);
    void set_opp_name(const std::string& opp_name);
    void set_initial_armies(int armies);
    void set_timebank(int timebank);
    void set_time_per_move(int time);
    void set_max_rounds(int rounds);


    void clear_starting_regions();
    void add_starting_region(std::size_t region);

    void add_opp_starting_region(std::size_t region);

    void start_delay(int delay);
    void set_state(State state);

    /**
     * Updates the regions from the game engine
     * @param noRegion region identifier
     * @param playerName player who owns it
     * @param nbArmies number of armies he gets
     */
    void update_region(std::size_t region, const std::string& name, int armies);

    void reset_owned_regions();

private:
    /**
     * Adds armies to a region
     * @param region region to add to
     * @param armies number of armies
     */
    void add_armies(std::size_t region, int armies);

    /**
     * Moves armies on the map
     * @param from_reg starting region
     * @param to_reg target region
     * @param armies number of armies
     */
    void move_armies(std::size_t from_reg, std::size_t to_reg, int armies);


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
    std::vector<unsigned> opp_starting_regions;
    std::vector<int> owned_regions;

    State state;
};

#endif // BOT_H_INCLUDED
