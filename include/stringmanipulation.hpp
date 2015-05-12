// This program is free software licenced under MIT Licence. You can
// find a copy of this licence in LICENCE.txt in the top directory of
// source code.
//

#ifndef STRING_MANIPULATION_H_INCLUDED
#define STRING_MANIPULATION_H_INCLUDED

// C++
#include <vector>
#include <string>
#include <sstream>

namespace stringManipulation {

inline std::vector<std::string> comma_split(const std::string &s)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;

    while (std::getline(ss, item, ','))
        if (!item.empty())
            tokens.emplace_back(item);

    return tokens;
}

inline std::string comma_join(const std::vector<std::string> &elems)
{
    std::string res = elems.front();

    for (auto it = elems.begin() + 1; it != elems.end(); ++it)
        res += ',' + *it;

    return res;
}

} // namespace stringManipulation

#endif // STRING_MANIPULATION_H_INCLUDED
