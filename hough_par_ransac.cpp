#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <utility>
#include "fonctions_auxiliaires.cpp"
#include "traitement_image.cpp"

// paramètres de la droite dans l'espace de Hough
struct ParamHough {
    double rho;
    double theta;
};

// Cette fonction prends en paramètre un vecteur de doubles ainsi qu'une valeur, puis retourne l'indice de la différence minimale entre l'une des valeur du vecteur, et la valeur val
int argmin_abs(const std::vector<double>& v, double val) {  
    std::vector<double> diffs(v.size());  // Initialisation du vecteur qui contiendra les différences

    // Boucle for qui calcule les dîtes différences

    for (int i = 0; i < v.size(); i++) { 
        diffs[i] = std::abs(v[i] - val);
    }
     
    // Fonction MinInd (à trouver dans un second fichier) qui renvoie l'indice de la valeur minimal
    MinInd res = trouve_min(diffs);
    return res.index;
}

// Méthode trouve_droite affûté à la classe ParamHough
ParamHough trouve_droite(const std::vector<double>& X,
                         const std::vector<double>& Y,
                         int nbligne = 1)
{
    int n = X.size(); // on affûte à "n" le nombre de points

    int nb = 10 * n;  // On discrètise 

    
    std::vector<double> thetas(nb); // initialisation du vecteur theta, qui prendra la taille de la discrétisation

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
        rhos[i] = -rhomax + i * (2 * rhomax) / (nb - 1);
    }

    std::vector<int> accumulateur(nb * nb, 0);

    for (int i = 0; i < n; i++) {
        double x = X[i];
        double y = Y[i];

        for (int argtheta = 0; argtheta < nb; argtheta++) {
            double rho = x * std::cos(thetas[argtheta])
                       + y * std::sin(thetas[argtheta]);

            int argrho = argmin_abs(rhos, rho);
            accumulateur[nb * argrho + argtheta] += 1;
        }
    }

    auto it = std::max_element(accumulateur.begin(), accumulateur.end());
    int argmax = it - accumulateur.begin();

    int argrhomax = argmax / nb;
    int argthetamax = argmax % nb;

    ParamHough res;
    res.rho = rhos[argrhomax];
    res.theta = thetas[argthetamax];

    return res;
};

// distance entre deux couleurs RGB
double distance_couleur(const Pixel& p1, const Pixel& p2) {
    double dr = p1._r - p2._r;
    double dg = p1._g - p2._g;
    double db = p1._b - p2._b;
    return std::sqrt(dr * dr + dg * dg + db * db);
};


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

            if (distance_couleur(p, fond) > tolerance) {
                X.push_back(j);
                Y.push_back(img._height - 1 - i);
            }
        }
    }
};

#include <utility> // déjà présent normalement
ParamHough HoughDepuis2Points(int x1, int y1, int x2, int y2) {
    ParamHough d;

    if (x1 == x2) {          // droite verticale : x = c
        d.theta = 0.0;
        d.rho = x1;
        return d;
    }

    if (y1 == y2) {          // droite horizontale : y = c
        d.theta = M_PI_2;
        d.rho = y1;
        return d;
    }

    double a = static_cast<double>(y2 - y1) / (x2 - x1);
    double b = y1 - a * x1;

    d.theta = atan(-1.0 / a);
    d.rho = b * sin(d.theta);

    return d;
}

//int main() {
//
//    // Test image
//    Image img("imgTresSimple.ppm");
//
//    std::vector<double> X;
//    std::vector<double> Y;
//
//    extraire_points_hors_fond(img, X, Y, 20.0);
//
//    std::cout << "Nombre de points extraits : " << X.size() << std::endl;
//
//    ParamHough d = trouve_droite(X, Y);
//
//    std::cout << "\nDroite trouvee dans l'espace de Hough :" << std::endl;
//    std::cout << "rho = " << d.rho << std::endl;
//    std::cout << "theta = " << d.theta << std::endl;
//
//    img.surligner_droite("resultat_hough", d.rho, d.theta);
//
    // Fonction teste
//
//    int x1 = 0, y1 = 10;
//    int x2 = 30, y2 = 0;
//
//    auto res = HoughDepuis2Points(x1, y1, x2, y2);
//
//    std::cout << "rho = " << res.rho << std::endl;
//    std::cout << "theta = " << res.theta << std::endl;
//
//    return 0;
//}
