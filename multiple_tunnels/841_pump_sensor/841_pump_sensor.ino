/*
   code for pump and level sensor, with hc-sr04 water sensor, for NANO
*/


#define floatSensorPin 5
#define pumpRly 7
#define statePin 0

boolean rlyType = 0;        //0 ou 1, de acordo com o tipo de relé
int loadTime = 2;

int movstatePin[3] = {1, 2, 3};
boolean movState[3] = {0, 0, 0};
boolean pumpState, rlyOn, rlyOff, tankState;

boolean waterLevel() {
  tankState = !digitalRead(floatSensorPin);
  //Serial.print("Tank: ");
  //Serial.print(tankState);
  //Serial.print(" | ");
  return tankState;
}

boolean checkTunels() {
  for (int i = 0; i < 3; i++) {
    movState[i] = digitalRead(movstatePin[i]);
    //Serial.print("Tunel ");
    //Serial.print(i);
    //Serial.print(" ");
    //Serial.print(movState[i]);
    //Serial.print(" | ");
  }
  //Serial.println();
  if (movState[0] == 0 && movState[1] == 0 && movState[2] == 0) {
    return false;
  } else {
    return true;
  }
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
  pinMode(floatSensorPin, INPUT_PULLUP);
  pinMode(pumpRly, OUTPUT);
  pinMode(statePin, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(movstatePin[i], INPUT);
  }

  //DEFINE INITIAL PIN STATE
  digitalWrite(pumpRly, rlyOff);
  digitalWrite(statePin, LOW);
  delay(loadTime);
}

void loop() {
  while (waterLevel()) {
    digitalWrite(statePin, HIGH);
    Serial.println("yes");
    while (checkTunels()) {
      digitalWrite(pumpRly, rlyOn);
    }
    digitalWrite(pumpRly, rlyOff);
  }
  digitalWrite(statePin, LOW);
}
