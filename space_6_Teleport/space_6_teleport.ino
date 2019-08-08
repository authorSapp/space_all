#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#define TIMER_DELAY 400 //for stable signal from jerome

#define BRIGHTNESS_LIGHT 30
#define VOLUME_SOUND 25

#define OPENED 1
#define CLOSED 0

#define J_MAG A0
#define J_DOOR A1
#define RESET A5

//#define J_EMEMERGENCY 2
//pin 3,4 SoftwareSerial for Mp3 Player
//#define DOOR_ACTION 5
#define DOOR_DIRECTION 6
#define BOX 7
#define MAGNET 8
#define LIGHT_12V 9
#define LIGHT_220V 10
#define MOVE_DETECTOR 11
#define GERCON 12

boolean slidingDoorStatus = CLOSED;
boolean magnetStatus = CLOSED;
boolean boxStatus = CLOSED;
boolean resetStatus = false;
boolean gerconStatus = false;
boolean detectorStatus = true;
boolean light220Status = false;

unsigned int game_position = 0;
unsigned long start_time = 0;
unsigned long time_delay = 0;
float brightness = BRIGHTNESS_LIGHT;

unsigned int playlist_1[] = {1, 11, 12, 13, 41};
unsigned int timer_for_playlist1[] = {0, 16, 80, 160, 0};
int numberTrack1 = 0;

unsigned int playlist_2[] = {23, 24, 25, 26};
unsigned int timer_for_playlist2[] = {4, 5, 5, 7};
int numberTrack2 = 0;

boolean logState = true;
boolean playSoundStatus = true;
boolean stroboscopeStatus = true;
unsigned int count = 0;

SoftwareSerial SoftwareSerialForMp3(3, 4);


//void emergency() {
//  digitalWrite(LIGHT_220V, HIGH);
//  digitalWrite(LIGHT_12V, HIGH);
//  digitalWrite(MAGNET, LOW);
//  if (slidingDoorStatus == CLOSED) {
//    digitalWrite(DOOR_DIRECTION, LOW);
//    slidingDoorStatus = OPENED;
//  }
//}

void reset() {
	mp3_stop();
	delay(100);
	init_Game();

	slidingDoorStatus = CLOSED;
	magnetStatus = CLOSED;
	boxStatus = CLOSED;
	resetStatus = false;
	gerconStatus = false;
	light220Status = false;

	game_position = 0;
	start_time = 0;
	time_delay = 0;
	brightness = BRIGHTNESS_LIGHT;

	numberTrack1 = 0;
	numberTrack2 = 0;

	logState = true;
	playSoundStatus = true;
	stroboscopeStatus = true;
	count = 0;
}


void setup() {

	init_GPIO();
	init_Mp3_Player();
	init_Game();

	Serial.begin(9600);
	Serial.println("Setup ended");
	delay(1000);

}


