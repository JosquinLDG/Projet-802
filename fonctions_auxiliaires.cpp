#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <string>

// Fichier contenant des fonctions et structures auxiliaires mais nécessaires au bon fonctionnement du programme global


// Similaire à la fonction linspace de numpy sur python
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


// Renvoie le min d'un vecteur et son indice
std::pair<int, double> trouve_min(const std::vector<double>& v) {
    auto it = std::min_element(v.begin(), v.end());
    std::pair<int, double> res;
    res.first = it - v.begin();// .first = index
    res.second = *it;// .second = valeur

    return res;
}

// paramètres de la droite dans l'espace de Hough
struct ParamPolaires {
    double rho;
    double theta;
    ParamPolaires() : rho(), theta() {}
    ParamPolaires(double p1, double p2) : rho(p1), theta(p2) {}
};


ParamPolaires HoughDepuis2Points(int x1, int y1, int x2, int y2) {
    ParamPolaires parametres;

    double dx = x1 - x2;
    double dy = y1 - y2;

    parametres.theta = std::atan2(dx,-dy); 
    parametres.rho = x1 * std::cos(parametres.theta) + y1 * std::sin(parametres.theta);

    return parametres;
}


// get_rhos et get_thetas sont deux fonctions utiles pour simplifier l'implémentation de certains codes
// Permet d'extraire un vecteur de rhos d'un vecteur de coordonnées polaires
std::vector<double> get_rhos(std::vector<ParamPolaires> vecteur_polaire){
    std::vector<double> vecteur_rhos;
    for(int i = 0; i < vecteur_polaire.size(); i++){
        vecteur_rhos.push_back(vecteur_polaire[i].rho);
    }
    return vecteur_rhos;
}

std::vector<double> get_thetas(std::vector<ParamPolaires> vecteur_polaire){
    std::vector<double> vecteur_thetas;
    for(int i = 0; i < vecteur_polaire.size(); i++){
        vecteur_thetas.push_back(vecteur_polaire[i].theta);
    }
    return vecteur_thetas;
}

// retourne l'indice de la valeur de v la plus proche de val
int arg_closest(const std::vector<double>& v, double val) {
    std::vector<double> diffs(v.size());

    for (int i = 0; i < (int)v.size(); i++) {
        diffs[i] = std::abs(v[i] - val);
    }

    std::pair<int,double> res = trouve_min(diffs);
    return res.first;
}