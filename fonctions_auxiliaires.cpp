#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <string>

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
struct ParamPolaires {
    double rho;
    double theta;
    ParamPolaires() : rho(), theta() {}
    ParamPolaires(double p1, double p2) : rho(p1), theta(p2) {}
};



double distance(const ParamPolaires& a, const ParamPolaires& b) {
    return std::sqrt((a.rho - b.rho)*(a.rho - b.rho) + (a.theta - b.theta)*(a.theta - b.theta));
}


// Fonction générée par IA car trop longue à faire soi même en C++
std::vector<ParamPolaires> Clustering(const std::vector<double> &X,
                                      const std::vector<double>& Y,
                                      double threshold = 0.15) // seuil réduit
{
    int n = X.size();
    std::vector<ParamPolaires> points(n);
    
    for (int i = 0; i < n; i++) {
        points[i] = {X[i], Y[i]};
    }

    //  normalisation
    double max_rho = *std::max_element(X.begin(), X.end());
    double min_rho = *std::min_element(X.begin(), X.end());
    double rho_scale = std::max(std::abs(max_rho), std::abs(min_rho));
    double theta_scale = M_PI;

    auto distance_norm = [&](const ParamPolaires& a, const ParamPolaires& b) {
        double dr = (a.rho - b.rho) / rho_scale;
        double dt = (a.theta - b.theta) / theta_scale;
        return std::sqrt(dr*dr + dt*dt);
    };

    std::vector<bool> visited(n, false);
    std::vector<ParamPolaires> centers;

    for (int i = 0; i < n; i++) {
        if (visited[i]) continue;

        std::vector<ParamPolaires> cluster;
        cluster.push_back(points[i]);
        visited[i] = true;

        // regroupe les points proches
        for (int j = 0; j < n; j++) {
            if (!visited[j] && distance_norm(points[i], points[j]) < threshold) {
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
// Fin de la fonction générée par IA


ParamPolaires HoughDepuis2Point(int x1, int y1, int x2, int y2) {
    ParamPolaires parametres;

    double dx = x1 - x2;
    double dy = y1 - y2;

    parametres.theta = std::atan2(dx,-dy); 
    parametres.rho = x1 * std::cos(parametres.theta) + y1 * std::sin(parametres.theta);

    return parametres;
}

ParamPolaires ransac(const std::vector<int>& X, // ensemble de points
                   const std::vector<int>& Y,
                   ParamPolaires parametre, // parametre polaire
                   int nb_essais = 50) // nb de tests au hasard
                   // trouve une droite qui passe par 2 points et qui minimise l'ecart des inliers à la droite
{
    double rho = parametre.rho;
    double theta = parametre.theta;

    int xmax = *std::max_element(X.begin(), X.end());
    int ymax = *std::max_element(Y.begin(), Y.end());

    double diagonale = std::sqrt(xmax * xmax + ymax * ymax); // parce qu'on est sur des images
    double seuil_dist = std::max(2.0, diagonale / 200.0);

    std::vector<int> inliers;
    for (int i = 0; i < X.size(); i++) {
        double dist = std::abs(X[i] * std::cos(theta) + Y[i] * std::sin(theta) - rho);
        if (dist < seuil_dist) {
            inliers.push_back(i);
        }
    }

    if (inliers.size() < 2) {
        return ParamPolaires(rho, theta);
    }

    std::vector<double> Xi, Yi;
    for (int k = 0; k < inliers.size(); k++) {
        Xi.push_back(X[inliers[k]]);
        Yi.push_back(Y[inliers[k]]);
    }

    double meilleur_score = std::numeric_limits<double>::infinity();
    double meilleur_rho = rho;
    double meilleur_theta = theta;

    for (int essai = 0; essai < nb_essais; essai++) {

        int i1 = std::rand() % Xi.size();
        int i2 = std::rand() % Xi.size();

        while (i2 == i1) {
            i2 = std::rand() % Xi.size();
        }

        double x1 = Xi[i1];
        double y1 = Yi[i1];
        double x2 = Xi[i2];
        double y2 = Yi[i2];

        ParamPolaires d = HoughDepuis2Point(x1, y1, x2, y2);

        double rhoi = d.rho;
        double thetai = d.theta;

        double score = 0.0;
        for (int i = 0; i < Xi.size(); i++) {
            double dist = std::abs(Xi[i] * std::cos(thetai) + Yi[i] * std::sin(thetai) - rhoi);
            score += dist * dist;
        }

        if (score < meilleur_score) {
            meilleur_score = score;
            meilleur_rho = rhoi;
            meilleur_theta = thetai;
        }
    }

    return ParamPolaires(meilleur_rho, meilleur_theta);
}