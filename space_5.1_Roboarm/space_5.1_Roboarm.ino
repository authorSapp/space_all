/*9-аэратор
	 8-дым
	 7---
	 6---
	 5-подсветка колб и подсветка холодильника
	 4-вентилляторы
	 3-лифт
	 2-лифт
	 активация-низкий уровень
*/
#include <Wire.h>
#include <Multiservo.h>

#define TIMER_DELAY 400 //for stable signal from jerome

#define openHand 40
#define closeHand 140

#define J_ELEM A0 //pulldown
#define ELEMENTS_READY A1 //pulldown
#define FRIDGE_DOOR 13
#define BUBBLE 9
#define SMOKE 8
#define BACKLIGHT 5
#define COOLER 4
#define LIFTUP 3
#define LIFTDOWN 2

//Multiservo servo1;
Multiservo servo2;
Multiservo servo3;
Multiservo servo4;
Multiservo servo5;
Multiservo servo6;

boolean timerFlag[] = {false, false};
unsigned long startTime[] = {0, 0};

boolean action = false;

void setup() {

  // Serial.begin(9600);
  // delay(500);
  // Wire.begin();

  init_GPIO();
  delay(1000);

  // while (1) {
  // 	liftUP();
  // 	delay(3000);
  // 	liftDown();
  // 	delay(3000);
  // }


  // test();

}


void loop() {

  // Serial.println(analogRead(JELEM));
  // delay(1000);


  // if (Serial.available() > 0) {
  // 	byte  incomingByte = Serial.parseInt();
  // 	servo6.attach (7);
  // 	servo6.write (incomingByte);
  // 	Serial.println(incomingByte,DEC);
  // }


  if ((analogRead(J_ELEM) > 500) && (digitalRead(FRIDGE_DOOR) == LOW)) {
    if (timerFlag[1] == false) {
      startTime[1] = millis();
      timerFlag[1] = true;
    }
    if (millis() - startTime[1] > TIMER_DELAY) {
      megaSuperGiperTrooperEffect();
      timerFlag[1] = false;
    }
  } else {
    if (timerFlag[1] == true) {
      if (millis() - startTime[1] > TIMER_DELAY) {
        timerFlag[1] = false;
      }
    }
  }

  if ((analogRead(J_ELEM) > 500) && (digitalRead(FRIDGE_DOOR) == HIGH)) {
    if (timerFlag[2] == false) {
      startTime[2] = millis();
      timerFlag[2] = true;
    }
    if (millis() - startTime[2] > TIMER_DELAY) {
      initToBegin();
      timerFlag[2] = false;
    }
  } else {
    if (timerFlag[2] == true) {
      if (millis() - startTime[2] > TIMER_DELAY) {
        timerFlag[2] = false;
      }
    }
  }

  if (action == false) {
    if ((digitalRead(ELEMENTS_READY) == HIGH) && (digitalRead(FRIDGE_DOOR) == LOW)) {
      action = true;
      megaSuperGiperTrooperEffect();
    }
  }

}


void init_GPIO() {
  pinMode (J_ELEM, INPUT);
  pinMode (ELEMENTS_READY, INPUT);
  pinMode (FRIDGE_DOOR, INPUT_PULLUP);

  pinMode (BUBBLE, OUTPUT);
  pinMode (SMOKE, OUTPUT);
  pinMode (BACKLIGHT, OUTPUT);
  pinMode (COOLER, OUTPUT);
  pinMode (LIFTUP, OUTPUT);
  pinMode (LIFTDOWN, OUTPUT);
  digitalWrite (BUBBLE, HIGH);
  digitalWrite (SMOKE, HIGH);
  digitalWrite (BACKLIGHT, HIGH);
  digitalWrite (COOLER, HIGH);
  digitalWrite (LIFTUP, HIGH);
  digitalWrite (LIFTDOWN, HIGH);
}

void initToBegin() {
  action = false;
  for (int i = 0; i < 2; i++) {
    timerFlag[i] = false;
    startTime[i] = 0;
  }
}

void megaSuperGiperTrooperEffect() {

  servo4.attach (5);
  servo5.attach (6);
  servo6.attach (7);

  //рука вертикально, закрыта
  servo4.write (40);
  delay (1000);
  servo5.write(45);
  delay (1000);
  servo6.write(60);
  delay (1000);

  digitalWrite (BUBBLE, LOW);
  delay(500);
  digitalWrite (BACKLIGHT, LOW);
  delay(10000);
  digitalWrite (BUBBLE, HIGH);
  delay(500);
  digitalWrite (BACKLIGHT, HIGH);
  delay(2000);

  digitalWrite (SMOKE, LOW);
  delay(2000);
  digitalWrite (SMOKE, HIGH);
  delay(1000);

  liftUP();
  delay(5000);

  digitalWrite (COOLER, LOW);
  delay (1500);

  //шоу руки
  servo5.write(130);
  delay (1000);
  servo6.write(5);
  delay (1000);
  servo6.write(60);
  delay (1000);
  //работаем, на позицию захвата
  servo5.write(45);
  servo6.write(5);//открыть захват
  delay (1000);
  servo4.write (120);
  delay (1000);
  //захват
  servo6.write(50);//закрыть захват
  delay (1000);
  //перекидка
  servo4.write (0);
  delay (3000);
  //открывание
  servo6.write(5);//открыть захват

  delay (3000);
  digitalWrite (COOLER, HIGH);
  servo4.write (40);//внртикально
  delay (2000);
  liftDown();
  delay(5000);

  servo4.detach ();
  servo5.detach ();
  servo6.detach ();
}

void liftUP() {
  digitalWrite (LIFTUP, LOW);
  delay(3000);
  digitalWrite (LIFTUP, HIGH);
  delay(300);
}

void liftDown() {
  digitalWrite (LIFTDOWN, LOW);
  delay(3000);
  digitalWrite (LIFTDOWN, HIGH);
  delay(300);
}

void test() {
  digitalWrite (BUBBLE, LOW);
  delay(5000);
  digitalWrite (BUBBLE, HIGH);
  delay(2000);

  digitalWrite (SMOKE, LOW);
  delay(2000);
  digitalWrite (SMOKE, HIGH);
  delay(2000);


  digitalWrite (BACKLIGHT, LOW);
  delay(3000);
  digitalWrite (BACKLIGHT, HIGH);
  delay(2000);


  digitalWrite (COOLER, LOW);
  delay(3000);
  digitalWrite (COOLER, HIGH);

  delay(10000);
}

