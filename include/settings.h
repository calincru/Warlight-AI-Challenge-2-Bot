// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


#ifndef CONSTS_H_INCLUDED
#define CONSTS_H_INCLUDED

#include <vector>
#include <tuple>

namespace warlightAi {

const int neutralArmies = 2;
const int wastelandArmies = 6;

enum class Player {
    ME,
    OPPONENT,
    NEUTRAL,
    UNKNOWN
};

}

#endif // CONSTS_H_INCLUDED
