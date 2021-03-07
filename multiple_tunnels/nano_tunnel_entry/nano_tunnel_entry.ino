/*
 * code for tunnel entry, for nano with hc-sr04
 */


#define movEchoPin 4
#define movTrigPin 3
#define tankState 10
#define tunnelState 11
#define valveRly 2
#define lightRly 7


///////////////////////////////////DEFINIÇOES GERAIS, ALTERAR SÓ AQUI///////////////////////////////////
int pumpOnTime = 5;         //Tempo de funcionamento da bomba, em segundos
int triggerThreshold = 60;  //Largura do tunel(em cm) -10cm
////////////////////////////////////////////////////////////////////////////////////////////////////////

int loadTime = 2; //Time before it starts, in Milliseconds

long trigDistance;
long distHolder[5];
long lowVal = 0;
boolean state = 0;

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

boolean waterLevel() {
  if (digitalRead(tankState) == HIGH) {
    return true;
  } else {
    return false;
  }
}

void printToSerial() {
  Serial.print("Tunel: ");
  Serial.print(trigDistance);
  Serial.print( "Pump: ");
  Serial.println(state);
}

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

  //DEFINE ALL PIN MODES
  pinMode(movEchoPin, INPUT);
  pinMode(movTrigPin, OUTPUT);
  pinMode(tankState, INPUT);
  pinMode(tunnelState, OUTPUT);
  pinMode(valveRly, OUTPUT);
  pinMode(lightRly, OUTPUT);

  //DEFINE INITIAL PIN STATE
  digitalWrite(movEchoPin, LOW);
  digitalWrite(movTrigPin, LOW);
  digitalWrite(valveRly, LOW);
  digitalWrite(lightRly, LOW);

  pumpOnTime = pumpOnTime * 1000;
  loadTime = loadTime * 1000;

}

void loop() {
  if (waterLevel()) {
    if (readDistance() < triggerThreshold) {
      digitalWrite(tunnelState, HIGH);
      digitalWrite(valveRly, HIGH);
      digitalWrite(lightRly, HIGH);
      state = HIGH;
      printToSerial();
      delay(pumpOnTime);
    } else {
      digitalWrite(tunnelState, LOW);
      digitalWrite(valveRly, LOW);
      digitalWrite(lightRly, LOW);
      state = LOW;
      printToSerial();
    }
    delay(50); //debounce delay
  }
  state = LOW;
  printToSerial();
}
