#include <Wire.h>
#include <Multiservo.h>

int JELEM = A0;
int SPECS = 10;
int MOTOR = 11;

Multiservo servo1;
Multiservo servo2;
Multiservo servo3;
Multiservo servo4;
Multiservo servo5;
Multiservo servo6;


void setup() {
	Wire.begin();
	servo1.attach (0);
	servo2.attach (1);
	servo3.attach (2);
	servo4.attach (3);
	servo5.attach (4);
	servo6.attach (5);
	pinMode (JELEM, INPUT);
	pinMode (SPECS, OUTPUT);
	pinMode (MOTOR, OUTPUT);
}


void loop() {
	servo1.write (105);
	servo2.write (110);
	delay (2000);
	servo6.write(45);
	servo5.write(135);
	delay (2000);
	servo3.write (10);
	servo4.write (170);

	if (analogRead(JELEM) > 500) {
		digitalWrite (SPECS, HIGH);
		delay (10000);
		digitalWrite (SPECS, LOW);
		digitalWrite (MOTOR, HIGH);
		delay (8000);
		servo1.write (180);
		delay (1000);
		servo2.write (135);
		servo5.write (50);
		delay (2000);
		servo6.write (5);
		servo3.write (35);
		delay (1000);
		servo4.write (80);
		delay (3000);
		servo6.write (45);
		delay (1500);
		servo4.write (170);
		servo3.write (10);
		delay (2000);
		servo5.write (135);
		servo2.write (110);
		delay (2000);
		servo1.write (180);
		delay (2000);
		servo2.write (135);
		servo5.write (50);
		delay(2000);
		servo3.write (35);
		servo4.write (80);
		delay (2000);
		servo6.write (5);
		delay (1000);
		servo4.write (170);
		delay (1000);
		servo3.write (10);
		delay (1000);
		servo6.write (45);
		delay (1000);
		servo5.write (135);
		delay (1000);
		servo2.write (110);
		delay (1000);
		servo1.write (105);
		delay (1000);
	}

}

