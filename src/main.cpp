// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//


// C++
#include <iostream>
#include <exception>

// Project
#include "Bot.h"


int main()
{
    std::ios_base::sync_with_stdio(false);

#ifndef STARTERBOT_DEBUG
    try
    {
#endif
        warlightAi::Bot b;
        b.play();
#ifndef STARTERBOT_DEBUG
    } catch (std::exception& ex) {
        std::cerr << "Exception:" << ex.what() << std::endl;
        return 1;
    }
#endif

    return 0;
}
