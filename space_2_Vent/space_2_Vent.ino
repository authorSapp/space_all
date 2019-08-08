#define TIMER_DELAY 400 //for stable signal from jerome

// #define MASTER A0
#define J_OPEN A1
//#define BATTERY 7
#define BUTTON 8
//#define ALARM 3
#define DOOR 4

// int number = 0;
boolean timerFlag = false;
boolean pushButton = false;
unsigned long startTime = 0;

void setup() {

	//pinMode (MASTER, INPUT);
	pinMode (J_OPEN, INPUT);
	//pinMode (BATTERY, INPUT);
	pinMode (BUTTON, INPUT_PULLUP);
	//pinMode (ALARM, OUTPUT);
	pinMode (DOOR, OUTPUT);
	delay(1000);

}

//void(* resetFunc) (void) = 0;

void loop() {

	if (digitalRead(BUTTON) == LOW) pushButton = true;

	//  if (digitalRead(BATTERY) == 0) {
	//    number = (millis() / 1000);
	//    if (number > 240) {
	//      digitalWrite(ALARM, HIGH);
	//    }
	//  }
	//  if (digitalRead(BATTERY) == 1) {
	//    digitalWrite(ALARM, LOW);
	//    resetFunc();
	//  }

	//  if (analogRead(MASTER) > 500) {
	//    if (digitalRead(BUTTON) == 1) {
	//      digitalWrite(DOOR, HIGH);
	//    }
	//  }



	if (analogRead(J_OPEN) > 500) {
		if(timerFlag == false) {
			startTime = millis();
			timerFlag = true;
		}
		if (millis() - startTime > TIMER_DELAY) {
			digitalWrite(DOOR, HIGH);
			pushButton = false;
			timerFlag = false;
		}
	} else {
		if (timerFlag == true) {
			if (millis() - startTime > TIMER_DELAY) {
				timerFlag = false;
			}
		}
	}
	
	  if (analogRead(J_OPEN) < 300) {
		if (pushButton == true) {
			digitalWrite(DOOR, HIGH);
		} else {
			digitalWrite(DOOR, LOW);
		}
	}
	

}
