/*
   Code for nano with floating level switch and hc-sr04
*/


#include <NewPing.h>
#define maxDistance 400

#define movEchoPin 3
#define movTrigPin 4
#define pumpRly 2

NewPing sonar(movTrigPin, movEchoPin, maxDistance); // NewPing setup of pins and maximum distance.

///////////////////////////////////DEFINIÇOES GERAIS, ALTERAR SÓ AQUI///////////////////////////////////
boolean rlyType = 1;        //0 ou 1, de acordo com o tipo de relé
int pumpOnTime = 5;         //Tempo de funcionamento da bomba, em segundos
int triggerThreshold = 90;  //Largura do tunel(em cm) -10cm
int debDelay = 50;
////////////////////////////////////////////////////////////////////////////////////////////////////////

long trigDistance;
boolean tankState, pumpState, rlyOn, rlyOff;
long distHolder[5];
long reading;

long readDistance() {
  trigDistance = sonar.ping_cm();
  return trigDistance;
}


void printToSerial() {
  Serial.print("Tunel: ");
  Serial.print(trigDistance);
  Serial.print(" | Deposito: ");
  Serial.print(tankState);
  Serial.print(" | Rele: ");
  Serial.print(pumpState);
  Serial.println();
}

void setup() {
  if (rlyType) {
    rlyOn = HIGH;
    rlyOff = LOW;
  } else if (!rlyType) {
    rlyOn = LOW;
    rlyOff = HIGH;
  }

  // initialize serial communication:
  Serial.begin(9600);

  //DEFINE ALL PIN MODES
  pinMode(movEchoPin, INPUT);
  pinMode(movTrigPin, OUTPUT);
  pinMode(pumpRly, OUTPUT);

  //DEFINE INITIAL PIN STATE
  digitalWrite(movEchoPin, LOW);
  digitalWrite(movTrigPin, LOW);
  digitalWrite(pumpRly, HIGH);

  pumpOnTime = pumpOnTime * 1000;

}

void loop() {
  reading = readDistance();
  if (reading != 0 && reading < triggerThreshold) {
    digitalWrite(pumpRly, rlyOn);
    pumpState = 1;
    printToSerial();
    delay(pumpOnTime);
  }
  digitalWrite(pumpRly, rlyOff);
  pumpState = 0;
  delay(debDelay); //debounce delay
  printToSerial();
}
