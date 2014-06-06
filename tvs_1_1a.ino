/* 

Elaxis
060614

Arduino Uno / RESPIRE / projet TVS
Utilisation de la bibliothèque Stepper pour contrôler le moteur

version 1.1alpha
Effectue une série de trois tests puis s'éteint.
Premier test lent
Second test rapide
Troisieme test incrémentation de vitesse 5 < x < 120 pour x+=5 
### T.V.S. ###

### À faire 
COMMUNICATION KINECT !IMPORTANT!

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


*/

// Variable pour la communication série
int protocole=0;
int minimum = 0;
int maximum = 1000;
char recu[20];
String reponse;
int i; // Pour attraper tous les caractères
int vitesse = 6; // Vitesse de base
#include <Stepper.h>
float amplitude = 1.5;
int enA  = 3;  // Enable pin 1
int enB  = 9;  // Enable pin 2
int dirA = 2;  // Direction pin dirA
int dirB = 8;  // Direction pin dirB
boolean shut = true;


const int stepsPerRevolution = 200;  

// Initialisation de la classe Stepper
Stepper myStepper(stepsPerRevolution, dirA, dirB);            

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino OK");
  changeVitesse(vitesse); // 60 < n < ~120
  // Allumage du moteur
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
}

void loop() {
  switch(protocole) {
    case 0:
      Serial.println("Step 1");
      testLent();
      Serial.println("Step 1 OK");
      protocole++;
    case 1:
      Serial.println("Step 2");
      testRapide();
      Serial.println("Step 2 OK");
      protocole++;
    case 2:
      Serial.println("Step 3");
      testAccel();
      Serial.println("Step 3 OK");
      protocole++;
    default:
      break;
  }
/*
      ###### COMMUNICATION SERIE A TRAVAILLER /!\
      
      Serial.print("Une respiration, ");
      Serial.print(amplitude);
      Serial.print(" tours, ");
      Serial.print(vitesse);
      Serial.println(" rotations par minute");
      myStepper.step(stepsPerRevolution*amplitude);
      myStepper.step(-stepsPerRevolution*amplitude);
      reponse = 0;
      i = 0 ;
      while ( Serial.available()>0 ) {
            recu[i] = Serial.read(); // Ecoute du port série
            reponse += recu[i];      // Rajout des bits au fur et a mesure
            i++;
            delay(1000);
      }
      if (i>0) {
            Serial.print("Message recu : ");
            Serial.println(reponse);
            vitesse = reponse.toInt(); // Conversion string to int dangereuse (utiliser isDigit etc.)
            if (vitesse < maximum && vitesse >= minimum) {
              Serial.println("OK ca change");
              changeVitesse(vitesse);
            } else if (vitesse == 0) { // Envoi d'un 0 => Arret (definitif) du moteur
              Serial.print("OK on arrete");
              extinctionMoteur;
            } else {                  // Envoi d'un mauvais numéro
              Serial.println("trop élevé ou trop faible"); // Rien ne se passe
            }
            switch(vitesse) {
              case 0:
               extinctionMoteur;
               break; 
              default:
               changeVitesse(vitesse);
               break;
            }

      }
*/
}

void allumageMoteur() {
  digitalWrite (enA, HIGH);
  digitalWrite (enB, HIGH);
  Serial.println("------Moteur ON");
  shut=false;
}
void extinctionMoteur() {
  digitalWrite(enA,LOW);
  digitalWrite(enB,LOW);
  Serial.println("------Moteur OFF");
  shut=true; 
}
void changeVitesse(int v) {
  myStepper.setSpeed(v);
}

void testLent() {
  Serial.println("---Initialisation du test");
  allumageMoteur();
  changeVitesse(6);
  myStepper.step(stepsPerRevolution*amplitude);
  Serial.println("------Half-Way");
  myStepper.step(-stepsPerRevolution*amplitude);
  extinctionMoteur();
  delay(6000);
}
void testRapide() {
  Serial.println("---Initialisation du test");
  allumageMoteur();
  changeVitesse(100);
  myStepper.step(stepsPerRevolution*amplitude);
  Serial.println("------Half-Way");
  myStepper.step(-stepsPerRevolution*amplitude);
  extinctionMoteur();
  delay(6000);

}
void testAccel() {
  Serial.println("---Initialisation du test");
  allumageMoteur();
  for(int o = 5; o < 120; o+=5) {
    changeVitesse(o);
    myStepper.step(stepsPerRevolution*amplitude);
    Serial.println("------Half-Way");
    myStepper.step(-stepsPerRevolution*amplitude);
  }
  extinctionMoteur();
  delay(6000);
}
