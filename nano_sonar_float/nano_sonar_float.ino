/*
 * Code for nano with floating level switch and hc-sr04
 */


#define movEchoPin 3
#define movTrigPin 4
#define levelPin 5
#define pumpRly 2
#define lightRly 7

///////////////////////////////////DEFINIÇOES GERAIS, ALTERAR SÓ AQUI///////////////////////////////////
boolean rlyType = 1;        //0 ou 1, de acordo com o tipo de relé
int pumpOnTime = 5;         //Tempo de funcionamento da bomba, em segundos
int triggerThreshold = 90;  //Largura do tunel(em cm) -10cm
////////////////////////////////////////////////////////////////////////////////////////////////////////

long trigDistance;
boolean tankState, pumpState, rlyOn, rlyOff;
long distHolder[5];
long lowVal = 0;

long readDistance() {
  // The PING is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  for (int i = 0; i < 5; i++) {
    distHolder[i] = 0;
  }
  trigDistance = 0;
  lowVal = 1000;
  for (int i = 0; i < 5; i++) {
    digitalWrite(movTrigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(movTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(movTrigPin, LOW);

    distHolder[i] = (0.034 * pulseIn(movEchoPin, HIGH)) / 2;

  }

  for (int i = 0; i < 5; i++) {
    if (distHolder[i] < lowVal) {
      lowVal = distHolder[i];
    }
  }

  for (int i = 0; i < 5; i++) {
    trigDistance = trigDistance + distHolder[i];
  }

  trigDistance = (trigDistance - lowVal) / 4;

  return trigDistance;
}

long waterLevel() {
  if (digitalRead(levelPin)) {
    tankState = false;
  }
  else {
    tankState = true;
  }

  return tankState; //TRUE = OK, FALSE = EMPTY
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
  pinMode(levelPin, INPUT_PULLUP);
  pinMode(pumpRly, OUTPUT);
  pinMode(lightRly, OUTPUT);

  //DEFINE INITIAL PIN STATE
  digitalWrite(movEchoPin, LOW);
  digitalWrite(movTrigPin, LOW);
  digitalWrite(pumpRly, HIGH);
  digitalWrite(lightRly, HIGH);

  pumpOnTime = pumpOnTime * 1000;

}

void loop() {
  if (waterLevel()) {
    if (readDistance() < triggerThreshold) {
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
  delay(50); //debounce delay
  printToSerial();
}
