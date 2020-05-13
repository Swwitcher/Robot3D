# Robot3D
Configuration requises : 
- OpenCV 3.2.0
- Unity3D 2018.3.0
- Qt 5.9.5 (Version utilisée par Qt Creator 4.5.2)

Lancement des programmes : 
- Se placer à la racine du projet
- Compiler le serveur avec la commande   mkdir build && cd build && qmake -qt=5 ../reseautech/ && make
- Lancer le serveur avec la commande     ./reseau_tech
- Sous Unity3D : ouvrir le projet Simulation_Robot puis cliquer sur Run
- Si la compilation échoue, éditer le fichier Robot3D/reseautech/reseautech.pro et inclure le chemin d'installation OpenCV
- Si le robot se perd dès le début, relancer le serveur ainsi que Run sous Unity3D 

Commandes clavier : 
- (français) Z, S, Q, D (anglais) W, A, S, D:  respectivement avancer/reculer sur l'axe Z et aller à gauche/droite sur l'axe X
- Espace : Changer de vue (vue robot / vue laterale) 
- P : Enregistre les images vue par le robot (Robot3D/SimulationRobot/left.jpg et right.jpg)
