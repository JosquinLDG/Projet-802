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

// paramètres de la droite dans l'espace de Hough
struct ParamHough {
    double rho;
    double theta;
};



double distance(const ParamHough& a, const ParamHough& b) {
    return std::sqrt((a.rho - b.rho)*(a.rho - b.rho) + (a.theta - b.theta)*(a.theta - b.theta));
}

std::vector<ParamHough> Clustering(const std::vector<double> &X, const std::vector<double>& Y, double threshold) {
    int n = X.size();
    std::vector<ParamHough> points(n);
    
    for (int i = 0; i < n; i++) {
        points[i] = {X[i], Y[i]};
    }

    std::vector<bool> visited(n, false);
    std::vector<ParamHough> centers;

    for (int i = 0; i < n; i++) {
        if (visited[i]) continue;

        std::vector<ParamHough> cluster;
        cluster.push_back(points[i]);
        visited[i] = true;

        // regroupe les points proches
        for (int j = 0; j < n; j++) {
            if (!visited[j] && distance(points[i], points[j]) < threshold) {
                cluster.push_back(points[j]);
                visited[j] = true;
            }
        }

        // calcule le centre
        double sumX = 0, sumY = 0;
        for (auto& p : cluster) {
            sumX += p.rho;
            sumY += p.theta;
        }

        centers.push_back({sumX / cluster.size(), sumY / cluster.size()});
    }

    return centers;
}