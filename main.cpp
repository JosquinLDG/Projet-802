#include "Houg_polaire.cpp"


std::vector<double> donnees_test_clustering_X = {
    -0.49186552, -0.46013226, -0.46013226, -0.428399,   -0.07933315, -0.04759989,
 -0.01586663,  0.01586663,  0.04759989,  0.07933315,  0.11106641,  1.1265307,
  1.15826396,  1.09479744,  1.1265307,   1.15826396 , 1.06306418,  1.09479744 
};

std::vector<double> donnees_test_clustering_Y = {
-2.32816779, -2.32816779, -1.39690068, -1.39690068, 13.5033732,  14.43464031,
 14.43464031, 15.36590743, 15.36590743 ,16.29717455, 16.29717455, 20.95351013,
 20.95351013 ,21.88477725 ,22.81604436, 22.81604436, 23.74731148, 23.74731148
};



int main(){
    Image img = Image("test_500x500.ppm");
    std::pair<std::vector<int>, std::vector<int>> coordonnees_points = img.get_coordinates(Pixel(250,250,250), 150);
    std::vector<int> X = coordonnees_points.first;
    std::vector<int> Y = coordonnees_points.second;

    std::vector<ParamHough> droites_trouvees = trouve_droites(X, Y, 2);

    std::vector<double> rhos, thetas;

    for(int i = 0; i < droites_trouvees.size(); i++){
        ParamHough droite_trouvee_i = droites_trouvees[i];
        rhos.push_back(droite_trouvee_i.rho);
        thetas.push_back(droite_trouvee_i.theta);
    }
    
    img.surligner_droite("test", rhos, thetas, Pixel(0,0,0));



    return 0;
}