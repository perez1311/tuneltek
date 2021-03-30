/*
   code for tunnel entry, for nano with hc-sr04
*/


#include <NewPing.h>
#define maxDistance 400

#define movEchoPin 0
#define movTrigPin 1
#define tankStatePin 2
#define tunnelStatePin 3
#define valveRly 7
#define onBoardLed 10

NewPing sonar(movTrigPin, movEchoPin, maxDistance); // NewPing setup of pins and maximum distance.

///////////////////////////////////DEFINIÇOES GERAIS, ALTERAR SÓ AQUI///////////////////////////////////
int pumpOnTime = 5;         //Tempo de funcionamento da bomba, em segundos
int triggerThreshold = 60;  //Largura do tunel(em cm) -10cm
boolean rlyType = 0;        //0 ou 1, de acordo com o tipo de relé
int debDelay = 50;
////////////////////////////////////////////////////////////////////////////////////////////////////////

int loadTime = 2; //Time before it starts, in Milliseconds

boolean pumpState, rlyOn, rlyOff, tankState;
long trigDistance;
int reading = 0;
boolean state = 0;

long readDistance() {
  trigDistance = sonar.ping_cm();
  return trigDistance;
}

boolean waterLevel() {
  if (digitalRead(tankStatePin) == HIGH) {
    tankState = true;
  } else {
    tankState = false;
  }
  return tankState;
}

void printToSerial() {
  Serial.print("Tunnel: ");
  Serial.print(trigDistance);
  Serial.print(" | Relay: ");
  Serial.print(pumpState);
  Serial.print(" | Tank: ");
  Serial.print(tankState);
  Serial.println();
  digitalWrite(onBoardLed, HIGH);
  delay(5);
  digitalWrite(onBoardLed, LOW);
  delay(5);
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
  Serial.begin(115200);

  //DEFINE ALL PIN MODES
  pinMode(movEchoPin, INPUT);
  pinMode(movTrigPin, OUTPUT);
  pinMode(tankStatePin, INPUT);
  pinMode(tunnelStatePin, OUTPUT);
  pinMode(valveRly, OUTPUT);

  //DEFINE INITIAL PIN STATE
  digitalWrite(movEchoPin, LOW);
  digitalWrite(movTrigPin, LOW);
  digitalWrite(valveRly, rlyOff);
 
  pumpOnTime = pumpOnTime * 1000;
  loadTime = loadTime * 1000;
  delay(loadTime);
}

void loop() {
  if (waterLevel()) {
    reading = readDistance();
    if (reading != 0 && reading < triggerThreshold) {
      digitalWrite(tunnelStatePin, HIGH);
      digitalWrite(valveRly, rlyOn);
      pumpState = 1;
      printToSerial();
      delay(pumpOnTime);
    }
    digitalWrite(tunnelStatePin, LOW);
    digitalWrite(valveRly, rlyOff);
    pumpState = LOW;
    delay(debDelay); //debounce delay
  }
  printToSerial();
}
