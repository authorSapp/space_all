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

#include <Servo.h>

#define ACTIVATE LOW
#define NONE HIGH
#define OPEN HIGH
#define CLOSE LOW
#define START LOW
#define STOP HIGH

#define JEROME_DELAY_1 400 //for stable signal from jerome
#define JEROME_DELAY_2 50 //for stable signal from jerome
#define START_ANGLE 10
#define openHand 40
#define closeHand 140


#define J_ELEM A0 //pulldown
#define ELEMENTS_READY A1 //pulldown
#define FRIDGE_DOOR A2 //pullup

#define SMOKE 2
#define SERVO_BASIC 3
#define BUBBLE 4
#define SERVO_ROTATE 5
#define SERVO_HAND 6
#define LIFTUP 7
#define LIFTDOWN 8
#define BACKLIGHT 9
#define COOLER 10
#define SERVO_POWER 11

#define DEBUG_PIN 13



Servo servo_basic;
Servo servo_rotate;
Servo servo_hand;

enum FromJerome:uint8_t { 
  OPERATOR_CMD = 0,
  E_FROMJEROME_LENGTH
};

struct PinJerome {
  boolean state = false;
  uint32_t timer = 0;
};

PinJerome pinJerome[E_FROMJEROME_LENGTH];
uint32_t ms_timer = 0;

boolean action_flag = false;
boolean operator_command = false;
boolean init_to_begin_flag = false;

void main_controller();
void jerome_controller();
void initToBegin();
void init_GPIO();
void init_servo();
void megaSuperGiperTrooperEffect();
void liftUP();
void liftDown();
void speedRotateServo(Servo &servo, uint8_t velocity, uint8_t degree);
void debug();



void setup() {

  delay(500);
  Serial.begin(9600);
  init_GPIO();
  init_servo();
  delay(500);

}


void loop() {

  if (digitalRead(DEBUG_PIN) == ACTIVATE) {
    debug();
  }
  main_controller();
  jerome_controller();

}



void main_controller() {
  if (action_flag == false) {
    if ((digitalRead(ELEMENTS_READY) == HIGH) && (digitalRead(FRIDGE_DOOR) == CLOSE)) {
      action_flag = true;
      init_to_begin_flag = false;
      megaSuperGiperTrooperEffect();
    }
  }

  if ((operator_command == true) && (digitalRead(FRIDGE_DOOR) == CLOSE)) {
    operator_command = false;
    init_to_begin_flag = false;
    megaSuperGiperTrooperEffect();
  }

  if (init_to_begin_flag == false) {
    if ((operator_command == true) && (digitalRead(FRIDGE_DOOR) == OPEN)) {
     init_to_begin_flag = true;
     initToBegin();
    }
  }
  
}


void jerome_controller() {

  ms_timer = millis();

  if(analogRead(J_ELEM) > 500 && pinJerome[OPERATOR_CMD].state == false) {
    pinJerome[OPERATOR_CMD].timer = ms_timer;
    pinJerome[OPERATOR_CMD].state = true;
  }

  if ((analogRead(J_ELEM) > 500) && (ms_timer - pinJerome[OPERATOR_CMD].timer > JEROME_DELAY_1)) {
    operator_command = true;
  }

  if(analogRead(J_ELEM) < 500 && pinJerome[OPERATOR_CMD].state == true && (ms_timer - pinJerome[OPERATOR_CMD].timer) > JEROME_DELAY_2) {
    pinJerome[OPERATOR_CMD].timer = ms_timer;
    pinJerome[OPERATOR_CMD].state = false;
  }

}


void initToBegin() {
  action_flag = false;
  operator_command = false;
  digitalWrite(SERVO_POWER, START);
  delay(200);
//  speedRotateServo(servo_basic, 1, 10);
//  speedRotateServo(servo_rotate, 1, 10);
//  speedRotateServo(servo_hand, 1, 10);
  servo_basic.write(START_ANGLE);
  servo_rotate.write(START_ANGLE);
  servo_hand.write(START_ANGLE);
  delay(100);
  digitalWrite(SERVO_POWER, HIGH);
  
  digitalWrite(SMOKE, HIGH);
  digitalWrite(BUBBLE, HIGH);
  digitalWrite(LIFTUP, HIGH);
  digitalWrite(LIFTDOWN, HIGH);
  digitalWrite(BACKLIGHT, HIGH);
  digitalWrite(COOLER, HIGH);

}



void init_GPIO() {
  pinMode(J_ELEM, INPUT);
  pinMode(ELEMENTS_READY, INPUT);
  pinMode(FRIDGE_DOOR, INPUT_PULLUP);
  pinMode(DEBUG_PIN, INPUT_PULLUP);
  pinMode(SMOKE, OUTPUT);
  pinMode(BUBBLE, OUTPUT);
  pinMode(LIFTUP, OUTPUT);
  pinMode(LIFTDOWN, OUTPUT);
  pinMode(BACKLIGHT, OUTPUT);
  pinMode(COOLER, OUTPUT);
  pinMode(SERVO_POWER, OUTPUT);

  digitalWrite(SMOKE, HIGH);
  digitalWrite(BUBBLE, HIGH);
  digitalWrite(LIFTUP, HIGH);
  digitalWrite(LIFTDOWN, HIGH);
  digitalWrite(BACKLIGHT, HIGH);
  digitalWrite(COOLER, HIGH);
  digitalWrite(SERVO_POWER, HIGH);
}

