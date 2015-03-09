#include <vector>
#include <algorithm>


int main()
{
    std::vector<int> v{9, 2, 1, 5, 7, 4, 3, 8};

    // Test CPP11
    auto f = [&v](decltype(v)::value_type &elem) {
        elem *= 2;
    };
    std::for_each(v.begin(), v.end(), f);

    // Test CPP14
    std::for_each(v.begin(), v.end(), [](auto &elem) {
            elem *= 2;
    });
    return 0;
}
