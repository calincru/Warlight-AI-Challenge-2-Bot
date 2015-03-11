// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

// C++
#include <stdio.h>
#include <string>
#include <vector>

// Project
#include "main.h"


class Bot;

class Parser : private boost::noncopyable
{
public:
    Parser(Bot* bot);
    virtual ~Parser();

    void parseInput();
    void parseSetupMap();
    void parseStartingRegions();
    void parseSettings();
    void parseUpdateMap();
    void parseOpponentMoves();
    void parseGo();
    void parseSuperRegions();
    void parseRegions();
    void parsePickStartingRegion();
    void parseOpponentStartingRegions();
    void parseNeighbors();
    void parseWastelands();

private:
    Parser();

    // helper function for the case we want to handle \r\n in future
    bool lineEnds()
    {
        return bool(std::cin.peek() == '\n');
    }


    Bot* theBot;
};

#endif // PARSER_H_INCLUDED