void init_servo() {
  servo_basic.attach(SERVO_BASIC);
  servo_rotate.attach(SERVO_ROTATE);
  servo_hand.attach(SERVO_HAND);
  
  servo_basic.write(START_ANGLE);
  servo_rotate.write(START_ANGLE);
  servo_hand.write(START_ANGLE);
}



void megaSuperGiperTrooperEffect() {

  digitalWrite(SERVO_POWER, START);
  delay(500);
  //рука вертикально
//  speedRotateServo(servo_basic, 1, 90);
  servo_basic.write(90);
  delay(1000);
  
  digitalWrite(BUBBLE, START);
  delay(500);
  digitalWrite(BACKLIGHT, START);
  delay(7000);
  digitalWrite(BUBBLE, STOP);
  delay(500);
  digitalWrite(BACKLIGHT, STOP);
  delay(2000);

  digitalWrite(SMOKE, START);
  delay(2000);
  digitalWrite(SMOKE, STOP);
  delay(1000);

  liftUP();
  delay(4000);

  digitalWrite (COOLER, START);
  delay(1500);

  //шоу руки
//  speedRotateServo(servo_rotate, 2, 130);
  servo_rotate.write(130);
  delay(1000);
//  speedRotateServo(servo_hand, 1, 60);
  servo_hand.write(60);
  delay(1000);
//  speedRotateServo(servo_rotate, 1, 50);
  servo_rotate.write(50);
  delay(1000);
//  speedRotateServo(servo_hand, 1, 5);
  servo_hand.write(5);
  delay(1000);
//  speedRotateServo(servo_rotate, 1, 10);
  servo_rotate.write(10);
  delay(1000);
  //работаем, на пзицию захвата
//  speedRotateServo(servo_basic, 3, 160);
  servo_basic.write(150);
  delay(1000);
//  speedRotateServo(servo_hand, 2, 40);
  servo_hand.write(40);
  delay(1000);
  //перекидка
//  speedRotateServo(servo_basic, 4, 10);
  servo_basic.write(10);
  delay(1000);
  //открывание
//  speedRotateServo(servo_hand, 1, 5);
  servo_hand.write(5);
  delay(3000);

  digitalWrite(COOLER, STOP);
//  speedRotateServo(servo_basic, 1, 90);
  servo_basic.write(90);
  delay(2000);

  liftDown();
  delay(1000);
  digitalWrite(SERVO_POWER, STOP);
  delay(500);

}


void liftUP() {
  digitalWrite(LIFTUP, START);
  delay(3000);
  digitalWrite(LIFTUP, STOP);
  delay(300);
}

void liftDown() {
  digitalWrite(LIFTDOWN, START);
  delay(3000);
  digitalWrite(LIFTDOWN, STOP);
  delay(300);
}

void speedRotateServo(Servo &servo, uint8_t velocity, uint8_t degree) {
  
  uint8_t currentAngle = servo.read();
  uint8_t angleRotate = abs(degree - currentAngle);
  if (angleRotate == 0) {
    return;
  }
  uint8_t partDelay = (velocity * 1000) / abs(degree - currentAngle);
  
  if (degree > currentAngle) {
    for (uint8_t i = currentAngle; i <= degree; i++) {
      servo.write(i);
      delay(partDelay);
    }
  }
  if (degree < currentAngle) {
    for (uint8_t i = currentAngle; i >= degree; i--) {
      servo.write(i);
      delay(partDelay);
    }
  }

}


void debug() {
  uint8_t select_test = 0;

  while (1) {
    if (Serial.available() > 0) {
      select_test = Serial.parseInt();
    }
    switch (select_test) {
      
      case 0:
        break;

      case 1: 
        Serial.println(analogRead(J_ELEM));
        delay(1000);
        break;

      case 2:
        liftUP();
        delay(2000);
        liftDown();
        delay(2000);
        select_test = 0;
        break;

      case 3:
        digitalWrite(BUBBLE, START);
        delay(2000);
        digitalWrite(BUBBLE, STOP);
        delay(2000);

        digitalWrite(SMOKE, START);
        delay(2000);
        digitalWrite(SMOKE, STOP);
        delay(2000);

        digitalWrite(BACKLIGHT, START);
        delay(2000);
        digitalWrite(BACKLIGHT, STOP);
        delay(2000);

        digitalWrite(COOLER, START);
        delay(2000);
        digitalWrite(COOLER, STOP);
        delay(2000);
        select_test = 0;
        break;

      case 4:
        digitalWrite(SERVO_POWER, START);
        delay(500);
        //рука вертикально
        speedRotateServo(servo_basic, 1, 90);
        delay(1000);
        //шоу руки
        speedRotateServo(servo_rotate, 2, 130);
        speedRotateServo(servo_hand, 1, 60);
        speedRotateServo(servo_rotate, 1, 50);
        speedRotateServo(servo_hand, 1, 0);
        speedRotateServo(servo_rotate, 1, 10);
        delay(1000);
        //работаем, на позицию захвата
        speedRotateServo(servo_basic, 3, 160);
        speedRotateServo(servo_hand, 2, 40);
        //перекидка
        speedRotateServo(servo_basic, 4, 10);
        //открывание
        speedRotateServo(servo_hand, 1, 5);
        speedRotateServo(servo_basic, 1, 90);
        digitalWrite(SERVO_POWER, STOP);
        delay(1000);
        select_test = 0;
        break;

      case 5:
        Serial.print(digitalRead(ELEMENTS_READY));
        Serial.print(" | ");
        Serial.print(digitalRead(FRIDGE_DOOR));
        Serial.println();
        delay(256);
        break;
        
      default:
        return;
        break;

    }
  }
}



