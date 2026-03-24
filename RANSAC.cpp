#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <iostream>

struct ParamHough2 {
    double rho;
    double theta;
};

ParamHough2 trouve_rho_theta(double x1, double y1, double x2, double y2);

ParamHough2 ransac(const std::vector<double>& X, // ensemble de points
                   const std::vector<double>& Y,
                   double rho, // parametre polaire rho
                   double theta, // parametre polaire theta
                   int nb_essais = 50) // nb de tests au hasard
                   // trouve une droite qui passe par 2 points et qui minimise l'ecart des inliers à la droite
{
    double xmax = *std::max_element(X.begin(), X.end());
    double ymax = *std::max_element(Y.begin(), Y.end());

    double diagonale = std::sqrt(xmax * xmax + ymax * ymax);
    double seuil_dist = std::max(2.0, diagonale / 200.0);

    std::vector<int> inliers;
    for (int i = 0; i < X.size(); i++) {
        double dist = std::abs(X[i] * std::cos(theta) + Y[i] * std::sin(theta) - rho);
        if (dist < seuil_dist) {
            inliers.push_back(i);
        }
    }

    if (inliers.size() < 2) {
        ParamHough2 meilleur;
        meilleur.rho = rho;
        meilleur.theta = theta;
        return meilleur;
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

        ParamHough2 d = trouve_rho_theta(x1, y1, x2, y2);

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

    ParamHough2 meilleur;
    meilleur.rho = meilleur_rho;
    meilleur.theta = meilleur_theta;

    return meilleur;
}

ParamHough2 trouve_rho_theta(double x1, double y1, double x2, double y2)
{
    ParamHough2 d;

    double dx = x2 - x1;
    double dy = y2 - y1;

    d.theta = std::atan2(-dx, dy);
    d.rho = x1 * std::cos(d.theta) + y1 * std::sin(d.theta);

    return d;
}

//int main() {
//    std::srand(std::time(nullptr));
//
//    std::vector<double> X = {0, 10, 20, 30};
//    std::vector<double> Y = {10, 7, 3, 0};
//
//    double rho = 9.5;
//    double theta = 1.25;
//
//    ParamHough2 d2 = ransac(X, Y, rho, theta, 50);
//
//    std::cout << d2.rho << std::endl;
//    std::cout << d2.theta << std::endl;
//
//    return 0;
//}