// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

// Project
#include "globals.h"
#include "boost/noncopyable.hpp"


namespace warlightAi {

// Fwrd decls
class Bot;

/**
 * The class which does all the parsing for our Bot.
 */
class Parser : private boost::noncopyable
{
public:
    /**
     * Creates a new and unique parser. It stores a reference to the Bot object
     * in order to forward to it all the information parsed.
     */
    Parser(Bot &bot);

    /**
     * Tells the parser to start parsing.
     */
    void parseInput();

private:
    // Helper methods mainly used to modularize based on different commands
    // received from the engine.
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

    bool lineEnds() const;

    Bot &m_bot;

}; // class Parser

} // namespace warlightAi

#endif // PARSER_H_INCLUDED
