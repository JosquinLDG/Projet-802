#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <utility>
#include <cstdlib>

// Structure Pixel, qui permet de stocker les valeurs r, g et b d'un pixel
struct Pixel {

    unsigned int _r, _g, _b;
    
    Pixel() : _r((unsigned int) 255), _g((unsigned int) 255), _b((unsigned int) 255) {}
    Pixel(unsigned int r, unsigned int g, unsigned int b) : _r(r), _g(g), _b(b) {}
    
    
    bool operator==(const Pixel other){ // Surcharge de l'opérateur de comparaison == pour comparer 2 Pixels entre eux
        if(other._r == _r && other._g == _g && other._b == _b){
            return true;
        }
        return false;
    }


    int distance(const Pixel other) const { // Distance de Manhattan entre la couleur de 2 Pixels
        return std::abs((int)_r - (int)other._r) + std::abs((int)_g - (int)other._g) + std::abs((int)_b - (int)other._b);
    }
};



// Structure Image: un tableau de pixel. On y stock l'information de coordonnées de chaque pixel.
struct Image {
    std::vector<std::vector<Pixel>> pixels;
    int _height;
    int _width;

    Image(unsigned int height, unsigned int width) // Constructeur à vide. Crée une image noire à partir de dimensions.
        : pixels(height, std::vector<Pixel>(width)), _height(height), _width(width) {}

        
    Image(std::string nomFichier) // Constructeur à partir du nom d'un fichier
        : Image(lire_ppm(nomFichier)) {}


    static std::string lire_token(std::ifstream& file) { // Une fonction qui permet de passer les lignes qui commencent par '#' et renvoie le premier "mot" suivant
        std::string token; // On initialise un mot à vide
        while (file >> token) { // Tant qu'il y a des choses à lire dans file
            if (token[0] == '#') { // Si la ligne commence par '#', on saute la ligne
                std::string ligne;
                std::getline(file, ligne); // ignorer le reste de la ligne
            } else {
                return token; // On renvoit le token si la ligne n'est pas un commentaire
            }
        }
        return "";
    }

    // méthode pour lire des fichiers en format .ppm
    static Image lire_ppm(const std::string& nomFichier) {
        std::ifstream file(nomFichier);
        if (!file) { // S'il y a une erreur, on affiche qu'il y a eu une erreur et on génère une image vide
            std::cerr << "Erreur ouverture fichier\n";
            return Image(0, 0);
        }

        // On récupère les informations importantes (sur les lignes sans '#') dans un ordre précis
        std::string format = lire_token(file); // format de l'image
        int width  = std::stoi(lire_token(file)); // Largeur de l'image
        int height = std::stoi(lire_token(file)); // Hauteur de l'image
        int maxVal = std::stoi(lire_token(file)); // Valeur maximale de l'image
        // Ces informations ne seront pas forcément toutes utiles, mais il est plus simple de toutes les traiter ainsi

        Image img(height, width); // On instancie une image noire de la bonne dimension

        for (int i = 0; i < height; i++) { // On modifie un à un tous les pixels de cette image d'après la lecture du fichier
            for (int j = 0; j < width; j++) {
                img.pixels[i][j]._r = std::stoi(lire_token(file));
                img.pixels[i][j]._g = std::stoi(lire_token(file));
                img.pixels[i][j]._b = std::stoi(lire_token(file));
            }
        }

        return img;
    }

    // méthode qui se sert de la méthode tracer_droite qui suit pour générer une nouvelle image, basée sur l'image d'origine de l'objet.
    // permet de surligner les droites détectées par 
    void surligner_droite(std::string const nom_nouveau_fichier, const std::vector<double> &rhos, const std::vector<double> &thetas, Pixel couleur = Pixel(255,0,0)){
        std::ofstream fichier(nom_nouveau_fichier + ".ppm");
        Image img = *this;

        for(int compteur = 0; compteur < rhos.size(); compteur++){
            std::cout << "rho " << compteur << " : " << rhos[compteur] << "  theta " << compteur << " : " << thetas[compteur] << std::endl;
        }

        std::cout << "Tracage de la droite" << std::endl;
        Image droite_tracee = tracer_droite(rhos, thetas, couleur); // on utilise la méthode tracer_droite pour générer une image avec des droites

        std::cout << "Conversion des pixel" << std::endl;

        for(int i = 0; i < _height; i++){ // on modifie les pixels de l'image là où la droite est dessinée par tracer_droite
            for(int j = 0; j < _width; j++){
                Pixel pixel = droite_tracee.pixels[i][j];
                if(pixel == couleur){
                    img.pixels[i][j]._r = couleur._r;
                    img.pixels[i][j]._g = couleur._g;
                    img.pixels[i][j]._b = couleur._b;
                }
            }
        }

        std::cout << "Ecriture du document" << std::endl;
        fichier << "P3\n";
        fichier << img._width << " " << img._height << "\n";
        fichier << 225 << "\n";
        for(int i = 0; i < _height; i++){
            for(int j = 0; j < _width; j++){
                fichier << img.pixels[i][j]._r << "\n";
                fichier << img.pixels[i][j]._g << "\n";
                fichier << img.pixels[i][j]._b << "\n";
            }
        }
    }

