/* 
Arduino Uno / RESPIRE / projet TVS
Utilisation de la bibliothèque Stepper pour contrôler le moteur

version alpha
eteindre = envoyer "0" en serie
### T.V.S. ###


*/

char recu[20];
String reponse;
int yeau = 0;
int i;


int vitesse = 60;
#include <Stepper.h>

int enA  = 3;  // Enable pin 1 on Motor Control Shield   
int enB  = 9;  // Enable pin 2 on Motor Control Shield   
int dirA = 2;  // Direction pin dirA on Motor Control Shield
int dirB = 8;  // Direction pin dirB on Motor Control Shield


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
      while ( Serial.available()>0 )      {
            recu[i] = Serial.read();
            reponse += recu[i];
            i++;
            delay(100);
      }
      if (i>0) {
            Serial.println(reponse);
            vitesse = reponse.toInt();
            if (vitesse < 120 && vitesse >= 60) {
              Serial.println("OK ca change");
              myStepper.setSpeed(vitesse);
            } else if (vitesse == 0) {
              Serial.print("OK on arrete");
              myStepper.setSpeed(vitesse);
              digitalWrite(enA,LOW);
              digitalWrite(enB,LOW);
            } else {
              Serial.println("pas bon"); 
            }
      }
}
