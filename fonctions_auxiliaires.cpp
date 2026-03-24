#include <vector>
#include <algorithm>

template <typename T>
std::vector<T> linspace(T start, T stop, unsigned int N){
    std::vector<T> lin;
    T span = stop - start;
    T step = span / (N-1);
    if(span < (T) 0){
        lin = {};
    }
    else if (span == (T) 0){
        lin = {(T) start};
    }
    
    else{
        lin.resize(N); // pour ne pas agir sur un vecteur vide
        for(int i = 0; i < N; i++){
            lin[i] = (T) (start + i * step); 
        }
    }
    return lin;
}


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