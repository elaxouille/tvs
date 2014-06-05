tvs
===

Trahison Virtuelle de la Sensibilité

# tvs.py (python 2.7)
script dépendant de freenect qui permet de moduler une valeur en fonction de la densité de pixels et de leur proximité par rapport au centre du point de vision de la kinect.

principal problème actuel : mauvaise gestion de libusb par linux.

### À faire 
* la valeur doit être envoyée en série (ttyACM0) grâce au module pySerial
* gestion du protocole de communication

# tvs.ino (arduino)
script dépendant de la bibliothèque Stepper pour arduino qui permet de réceptionner une variable à travers un port série et de modifier sa vitesse et son amplitude en fonction de cette variable.

### À faire 
* protocole de lancement, principalement pour un calibrage, avec :
    1. allumage bobines
    2. verification à vide : vitesse minimale
    3. verification à vide : vitesse maximale
    4. mise en position de départ
    5. attente avant confirmation pour départ deuxieme calibrage (fixation des fils)
    6. deuxieme calibrage après fixation
        1. position minimale
        2. position maximale
        3. vitesse minimale, amplitude minimale
        4. vitesse maximale, amplitude maximale (bref instant)
        5. vitesse maximale, amplitude minimale (pour les tremblements, la resistance du moteur, de ses fixations, et de fils)
    7. infirmation = repetition des etapes
    8. confirmation = allumage de la kinect
    9. test de fonctionnement de la kinect
    10. test de communication entre kinect et arduino
    11. processus d'enclenchement du réveil
* receptionner la variable à un meilleur rythme
* diviser la variable en deux :
    * amplitude de respiration
    * vitesse
* trouver une meilleur solution pour gérer le changement de vitesse et la perturbation des cycles de respiration
* tout en gardant une cohérence dans le positionnement
* trouver des moments pour éteindre le moteur afin d'éviter la surchauffe
