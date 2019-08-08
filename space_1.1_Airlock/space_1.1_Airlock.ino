#define J_DOOR_IN A0
#define J_CABIN_LIGHT A1
#define J_STATION_LIGHT A2
//#define J_EMEMERGENCY 2

#define LED_AND_DOOR_IN 4
#define AIRLOCK_LIGHT 5
#define STATION_LIGHT 6
#define SOUND 7
#define BUTTON 8
#define SMOKE 9
#define SERVO 10
#define BUTTON_LAZER_ON 11
#define POWER_LAZER 12
#define DOOR_OUT 13

unsigned int game_position = 0;
unsigned long start_time = 0;
unsigned int numberTrack = 0;


void emergency() {
	digitalWrite(LED_AND_DOOR, HIGH);
}

void setup() {
	attachInterrupt(digitalPinToInterrupt(J_EMEMERGENCY), emergency, HIGH);
	
	pinMode (J_DOOR_IN, INPUT);
	pinMode (J_CABIN_LIGHT, INPUT);
	pinMode (J_STATION_LIGHT, INPUT);
	//pinMode (J_EMEMERGENCY, INPUT_PULLUP);
	pinMode (BUTTON, INPUT_PULLUP);

	pinMode (LED_AND_DOOR_IN, OUTPUT);
	pinMode (AIRLOCK_LIGHT, OUTPUT);
	pinMode (STATION_LIGHT, OUTPUT);
	pinMode (SOUND, OUTPUT);
	pinMode (SMOKE, OUTPUT);
	pinMode (SERVO, OUTPUT);
	pinMode (POWER_LAZER, OUTPUT);


}

void loop() {

	switch (game_position) {
		case 0: //Are there all players in the room?
		if (analogRead(J_DOOR_IN) > 500) {
			if (start_time == 0) {
				start_time = millis();
			}
			playTrack(20000, 10);
			numberTrack++;

		} else {
			game_position = 1;
			start_time = 0;
		}
		break;

		case 1: //Has button pushed?

		break;

		case 2: //
		
		break;

		case 3:

		break;

	}
	if (digitalRead(BUTTON) == LOW) {
		digitalWrite(LED, HIGH);
	} else {
		digitalWrite(LED, LOW);
	}

}

void playSound() {
	tone(SOUND, 30000, 1000);
	tone(SOUND, 20000, 2000);

}

int playTrack(int numberTrack, int playFromSrartTime) {
	if((millis() - start_time) > (playFromSrartTime * 1000)) {
		tone(SOUND, numberTrack);
		tone(SOUND, numberTrack * 1.2);
		tone(SOUND, numberTrack * 1.5, 3000);
		return 1;
	} else {
		return 0;
	}
}


// #include <Servo.h> //используем библиотеку для работы с сервоприводом

// Servo servo; //объявляем переменную servo типа Servo

// void setup() {
//   servo.attach(10);
//   servo.write(0);
// }

// void loop() {
//   speedRotateServo(3, 90);
//   delay(2000);
//   speedRotateServo(5, 60);
//   delay(2000);
// }

// void speedRotateServo(int velocity, int degree) {

//   int currentAngle = servo.read();
//   int angleRotate = abs(degree - currentAngle);
//   if (angleRotate == 0) {
//     angleRotate = 1;
//   }
//   int partDelay = (velocity * 1000) / abs(degree - currentAngle);

//   if (degree < currentAngle) {
//     for (int i = currentAngle; i <= degree; i++) {
//       servo.write(i);
//       delay(partDelay);
//     }
//   }
//   if (degree > currentAngle) {
//     for (int i = degree; i >= currentAngle; i--) {
//       servo.write(i);
//       delay(partDelay);
//     }
//   }

// }