    // méthode pour générer une image avec des droites
    Image tracer_droite(const std::vector<double> &rhos, const std::vector<double> &thetas, Pixel couleur = Pixel(255,0,0)){
        Image droite_tracee = Image(_height, _width); // initialisation de droite_tracee

        int diag = (int) std::sqrt(_width*_width + _height*_height); //longueur de la diagonale
        double pi = 2 * std::acos(0.0);

        int i;
        int j;

        double a;
        double b;

        double theta;
        double rho;

        for (int k = 0; k < rhos.size(); k++){
            
            theta = thetas[k];
            rho = rhos[k];

            if (std::abs(std::cos(theta)) > std::abs(std::sin(theta))){//(theta > - pi/4 && theta < pi/4){
                a =  - std::sin(theta) / std::cos(theta);
                b = rho / std::cos(theta);

                for(int i = 0; i < _height; i++){

                    int j = (int) (a*i + b + 0.5); // le "+0.5" permet d'arrondir convenablement, pas simplement au nombre inferieur
                    int l = 0;

                    do {
                        if (j + l >= 0 && j + l < _width){ //verification que la colonne du pixel appartient à l'image 
                            droite_tracee.pixels[_height - 1 - i][j+l] = couleur;
                        }
                        if (j - l >= 0 && j - l < _width){ //verification que la colonne du pixel appartient à l'image 
                        droite_tracee.pixels[_height - 1 - i][j-l] = couleur;
                        }
                        l++;
                    }
                    while(l < diag / 200); // on définit une valeur arbitraire d'épaisseur de ligne basee sur la diagonale
                }

            }

            else{
                a =  - std::cos(theta) / std::sin(theta);
                b = rho / std::sin(theta);

                for(int j = 0; j < _width; j++){
                    int i = (int) (a*j + b + 0.5);
                    int l = 0;


                    do  {
                        int y1 = _height - 1 - i + l; // le point au dessus de la diagonale auquel on s'intéresse
                        int y2 = _height - 1 - i - l; // le point en dessous de la diagoale auquel on s'intéresse

                        if(y1 >= 0 && y1 < _height){
                            droite_tracee.pixels[y1][j] = couleur;
                        }
                        if(y2 >= 0 && y2 < _height){
                            droite_tracee.pixels[y2][j] = couleur;
                        }
                        l++;
                        }
                    while(l < diag / 200);
                }
            }
        }

        return droite_tracee;
    }

    void afficher_donnees(){ // Permet d'afficher le contenu d'une image. Chaque pixel est représenté sous forme (r,g,b)
            for (auto ligne : pixels){
            for (auto pixel : ligne) {
                std::cout << pixel._r << " " << pixel._g << " " << pixel._b << "   ";
            }
            std::cout << std::endl;
        }
    }

    // renvoie les coordonnées de tous les points d'une certaine couleur ou de couleur proche 
    std::pair<std::vector<int>, std::vector<int>> get_coordinates(Pixel couleur = Pixel(255,255,255), int seuil = 50){ // seuil pour décréter qu'une couleur est assez proche ou non
        // renvoie un vecteur de toutes les ordonnées x et un de toutes les abscisses y dans une paire de vecteur
        std::vector<int> vecteurX;
        std::vector<int> vecteurY;

        int i = 0;
        int j;
        for(auto ligne : pixels){
            j = 0;
            for(auto pixel : ligne){
                if(pixel.distance(couleur) <= seuil){ // après surcharge 
                    vecteurX.push_back(j); // on ajoute l'ordonnée du point au vecteur des ordonnées
                    vecteurY.push_back(_height - 1 - i); // on ajoute l'abscice du point au vecteur des abscisses
                }
                j++;
            }
            i++;
        }
        std::pair<std::vector<int>, std::vector<int>> coordonnees = {vecteurX, vecteurY};
        return  coordonnees;
    }
};