void loop() {

	if (analogRead(RESET) > 500) {
		if (resetStatus == true) {
			reset();
			resetStatus = false;
		}
		if (logState == true) {
			Serial.println("Reset sector");
			logState = false;
		}
		while(analogRead(RESET) > 500) {
			logState = true;
			Serial.println("Reset delay");
			delay(500);
		}
	} else {
		resetStatus = true;
	}

	if (analogRead(J_MAG) > 500) {
		if (magnetStatus == CLOSED) {
			digitalWrite(MAGNET, LOW);
			magnetStatus = OPENED;
			Serial.println("J_MAG closed");
		}

	} else {
		if (magnetStatus == OPENED) {
			digitalWrite(MAGNET, HIGH);
			magnetStatus = CLOSED;
			Serial.println("J_MAG opened");
		}

	}

	if (analogRead(J_DOOR) > 500) {
		if (slidingDoorStatus == CLOSED) {
			digitalWrite(DOOR_DIRECTION, LOW);
			slidingDoorStatus = OPENED;
			Serial.println("J_DOOR opened");
		}
	} else {
		if (slidingDoorStatus == OPENED) {
			digitalWrite(DOOR_DIRECTION, HIGH);
			slidingDoorStatus = CLOSED;
			Serial.println("J_DOOR closed");
		}
	}

	switch (game_position) {

		case 0:
		if (logState == true) {
			Serial.println("case 0");
			logState = false;
		}
		if (start_time == 0) {
			start_time = millis();
		}
		if ((digitalRead(MOVE_DETECTOR) == HIGH) && (digitalRead(J_MAG) == LOW)) {
			Serial.println("MOVE_DETECTOR activated");
			delay(2000);
			if (numberTrack1 < 1) {
				if (play_Track_byTimer(playlist_1[numberTrack1], timer_for_playlist1[numberTrack1], VOLUME_SOUND)) {
					numberTrack1++;
					Serial.print("numberTrack1 - ");
					Serial.println(numberTrack1);
				}
			}
			start_time = 0;
			logState = true;
			game_position = 1;
			delay(4500);
			Serial.println("to case 1");
		}
		break;


		case 1:
		if (logState == true) {
			Serial.println("case 1");
			logState = false;
		}
		if (start_time == 0) {
			start_time = millis();
		}
		if (numberTrack1 < 4) {
			if (play_Track_byTimer(playlist_1[numberTrack1], timer_for_playlist1[numberTrack1], VOLUME_SOUND)) {
				numberTrack1++;
				Serial.print("numberTrack1 - ");
				Serial.println(numberTrack1);
			}
		}
		if (digitalRead(GERCON) == LOW) {
			Serial.println("GERCON activated");
			play_Track(14, VOLUME_SOUND);
			start_time = 0;
			numberTrack1 = 4;
			logState = true;
			game_position = 2;
			Serial.println("to case 2");
			delay(1100);
		}
		break;


		case 2:
		if (logState == true) {
			Serial.println("case 2");
			logState = false;
		}
		if (digitalRead(GERCON) == HIGH) {
			Serial.println("GERCON deactivated");
			play_Track(22, VOLUME_SOUND);
			while (brightness > BRIGHTNESS_LIGHT) {
				analogWrite(LIGHT_12V, (unsigned int)brightness);
				brightness -= 0.005;
			}
			count = 0;
			numberTrack1 = 1;
			game_position = 1;
			delay(500);
			break;
		}
		if (start_time == 0) {
			start_time = millis();
		}
		if (digitalRead(MOVE_DETECTOR) == LOW) {
			if (count < 4) {
				Serial.print("count - ");
				Serial.println(count);

				play_Track(21, VOLUME_SOUND + count);
				for (int i = 0; i < 250; ++i) {
					brightness = abs(100 * (float)sin(i / 45.0)) + (30 * count);
					Serial.print(i);
					Serial.print(" - ");
					Serial.println(brightness);
					analogWrite(LIGHT_12V, brightness);
				}
				count++;
				playSoundStatus = true;
			} else {
				count = 0;
				start_time = 0;
				logState = true;
				game_position = 3;
				Serial.println("to case 3");
				break;
			}
		} else {
			if (playSoundStatus == true) {
				play_Track(22, VOLUME_SOUND);

				while (brightness > BRIGHTNESS_LIGHT) {
					analogWrite(LIGHT_12V, (unsigned int)brightness);
					brightness -= 0.005;
				}
				delay(500);
				play_Track(playlist_2[numberTrack2], VOLUME_SOUND);
				delay(timer_for_playlist2[numberTrack2] * 1000);
				numberTrack2 = (numberTrack2 + 1) % 4;
				Serial.print("numberTrack2 - ");
				Serial.println(numberTrack2);
				playSoundStatus = false;
			}
			count = 0;
		}
		break;


		case 3:
		if (logState == true) {
			Serial.println("case 3");
			logState = false;
		}

		logState = true;
		game_position = 4;
		Serial.println("to case 4");
		break;

		case 4:
		if (logState == true) {
			Serial.println("case 4");
			logState = false;
		}
		if (numberTrack1 < 5) {
			if (play_Track_byTimer(playlist_1[numberTrack1], timer_for_playlist1[numberTrack1], VOLUME_SOUND + 3)) {
				numberTrack1++;
				Serial.print("numberTrack1 - ");
				Serial.println(numberTrack1);
			}
		}

		if (stroboscopeStatus == true) {
			time_delay = 200;
			for (int i = 0; i < 6; i++) {
				if (time_delay > 90) {

					for (int j = 0; j < 4; j++) {
						stroboscope(time_delay);
					}
					time_delay -= 16;
				} else {
					for (int j = 0; j < 12; j++) {
						stroboscope(time_delay);
					}
					time_delay -= 16;
				}
			}
			time_delay = 0;
			stroboscopeStatus = false;
		}

		if (start_time == 0) {
			start_time = millis();
		}

		time_delay++;

		if (time_delay > (unsigned long)5 * 1000) {
			if (light220Status == false) {
				digitalWrite(LIGHT_220V, HIGH);
				Serial.print("LIGHT_220V switch on, sec: ");
				Serial.println((millis() - start_time) / 1000.0);
				light220Status = true;
			}
		}

//            if (time_delay > (unsigned long)15 * 1000) {
//              if (boxStatus == CLOSED) {
//                digitalWrite(BOX, HIGH);
//                Serial.print("BOX opened, sec: ");
//                Serial.println((millis() - start_time) / 1000.0);
//                boxStatus = OPENED;
//              }
//            }

		if (time_delay > (unsigned long)20 * 1000) {
//			if (slidingDoorStatus == CLOSED) {
				digitalWrite(DOOR_DIRECTION, LOW);
			  // slidingDoorStatus = OPENED;
				Serial.print("sliding_Door opened, sec: ");
				Serial.println((millis() - start_time) / 1000.0);
				time_delay = 0;
				start_time = 0;
        		brightness = 254;
				logState = true;
				game_position = 5;
				Serial.println("to case 5");
//			}
		}
		break;

		case 5:
		//End game
		if (logState == true) {
			Serial.println("End game");
			logState = false;
		}
		if (brightness > 0) {
			analogWrite(LIGHT_12V, (unsigned int)brightness);
			brightness -= 0.002;
		}
		break;

	}

	
}


