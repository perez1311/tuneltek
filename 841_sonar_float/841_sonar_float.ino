/*
 * This code is for attiny 841 with floating level switch and sr-hc04 sensor.
 * Use Counter-clockwise pin mode
 */


#include <NewPing.h>
#define maxDistance 400

#define movEchoPin 10
#define movTrigPin 7
#define pumpRly 6
#define lightRly 5  //this is also the test pin
#define tankPin 3
#define onBoardLed 1

NewPing sonar(movTrigPin, movEchoPin, maxDistance); // NewPing setup of pins and maximum distance.

///////////////////////////////////DEFINIÇOES GERAIS, ALTERAR SÓ AQUI///////////////////////////////////
int pumpOnTime = 4;         //Tempo de funcionamento da bomba, em segundos
int triggerThreshold = 80;  //Largura do tunel(em cm) -10cm
boolean rlyType = 0;        //0 ou 1, de acordo com o tipo de relé
int debDelay = 50;
////////////////////////////////////////////////////////////////////////////////////////////////////////

boolean pumpState, rlyOn, rlyOff, tankState, testState;
long trigDistance;
int reading = 0;

long readDistance() {
  trigDistance = sonar.ping_cm();
  return trigDistance;
}

boolean waterLevel() {
  tankState = !digitalRead(tankPin);
  return tankState;
}

boolean testMode() {
  if (!digitalRead(lightRly)) {
    testState = true;
  }
  else if (digitalRead(lightRly)) {
    testState = false;
  }
  return testState;
}

//USE TO TEST SKETCH
//boolean waterLevel() {
//  tankState = HIGH;
//  return tankState; //TRUE = OK, FALSE = EMPTY
//}

void printToSerial() {
  Serial.print("Tunnel: ");
  Serial.print(trigDistance);
  Serial.print(" | Rele: ");
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

  pumpOnTime = pumpOnTime * 1000;

  // initialize serial communication:
  Serial.begin(9600);

  pinMode(lightRly, INPUT_PULLUP);
  pinMode(onBoardLed, OUTPUT);
  pinMode(pumpRly, OUTPUT);
  
  if (testMode()) {
    while (1) {
      digitalWrite(pumpRly, rlyOn);
      digitalWrite(onBoardLed, HIGH);
      Serial.println("TEST MODE ON!");
      delay(2000);
      digitalWrite(pumpRly, rlyOff);
      digitalWrite(onBoardLed, LOW);
      Serial.println("TEST MODE OFF!");
      delay(2000);
    }
  }

  

  //DEFINE ALL PIN MODES
  pinMode(lightRly, OUTPUT);
  pinMode(tankPin, INPUT);

  //DEFINE INITIAL PIN STATE
  digitalWrite(pumpRly, rlyOff);
  digitalWrite(lightRly, rlyOff);
  digitalWrite(onBoardLed, LOW);
}

void loop() {
  if (waterLevel()) {
    reading = readDistance();
    if (reading != 0 && reading < triggerThreshold) {
      digitalWrite(pumpRly, rlyOn);
      digitalWrite(lightRly, rlyOn);
      pumpState = 1;
      printToSerial();
      delay(pumpOnTime);
    }
  }
  digitalWrite(pumpRly, rlyOff);
  digitalWrite(lightRly, rlyOff);
  pumpState = 0;
  delay(debDelay); //debounce delay
  printToSerial();
}
