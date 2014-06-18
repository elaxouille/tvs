/*
Elaxis
180614
Utilisation de la bibliothèque Stepper pour contrôler le moteur
version 1.2alpha
*/

// Variable pour la communication série
int buf[3];
int last = 6;
int protocole=0;
int minimum = 1;
int maximum = 120;
int respirationNormale = 6;
int vitesse = 6; // Vitesse de base
#include <Stepper.h>
float amplitude = 1.5;
int enA = 3; // Enable pin 1
int enB = 9; // Enable pin 2
int dirA = 2; // Direction pin dirA
int dirB = 8; // Direction pin dirB
boolean shut = false; // a utiliser
const unsigned int MAX_INPUT = 50;

const int stepsPerRevolution = 200;

// Initialisation de la classe Stepper
Stepper myStepper(stepsPerRevolution, dirA, dirB);


void setup() {
  Serial.begin(9600);
  Serial.println("[ARD]\tArduino OK");
  // Allumage du moteur
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  Serial.println("[ARD]\tFin du setup");
}

void loop() {
  switch(protocole) {
  case 0:
    while (Serial.available() == 0) {
      // rien
    }
    allumageMoteur();
      protocole++;
    
  default:
    if (Serial.available() > 0) {
      char digits[] = {' ',' '};
      while (!Serial.available());
      Serial.readBytesUntil('\n', digits, 2);
      int incomingValue = atoi(digits);
      Serial.println(incomingValue);
      if (incomingValue == 0) {
        Serial.println("[ARD]\tReception d'un 0");
        vitesse = last;
      } else if (incomingValue == 99) {
        angoisse();
      } else {
        vitesse=incomingValue;
        changeVitesse(vitesse);
        respire();
      }
      last=vitesse;
      Serial.flush();
    } else {
      Serial.println("[ARD]\tPas de communication");
    }
  }
}

void allumageMoteur() {
  digitalWrite (enA, HIGH);
  digitalWrite (enB, HIGH);
  shut=false;
}
void extinctionMoteur() {
  digitalWrite(enA,LOW);
  digitalWrite(enB,LOW);
  shut=true;
}
void changeVitesse(int v) {
  Serial.print("[ARD]\tCHANGEMENT-VITESSE : \t");
  Serial.println(v);
  myStepper.setSpeed(v);
}

void respire() {
  inspire();
  expire();
  Serial.print("[ARD]\tTour fait, v : ");
  Serial.println(vitesse);
}
void inspire() {
  myStepper.step(stepsPerRevolution*amplitude);
}
void expire() {
  myStepper.step(-stepsPerRevolution*amplitude);
}

void angoisse() { // Pas pour l'instant
  Serial.println("\033[0;34m[ARD]\033[0m\tTOP ANGOISSE");
  allumageMoteur();
  for (int u = 0; u <= 6; u++) {
    Serial.print("\033[0;34m[ARD]\033[0m\tANGOISSE");
    changeVitesse(99);
    respire();
    Serial.println("\033[0;34m[ARD]\033[0m\tTour Angoisse : ");
    Serial.println(u);
  }
  myStepper.step(stepsPerRevolution*amplitude);
  changeVitesse(6);
  myStepper.step(-stepsPerRevolution*amplitude);
  Serial.println("\033[0;34m[ARD]\033[0m\tFIN ANGOISSE");
  Serial.flush();
}