void init_GPIO() {
	pinMode(RESET, INPUT);
	pinMode(J_MAG, INPUT);
	pinMode(J_DOOR, INPUT);
	//  pinMode(J_EMEMERGENCY, INPUT);
	pinMode(GERCON, INPUT);
	pinMode(MOVE_DETECTOR, INPUT);

	pinMode(DOOR_DIRECTION, OUTPUT);  // LOW = open direction
	pinMode(BOX, OUTPUT);             // HIGH = open
	pinMode(MAGNET, OUTPUT);          // HIGH = open
	pinMode(LIGHT_12V, OUTPUT);       // HIGH = on
	pinMode(LIGHT_220V, OUTPUT);      // HIGH = on
}

void init_Mp3_Player() {
	SoftwareSerialForMp3.begin(9600);
	mp3_set_serial (SoftwareSerialForMp3);
	delay(100);
	mp3_set_volume (VOLUME_SOUND);
	delay(100);
	mp3_stop();
}

void init_Game() {
	digitalWrite(BOX, LOW); // BOX closed
	digitalWrite(MAGNET, LOW); // MAGNET closed
	digitalWrite(LIGHT_220V, LOW); // LIGHT_220V off
	digitalWrite(DOOR_DIRECTION, LOW); // DOOR_DIRECTION open
	analogWrite(LIGHT_12V, BRIGHTNESS_LIGHT);
}

void play_Track(int track, int volume) {
	volume = constrain(volume, 0, 30);
	mp3_set_volume (volume);
	delay (100);
	mp3_play(track);
	delay (100);
}

int play_Track_byTimer(int track, unsigned long playFromSrartTime, int volume) {
	if ((millis() - start_time) > (playFromSrartTime * 1000)) {
		play_Track(track, volume);
		return 1;
	} else {
		return 0;
	}
}

void stroboscope(unsigned int time_delay) {
	digitalWrite(LIGHT_12V, LOW);
	delay(time_delay);
	digitalWrite(LIGHT_12V, HIGH);
	delay(time_delay);

}

