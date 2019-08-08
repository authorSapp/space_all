int SensIn1 = A7;
int SensIn2 = A8;
int SensIn3 = A9;
int SensIn4 = A10;
int SensIn5 = A11;
int SensIn6 = A12;
int SensIn7 = A13;
int SensIn8 = A14;
int SensIn9 = A15;
int J_IN13 = A0;
int J_IN46 = A1;
int J_IN79 = A2;
int J_MASTER = A3;
int BoxOut1 = 23;
int BoxOut2 = 25;
int BoxOut3 = 27;
int BoxOut4 = 29;
int BoxOut5 = 31;
int BoxOut6 = 33;
int BoxOut7 = 35;
int BoxOut8 = 37;
int BoxOut9 = 39;
int SensorDisconnect = 8;

void setup() {

	analogReference(EXTERNAL);
	pinMode(SensIn1, INPUT);
	pinMode(SensIn2, INPUT);
	pinMode(SensIn3, INPUT);
	pinMode(SensIn4, INPUT);
	pinMode(SensIn5, INPUT);
	pinMode(SensIn6, INPUT);
	pinMode(SensIn7, INPUT);
	pinMode(SensIn8, INPUT);
	pinMode(SensIn9, INPUT);
	pinMode(J_IN13, INPUT);
	pinMode(J_IN46, INPUT);
	pinMode(J_IN79, INPUT);
	pinMode(J_MASTER, INPUT);
	pinMode(BoxOut1, OUTPUT);
	pinMode(BoxOut2, OUTPUT);
	pinMode(BoxOut3, OUTPUT);
	pinMode(BoxOut4, OUTPUT);
	pinMode(BoxOut5, OUTPUT);
	pinMode(BoxOut6, OUTPUT);
	pinMode(BoxOut7, OUTPUT);
	pinMode(BoxOut8, OUTPUT);
	pinMode(BoxOut9, OUTPUT);
	pinMode(SensorDisconnect, OUTPUT);

	digitalWrite(BoxOut1, HIGH);
	digitalWrite(BoxOut2, HIGH);
	digitalWrite(BoxOut3, HIGH);
	digitalWrite(BoxOut4, HIGH);
	digitalWrite(BoxOut5, HIGH);
	digitalWrite(BoxOut6, HIGH);
	digitalWrite(BoxOut7, HIGH);
	digitalWrite(BoxOut8, HIGH);
	digitalWrite(BoxOut9, HIGH);
	digitalWrite(SensorDisconnect, HIGH);
	delay(1000);
	
}

void loop() {

	OpenBox(SensIn1, BoxOut1);
	OpenBox(SensIn2, BoxOut2);
	OpenBox(SensIn3, BoxOut3);
	OpenBox(SensIn4, BoxOut4);
	OpenBox(SensIn5, BoxOut5);
	OpenBox(SensIn6, BoxOut6);
	OpenBox(SensIn7, BoxOut7);
	OpenBox(SensIn8, BoxOut8);
	OpenBox(SensIn9, BoxOut9);

	OpenBox_ByJerome(J_IN13, BoxOut1, BoxOut2, BoxOut3);
	OpenBox_ByJerome(J_IN46, BoxOut4, BoxOut5, BoxOut6);
	OpenBox_ByJerome(J_IN79, BoxOut7, BoxOut8, BoxOut9);

	DisconectSensorPanel(J_MASTER);

}

void OpenBox(int SensIn, int BoxOut) {
	if (analogRead(SensIn) > 500) {
		digitalWrite(BoxOut, LOW);
		delay(500);
		digitalWrite(BoxOut, HIGH);
	}
}

void OpenBox_ByJerome(int JeromePin, int BoxOut1, int BoxOut2, int BoxOut3) {
	if (analogRead(JeromePin) > 500) {
		digitalWrite(BoxOut1, LOW);
		delay(200);
		digitalWrite(BoxOut2, LOW);
		delay(200);
		digitalWrite(BoxOut3, LOW);
		delay(200);
		digitalWrite(BoxOut1, HIGH);
		digitalWrite(BoxOut2, HIGH);
		digitalWrite(BoxOut3, HIGH);
	}
}

void DisconectSensorPanel(int JeromePin) {
	if (analogRead(JeromePin) > 500) {
		digitalWrite(SensorDisconnect, HIGH);
	} else {
		digitalWrite(SensorDisconnect, LOW);
	}
}
