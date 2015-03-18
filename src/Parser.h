// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

// Project
#include "consts.h"
#include "boost/noncopyable.hpp"

// C++
#include <vector>
#include <tuple>


namespace warlightAi {

// Fwrd decls
class Bot;

class Parser : private boost::noncopyable
{
public:
    Parser(Bot &bot);

    void parseInput();

private:
    void parseSetupMap();
    void parseStartingRegions();
    void parseSettings();
    void parseUpdateMap();
    void parseOppMoves();
    void parseGo();
    void parseSuperRegions();
    void parseRegions();
    void parsePickStartingRegion();
    void parseOppStartingRegions();
    void parseNeighbors();
    void parseWastelands();

    // helper function for the case we want to handle \r\n in future
    bool lineEnds() const;

    Bot &m_bot;

}; // class Parser

} // namespace warlightAi

#endif // PARSER_H_INCLUDED
