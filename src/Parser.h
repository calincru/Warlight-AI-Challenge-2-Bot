// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

// Project
#include "Settings.h"
#include "boost/noncopyable.hpp"


// Fwrd decls
class Bot;

class Parser : private boost::noncopyable
{
public:
    Parser(Bot* bot);

    void parseInput();

private:
    void parse_setup_map();
    void parseStartingRegions();
    void parse_settings();
    void parse_update_map();
    void parse_opp_moves();
    void parse_go();
    void parse_super_regions();
    void parse_regions();
    void parse_pick_starting_region();
    void parse_opp_starting_regions();
    void parse_neighbors();
    void parse_wastelands();

    // helper function for the case we want to handle \r\n in future
    bool lineEnds() const;

    Bot* bot;
};

#endif // PARSER_H_INCLUDED
