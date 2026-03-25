#include "Hough.cpp"


int main(){
    Image img = Image("imageM1.ppm");
    std::pair<std::vector<int>, std::vector<int>> coordonnees_points = img.get_coordinates(Pixel(250,0,0), 150); // attention à bien choisir la couleur à détecter
    std::vector<int> X = coordonnees_points.first;
    std::vector<int> Y = coordonnees_points.second;

    std::vector<ParamPolaires> droites_trouvees = trouve_droites(X, Y, 3, 0.1); 
    // La troisième valeur représente un nombre minimal de lignes à trouver
    // La quatrième un seuil de différence nécessaire au clustering.   0.05 pour bien séparer les droites, 0.15 pour en limiter le nombre

    std::vector<double> rhos, thetas;

    for(int i = 0; i < droites_trouvees.size(); i++){
        ParamPolaires droite_trouvee_i = droites_trouvees[i];
        rhos.push_back(droite_trouvee_i.rho);
        thetas.push_back(droite_trouvee_i.theta);
    }
    
    img.surligner_droite("test", rhos, thetas, Pixel(0,0,255)); // On peut choisir la couleur de surlignage



    return 0;
}