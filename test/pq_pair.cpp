#include <iostream>
#include <queue>
#include <cassert>
#include <limits>


int main()
{
    const auto entries = 10;

    // Test with declared comparator
    auto pq_funct = [](const auto &lhs, const auto &rhs) {
            return lhs.first <= rhs.first;
    };
    using ScoreQueue = std::priority_queue<
                                            std::pair<int, int>,
                                            std::vector<std::pair<int, int>>,
                                            decltype(pq_funct)
                                          >;

    ScoreQueue q(pq_funct);
    for (auto i = 0; i < 2*entries; ++i)
        q.emplace(std::rand() % entries, i);

    auto last = std::numeric_limits<int>::max();
    while (!q.empty()) {
        auto p = q.top();
        assert(p.first <= last);
        last = p.first;
        q.pop();
    }

    // Test with pair's default comparator
    std::priority_queue<std::pair<int, int>> qq;
    for (auto i = 0; i < 2*entries; ++i)
        qq.emplace(std::rand() % entries, i);

    last = std::numeric_limits<int>::max();
    while (!qq.empty()) {
        auto p = qq.top();
        assert(p.first <= last);
        last = p.first;
        qq.pop();
    }

    return 0;
}
