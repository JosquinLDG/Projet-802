#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <string>
#include "indice(max:min).cpp"
#include "traitement_image.cpp"




// clang++ -std=c++17 TP5.cpp -o TP5 && ./TP5∏"


// paramètres de la droite dans l'espace de Hough
struct ParamHough {
    double rho;
    double theta;
};

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
std::vector<ParamHough> clustering_simple(const std::vector<double>& rho_candidats,
                                          const std::vector<double>& theta_candidats,
                                          double seuil_rho = 10.0,
                                          double seuil_theta = 0.1)
{
    std::vector<ParamHough> centres;


    for (int i = 0; i < (int)rho_candidats.size(); i++) {
        double rho = rho_candidats[i];
        double theta = theta_candidats[i];

        bool deja_proche = false;

        for (int j = 0; j < (int)centres.size(); j++) {
            if (std::abs(centres[j].rho - rho) < seuil_rho &&
                std::abs(centres[j].theta - theta) < seuil_theta) {
                deja_proche = true;
                break;
            }
        }

        if (!deja_proche) {
            ParamHough p;
            p.rho = rho;
            p.theta = theta;
            centres.push_back(p);
        }
    }

    return centres;
}

// Trouve plusieurs droites dans l'espace de Hough
std::vector<ParamHough> trouve_droites(const std::vector<double>& X,
                                       const std::vector<double>& Y,
                                       int nblignes = 2)
{
    std::vector<ParamHough> resultats;

    int n = X.size();
    if (n == 0 || Y.size() != X.size()) {
        return resultats;
    }

    int nb = 10 * n;
    if (nb < 2) nb = 2;

    std::vector<double> thetas(nb);

    double theta_min = -M_PI / 2.0;
    double theta_max =  M_PI / 2.0;

    for (int i = 0; i < nb; i++) {
        thetas[i] = theta_min + i * (theta_max - theta_min) / (nb - 1);
    }

    double rhomax = 0.0;
    for (int i = 0; i < n; i++) {
        double rho = std::sqrt(X[i] * X[i] + Y[i] * Y[i]);
        if (rho > rhomax) {
            rhomax = rho;
        }
    }

    std::vector<double> rhos(nb);
    for (int i = 0; i < nb; i++) {
        rhos[i] = -rhomax + i * (2.0 * rhomax) / (nb - 1);
    }

    std::vector<int> accumulateur(nb * nb, 0);

    for (int i = 0; i < n; i++) {
        double x = X[i];
        double y = Y[i];

        for (int argtheta = 0; argtheta < nb; argtheta++) {
            double rho = x * std::cos(thetas[argtheta])
                       + y * std::sin(thetas[argtheta]);

            int argrho = arg_closest(rhos, rho);
            accumulateur[nb * argrho + argtheta] += 1;
        }
    }

    int nb_max = *std::max_element(accumulateur.begin(), accumulateur.end());

    std::vector<int> argcandidats;
    double seuil = 0.6 * nb_max / nblignes;

    for (int i = 0; i < (int)accumulateur.size(); i++) {
        if (accumulateur[i] > seuil) {
            argcandidats.push_back(i);
        }
    }

    std::vector<double> rho_candidats;
    std::vector<double> theta_candidats;

    for (int i = 0; i < (int)argcandidats.size(); i++) {
        int idx = argcandidats[i];

        int argrho = idx / nb;
        int argtheta = idx % nb;

        rho_candidats.push_back(rhos[argrho]);
        theta_candidats.push_back(thetas[argtheta]);
    }

    resultats = clustering_simple(rho_candidats, theta_candidats);

    return resultats;
}

// distance entre deux couleurs RGB


// extraction adaptable : on garde tous les pixels assez différents du fond
void extraire_points_hors_fond(const Image& img,
                               std::vector<double>& X,
                               std::vector<double>& Y,
                               double tolerance = 20.0)
{
    Pixel fond = img.pixels[0][0];  // on suppose que le fond est la couleur du coin haut-gauche

    for (int i = 0; i < img._height; i++) {
        for (int j = 0; j < img._width; j++) {
            const Pixel& p = img.pixels[i][j];

            if (p.distance(fond) > tolerance) {
                X.push_back(j);
                Y.push_back(img._height -1-i);
            }
        }
    }
}

ParamHough HoughDepuis2Point(int x1, int y1, int x2, int y2) {
    ParamHough parametres;

    double dx = x1 - x2;
    double dy = y1 - y2;

    parametres.theta = std::atan2(dx,-dy); 
    parametres.rho = x1 * std::cos(parametres.theta) + y1 * std::sin(parametres.theta);

    return parametres;
}

int main() {
    Image img("imageAvecDeuxSegments.ppm");

    std::vector<double> X;
    std::vector<double> Y;

    extraire_points_hors_fond(img, X, Y, 20.0);

    std::cout << "Nombre de points extraits : " << X.size() << std::endl;

    // On cherche ici plusieurs droites
    std::vector<ParamHough> droites = trouve_droites(X, Y, 2);

    std::cout << "\nDroites trouvees dans l'espace de Hough :" << std::endl;

    for (int i = 0; i < (int)droites.size(); i++) {
        std::cout << "\nDroite " << i + 1 << " :" << std::endl;
        std::cout << "rho = " << droites[i].rho << std::endl;
        std::cout << "theta = " << droites[i].theta << std::endl;

        img.surligner_droite("resultat_hough_" + std::to_string(i + 1),
                             droites[i].rho,
                             droites[i].theta);
    }

    // Test de la fonction HoughDepuis2Points
    int x1 = 0, y1 = 10;
    int x2 = 30, y2 = 0;

    ParamHough res = HoughDepuis2Point(x1, y1, x2, y2);

    std::cout << "\nTest HoughDepuis2Points :" << std::endl;
    std::cout << "rho = " << res.rho << std::endl;
    std::cout << "theta = " << res.theta << std::endl;

    return 0;
}