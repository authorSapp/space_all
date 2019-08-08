int J_MASTER = A0;
int J_CABIN_LIGHT = A1;
int J_STATION_LIGHT = A2;
//int JOPEN = A3;
//int DOOR = 2;
//int MAGNET = 3;
int LED_AND_DOOR = 4;
int STATION_LIGHT = 5;
int BATTERY_CONNECTED = 8;
int CABIN_LIGHT = 7;
  
void setup() {

  delay(1000);
  pinMode(J_MASTER, INPUT);
  pinMode(J_CABIN_LIGHT, INPUT);
  pinMode(J_STATION_LIGHT, INPUT);
  //pinMode(J_OPEN, INPUT);
  pinMode(BATTERY_CONNECTED, INPUT); //high=battery installed, light=white

  //pinMode(DOOR, OUTPUT);
  //pinMode(MAGNET, OUTPUT);
  pinMode(LED_AND_DOOR, OUTPUT);
  pinMode(STATION_LIGHT, OUTPUT);
  pinMode(CABIN_LIGHT, OUTPUT);
  delay(1000);

}

void loop() {

  if (analogRead(J_MASTER) > 500) {
    digitalWrite(LED_AND_DOOR, HIGH);  //switch on red led, close door
    // digitalWrite(DOOR, HIGH);
  } else {
    digitalWrite(LED_AND_DOOR, LOW);  //switch on green led, close door
  }

  if ((analogRead(J_STATION_LIGHT) > 500) || (digitalRead(BATTERY_CONNECTED) == HIGH)) {
    digitalWrite(STATION_LIGHT, LOW); //white
  } else {
    digitalWrite(STATION_LIGHT, HIGH); //red
  }

  if (analogRead(J_CABIN_LIGHT) > 500) {
    digitalWrite(CABIN_LIGHT, HIGH);
  } else {
    digitalWrite(CABIN_LIGHT, LOW);
  }

  /*
    if (analogRead(JOPEN) > 500) {// not in use
    digitalWrite(MAGNET, HIGH);
    }

  */

}





