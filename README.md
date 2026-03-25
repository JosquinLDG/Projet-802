# Projet-802

Ce projet est contenu dans plusieurs fichiers séparés.  

Le programme à compiler est le programme main.cpp .  
Celui-ci se base sur le programme Hough.cpp, et ces deux programmes s'appuient sur des structures et méthode définies dans traitement_image.cpp et dans fonctions_auxiliaires.cpp .  

Une fonction Clustering est définie dans le fichier fonctions_auxiliaires.cpp ; elle a été générée par IA puisque cela aurait été un pan entier de travail en plus.


A l'exécution, ce projet permet de détecter des droites sur une image en format .ppm et de les surligner. Le résultat se lit dans un nouveau fichier .ppm créé automatiquement. Le nom de ce fichier dépend des instructions données dans la fonction main. 

En Bref:
main : pour faire tourner le programme global  

traitement_image : 
- définition de la classe Pixel.
- définition de la classe Image. Elle permet de lire une image, de récupérer les coordonées de pixels cibles de cette image, et de tracer des droites sur une nouvelle image.cpp d'après des coordonnées polaires de droite données.  

Hough : définition de l'algorithme appelé dans le main pour déterminer les coefficients de droites à surligner  

fonctions_auxiliaires : définitions de fonctions et de classes utiles au fonctionnement de Hough
