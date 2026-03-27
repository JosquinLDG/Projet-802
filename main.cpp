#include "Hough.cpp"

// Le programme tout entier ne nécessite que de faire tourner celui-ci


int main(){
    Image img = Image("imageAvecDeuxSegments.ppm");

    std::pair<std::vector<int>, std::vector<int>> coordonnees_points = img.get_coordinates(Pixel(255,255,255), 150); // attention à bien choisir la couleur à détecter
    std::vector<int> X = coordonnees_points.first;
    std::vector<int> Y = coordonnees_points.second;

    std::vector<ParamPolaires> droites_trouvees = trouve_droites(X, Y, 3, 0.1); 
    // La troisième valeur est une estimation du nombre de droites à trouver (fonctionne même avec une très mauvaise estimation)
    // La quatrième un seuil de différence nécessaire au clustering.   0.05 pour bien séparer les droites, 0.15 pour en limiter le nombre
    
    img.surligner_droite("imageAvecDeuxSegments_surlignes", get_rhos(droites_trouvees), get_thetas(droites_trouvees), Pixel(255,0,0)); // On peut choisir la couleur de surlignage



    // Pour l'exemple, on refait les mêmes opérations sur plusieurs images

    std::cout << std::endl;

    Image img2 = Image("diagonale_avec_bruit.ppm");
    coordonnees_points = img2.get_coordinates(Pixel(0,0,0), 150);
    X = coordonnees_points.first;
    Y = coordonnees_points.second;
    droites_trouvees = trouve_droites(X, Y, 3, 0.1); 
    img2.surligner_droite("imageBruit_surligne", get_rhos(droites_trouvees), get_thetas(droites_trouvees), Pixel(255,0,0));

    std::cout << std::endl;

    Image img3 = Image("imageM1.ppm");
    coordonnees_points = img3.get_coordinates(Pixel(255,0,0), 150);
    X = coordonnees_points.first;
    Y = coordonnees_points.second;
    droites_trouvees = trouve_droites(X, Y, 3, 0.1); 
    img3.surligner_droite("imageM1_surligne", get_rhos(droites_trouvees), get_thetas(droites_trouvees), Pixel(0,0,0));

    std::cout << std::endl;

    Image img4 = Image("testComplet.ppm");
    coordonnees_points = img4.get_coordinates(Pixel(0,0,130), 150);
    X = coordonnees_points.first;
    Y = coordonnees_points.second;
    droites_trouvees = trouve_droites(X, Y, 3, 0.1); 
    img4.surligner_droite("test_complet_surligne", get_rhos(droites_trouvees), get_thetas(droites_trouvees), Pixel(255,0,255));

    return 0;
}