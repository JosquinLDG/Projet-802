#include "traitement_image.cpp"
#include "fonctions_auxiliaires.cpp"

// Fichier central pour l'application de la méthode : contient les programmes relatifs à la méthode de Hough


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

    double diagonale = std::sqrt(xmax * xmax + ymax * ymax); // calcul de diagonal simple dans ce cas
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

        ParamPolaires d = HoughDepuis2Points(x1, y1, x2, y2);

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


// Trouve plusieurs droites dans l'espace de Hough
std::vector<ParamPolaires> trouve_droites(const std::vector<int>& X,
                                       const std::vector<int>& Y,
                                       int nblignes = 1, double seuil_clustering=0.1)
{
    std::cout << "Calcul des parametres polaires des droites" << std::endl;
    std::vector<ParamPolaires> resultats;

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


    std::vector<ParamPolaires> centres = Clustering(rho_max_trouve, theta_max_trouve, seuil_clustering);

    for(int i = 0; i < centres.size(); i++){
        resultats.push_back(ransac(X, Y, centres[i]));
    }


    return resultats;
}
