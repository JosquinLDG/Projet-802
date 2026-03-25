#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <string>

#include "traitement_image.cpp"
#include "fonctions_auxiliaires.cpp"

// Cette fonction prend en paramètre un vecteur de doubles ainsi qu'une valeur,
// puis retourne l'indice de la différence minimale entre l'une des valeurs
// du vecteur et la valeur val
int arg_closest(const std::vector<double>& v, double val) {
    std::vector<double> diffs(v.size());

    for (int i = 0; i < (int)v.size(); i++) {
        diffs[i] = std::abs(v[i] - val);
    }

    MinInd res = trouve_min(diffs);
    return res.index;
}

// Clustering simple : si deux couples (rho, theta) sont proches,
// on les considère comme représentant la même droite


// Trouve plusieurs droites dans l'espace de Hough
std::vector<ParamHough> trouve_droites(const std::vector<int>& X,
                                       const std::vector<int>& Y,
                                       int nblignes = 1)
{
    std::vector<ParamHough> resultats;

    int n = X.size();
    if (n == 0 || Y.size() != X.size()) {  // Optionnel
        return resultats;
    }

    int m = std::min(100,n*10);

    std::vector<double> thetas;

    thetas = linspace((double)-M_PI / 2.0, (double)M_PI / 2.0, m);

    double rhomax = 0.0;

    double rho;
    for (int i = 0; i < n; i++) {
        rho = std::sqrt(X[i] * X[i] + Y[i] * Y[i]);
        if (rho > rhomax) {
            rhomax = rho;
        }
    }


    std::vector<double> rhos(m);
    rhos = linspace(-rhomax, rhomax, m);

    std::vector<int> accumulateur(m * m, 0);

    double x;
    double y;

    int argrho;

    for (int i = 0; i < n; i++) {
        x = X[i];
        y = Y[i];

        for (int argtheta = 0; argtheta < m; argtheta++) {
            rho = x * std::cos(thetas[argtheta]) + y * std::sin(thetas[argtheta]);

            argrho = arg_closest(rhos, rho);
            accumulateur[m * argrho + argtheta] += 1;
        }
    }

    int nb_max = *std::max_element(accumulateur.begin(), accumulateur.end()); // fonction etrange pour trouver le max

    std::vector<int> argcandidats;
    double seuil = (double) nb_max * .9  / std::sqrt(nblignes); // .9 adaptable au besoin

    for (int i = 0; i < m*m; i++) {
        if (accumulateur[i] > seuil) {
            argcandidats.push_back(i);
        }
    }

    std::vector<int> argrho_candidats, argtheta_candidats;

    for (int i : argcandidats) {

        argrho_candidats.push_back(i/m);
        argtheta_candidats.push_back(i%m);

    }

    std::vector<double> rho_max_trouve;
    std::vector<double> theta_max_trouve;

    for(int indice : argrho_candidats){
        rho_max_trouve.push_back(rhos[indice]);
    }
    for(int indice : argtheta_candidats){
        theta_max_trouve.push_back(thetas[indice]);
    }


    std::vector<ParamHough> centres = Clustering(rho_max_trouve, theta_max_trouve);

    for(int i = 0; i < centres.size(); i++){
        resultats.push_back(ransac(X, Y, centres[i]));
    }


    return resultats;
}