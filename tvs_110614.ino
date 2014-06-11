/* 
 Elaxis
 110614
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
int enA  = 3;  // Enable pin 1
int enB  = 9;  // Enable pin 2
int dirA = 2;  // Direction pin dirA
int dirB = 8;  // Direction pin dirB
boolean shut = true;
const unsigned int MAX_INPUT = 50;

const int stepsPerRevolution = 200;  

// Initialisation de la classe Stepper
Stepper myStepper(stepsPerRevolution, dirA, dirB);            


void setup() {
  Serial.begin(9600);
  Serial.println("Arduino OK");
  // Allumage du moteur
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  allumageMoteur();
}

void process_data(const char* data) {
  vitesse = atoi(data);
  Serial.print("RECU : ");
  Serial.println(vitesse);
  buf[sizeof(buf)] += vitesse;
}

void processIncomingByte(const byte inByte) {
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;
  switch(inByte) {
    case '\n':
      input_line [input_pos]= 0;
      process_data (input_line);
      input_pos = 0;
      break;
    case '\r':
      break;
    default:
      if (input_pos < (MAX_INPUT -1)) 
        input_line [input_pos++] = inByte;
      break;
  }
}

void loop() {
  switch(protocole) {
  case 0:
    Serial.println("ARDUINO PRETE");
    protocole++;
  case 1: // GENERAL
    if (Serial.available() > 0) {
      processIncomingByte(Serial.read());
      changeVitesse(vitesse);
      respire();
      buf[sizeof(buf)] = 0;
    }
  default:
    break;
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
  Serial.print("CHANGEMENT-VITESSE : \t");
  Serial.println(v);
  myStepper.setSpeed(v);
}

void respire() {
  inspire();
  expire();
}
void inspire() {
  myStepper.step(stepsPerRevolution*amplitude);
}
void expire() {
  myStepper.step(-stepsPerRevolution*amplitude);
}

void angoisse() {
  Serial.println("\033[0;34m[ARD]\033[0m\tTOP ANGOISSE");
  allumageMoteur();
  for (int u = 0; u <= 20; u++) {
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
}
