// C++
#include <iostream>
#include <exception>

// Project
#include "Bot.h"

int main(int /*argc*/, char ** /*argv[] */)
{
    std::cout.sync_with_stdio(false);
    std::cout << "It works." << std::endl;

#ifndef STARTERBOT_DEBUG
    try
    {
#endif
        Bot bot;
        bot.playGame();
#ifndef STARTERBOT_DEBUG
    } catch (std::exception& ex)
    {
        std::cerr << "Exception:" << ex.what() << std::endl;
        return 1;
    }
#endif

    return 0;
}
