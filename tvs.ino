/* 

Alexis Pétard
http://elaxis.fr
Bruxelles, 2014

Arduino Uno / RESPIRE / projet TVS
Utilisation de la bibliothèque Stepper pour contrôler le moteur

version alpha
eteindre = envoyer "0" en serie
### T.V.S. ###


*/

// Variable pour la communication série
char recu[20];
String reponse;
int i; // Pour attraper tous les caractères
int vitesse = 60; // Vitesse de base
#include <Stepper.h>

int enA  = 3;  // Enable pin 1
int enB  = 9;  // Enable pin 2
int dirA = 2;  // Direction pin dirA
int dirB = 8;  // Direction pin dirB


const int stepsPerRevolution = 200;  

// Initialisation de la classe Stepper
Stepper myStepper(stepsPerRevolution, dirA, dirB);            

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino OK");
  myStepper.setSpeed(vitesse); // 60 < n < ~120
  // Allumage du moteur
  pinMode(enA, OUTPUT);
  digitalWrite (enA, HIGH);
  pinMode(enB, OUTPUT);
  digitalWrite (enB, HIGH);  
  Serial.println("Moteur OK");
}

void loop() {
      myStepper.step(stepsPerRevolution*6);
      myStepper.step(-stepsPerRevolution*6);
      Serial.println("Moteur GO");
      reponse = 0;
      i = 0 ;
      while ( Serial.available()>0 ) {
            recu[i] = Serial.read();
            reponse += recu[i];
            i++;
            delay(100);
      }
      if (i>0) {
            Serial.println(reponse);
            vitesse = reponse.toInt(); // Conversion string to int dangereuse
            if (vitesse < 120 && vitesse >= 60) {
              Serial.println("OK ca change");
              myStepper.setSpeed(vitesse);
            } else if (vitesse == 0) { // Envoi d'un 0 => Arret (definitif) du moteur
              Serial.print("OK on arrete");
              myStepper.setSpeed(vitesse);
              digitalWrite(enA,LOW); // Arret bobine A
              digitalWrite(enB,LOW); // Arret bobine B
            } else {                  // Envoi d'un mauvais numéro
              Serial.println("pas bon"); // Rien ne se passe
            }
      }
}
