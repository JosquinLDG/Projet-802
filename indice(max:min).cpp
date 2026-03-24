#include <vector>
#include <algorithm>
#include <iostream> 



std::vector<double> v1 = {3, 4, 1, 10, 9, 4};

struct Maxind {
    int index;
    double value;
};

Maxind trouve_max(const std::vector<double>& v) {
    auto it = std::max_element(v.begin(), v.end());

    Maxind res;
    res.index = it - v.begin();
    res.value = *it;

    return res;
}

struct MinInd {
    int index;
    double value;
};

MinInd trouve_min(const std::vector<double>& v) {
    auto it = std::min_element(v.begin(), v.end());

    MinInd res;
    res.index = it - v.begin();
    res.value = *it;

    return res;
}


