# Robot3D
Configuration requise :
- OpenCV 3.2.0
- Unity3D 2018.3.0
- Qt 5.9.5 (Version utilisée par Qt Creator 4.5.2)

Lancement des programmes :
- Se placer à la racine du projet
- Compiler le serveur avec la commande mkdir build && cd build && qmake -qt=5 ../reseautech/ && make
- Lancer le serveur avec la commande ./reseau_tech
  Sous Unity3D : ouvrir le projet Simulation_Robot puis cliquer sur Run
- Si la compilation échoue, éditer le fichier Robot3D/reseautech/reseautech.pro et inclure le chemin d'installation OpenCV
- Si le robot se perd dès le début, relancer le serveur ainsi que Run sous Unity3D
Commandes clavier :
- (français) Z, S, Q, D (anglais) W, A, S, D: respectivement avancer/reculer et aller à gauche/droite
- Espace : Changer la vue robot/latérale
- P : Enregistre les images vue par le robot (Robot3D/SimulationRobot/left.jpg et right.jpg)
Utilisation de Unity :
Vous contrôlerez un cylindre qui, une fois déplacé avec Z,Q,S,D (respectivement en anglais W, A, S, D) sera suivi par le robot. Le robot restera à 1m de distance de l’objet. 

![Figure 1](https://github.com/Swwitcher/Robot3D/tree/master/Image_README/Capture2.PNG?raw=true)
figure 1: Capture d’écran de la fenêtre de simulation avec le sol (vert),
 le (rouge), le robot (gris) et le cylindre (noir).
 
La vue du robot par défaut est celle de la caméra gauche.

![Figure 2](https://github.com/Swwitcher/Robot3D/tree/master/Image_README/Capture.PNG?raw=true)
figure 2: Capture d’écran de la vue du robot.
 
Une fois la touche espace utilisé, nous passons à la vue latérale.

![Figure 3](https://github.com/Swwitcher/Robot3D/tree/master/Image_README/Capture4.PNG?raw=true)
figure 3: Capture d’écran de la vue latérale.
 
 
Lors d’une déconnexion du client (volontaire ou accidentelle), le serveur se déconnecte automatiquement. Pour réutiliser la simulation, il faut à nouveau lancer le serveur (sans compiler une nouvelle fois), puis “Run” sur Unity. 
 
Auteurs : UNG Pierre, EL HOULALI Théo, LHERITIER Alexis, GRUILLOT Ewan

