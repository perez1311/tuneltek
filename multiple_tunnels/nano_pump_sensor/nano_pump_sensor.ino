/*
 * code for pump and level sensor, with hc-sr04 water sensor, for NANO
 */


#define levelEchoPin 5
#define levelTrigPin 6
#define pumpRly 7
#define statePin 8

int movstatePin[3] = {10, 11, 12};
boolean movState[3] = {0, 0, 0};

int maxWaterDistance = 75; //Maximum water distance, cm from top


boolean waterLevel() {
  // The PING is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(levelTrigPin, LOW);
  delayMicroseconds(4);
  digitalWrite(levelTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(levelTrigPin, LOW);

  int waterDistance = (0.034 * pulseIn(levelEchoPin, HIGH)) / 2;

  if (waterDistance < maxWaterDistance) {
    Serial.println(waterDistance);
    return true;
  }
  else {
    Serial.println(waterDistance);
    return false;
  }
}

boolean checkTunnels() {
  for (int i = 0; i < 3; i++) {
    movState[i] = digitalRead(movstatePin[i]);
    Serial.print(i);
    Serial.print(" ");
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
  pinMode(levelEchoPin, INPUT);
  pinMode(levelTrigPin, OUTPUT);
  pinMode(pumpRly, OUTPUT);
  pinMode(statePin, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(movstatePin[i], INPUT);
  }

  //DEFINE INITIAL PIN STATE
  digitalWrite(levelEchoPin, LOW);
  digitalWrite(levelTrigPin, LOW);
  digitalWrite(pumpRly, LOW);
  digitalWrite(statePin, LOW);
}

void loop() {
  while (waterLevel()) {
    digitalWrite(statePin, HIGH);
    if (checkTunnels()) {
      digitalWrite(pumpRly, HIGH);
    } else {
      digitalWrite(pumpRly, LOW);
    }
    digitalWrite(statePin, LOW);
  }
}
