# Projet-802

Ce projet est contenu dans plusieurs fichiers séparés.  

♥ Hough.cpp :  
► structure Pixel  
► structure Image :  
- lire_token : méthode permettant de lire le premier token (~ mot) d'un texte en ignorant les lignes qui commencent par #. Le token renvoyé par la fonction est supprimé du texte lu. Appeler n fois la fonction avec le même texte permet de lire les n premiers token du texte (excepté les lignes en #).  
- lire_ppm : une fonction pour lire les fichiers .ppm et les convertir en objet Image. Utilise la méthode lire_token pour traiter les .ppm .  
- tracer_droite : crée et renvoie un objet Image avec une droite tracée selon des coordonnées polaires
- surligner_droite : crée un nouveau fichier .ppm à partir d'un autre fichier déjà existant et change les valeurs des pixels concernées selon la fonction tracer_droite
- afficher_données : fonction utile lors du développement pour visualiser un objet Image
- 
