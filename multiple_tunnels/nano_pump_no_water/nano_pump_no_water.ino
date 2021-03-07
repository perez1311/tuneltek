/*
 * code for pump and level sensor, NO WATER SENSOR, for NANO
 */

#define pumpRly 7
#define statePin 8

int movstatePin[3] = {10, 11, 12};
boolean movState[3] = {0, 0, 0};

boolean checkTunnels() {
  for (int i = 0; i < 3; i++) {
    movState[i] = digitalRead(movstatePin[i]);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(movState[i]);
    Serial.print(" | ");
  }
  Serial.println();
  if (movState[0] == 0 && movState[1] == 0 && movState[2] == 0) {
    return false;
  } else {
    return true;
  }
}

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

  //DEFINE ALL PIN MODES
  pinMode(pumpRly, OUTPUT);
  pinMode(statePin, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(movstatePin[i], INPUT);
  }

  //DEFINE INITIAL PIN STATE
  digitalWrite(pumpRly, LOW);
  digitalWrite(statePin, HIGH);
}

void loop() {
  if (checkTunnels()) {
    digitalWrite(pumpRly, HIGH);
  } else {
    digitalWrite(pumpRly, LOW);
  }
}
