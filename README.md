T.V.S.
===========================================

Trahison Virtuelle de la Sensibilité //////
-------------------------------------------

Alexis Pétard & Clément Catherine
Bruxelles, 2013

# tvs.py (python 2.7)
script dépendant de freenect qui permet de moduler une valeur en fonction de la densité de pixels et de leur proximité par rapport au centre du point de vision de la kinect.

prerequis = compiler freenect.py en local et déplacer le .so dans tvs/
> python setup.py build_ext --inplace


pour communiquer en série, utiliser pyserial :

   1. peut etre rajouter le baudrate et le timeout en arguments directement à la suite du directory
   2. lit un byte
   3. lit jusqu'à 10 bytes
   4. lit une ligne qui se termine pas un '\n' (attention, methode dangereuse)
   5. ferme le port


import serial
ser = serial.Serial()
ser.baudrate = 9600
ser.port = '/dev/ttyACM0'
ser.open()
ser.write(mouvement)


! insérer delay pour ne pas provoquer de collisions
! insérer une vérification d'accusé de réception


principal problème actuel : mauvaise gestion de libusb par linux. beaucoup de frames sont droppées
- essayer avec d'autres usb branchés (bidouille)
- attendre de voir avec les updates

#### À faire 
* la valeur doit être envoyée en série (ttyACM0) grâce au module pySerial
* gestion du protocole de communication

# tvs.ino (arduino)
script dépendant de la bibliothèque Stepper pour arduino qui permet de réceptionner une variable à travers un port série et de modifier sa vitesse et son amplitude en fonction de cette variable.

#### À faire 
* commande d'annulation d'urgence (peu importe la circonstance : arrêt des moteurs)
* meilleure réception des paquets de la part de tvs.ino
* meilleure transmission des paquets de la part de tvs.py
* (diviser la variable en deux :)
    * amplitude de respiration
    * vitesse
* trouver des moments pour éteindre le moteur afin d'éviter la surchauffe
