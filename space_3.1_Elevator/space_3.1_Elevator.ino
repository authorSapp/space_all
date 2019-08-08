#include <RS485_protocol.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>


#define ACTIVATE LOW
#define NONE HIGH

#define VOLUME_SOUND 22 
#define JEROME_DELAY_1 400 //for stable signal from jerome
#define JEROME_DELAY_2 50 //for stable signal from jerome
#define LIGHT_DELAY 16*1000
#define DOOR_DELAY 10*1000

#define ENABLE_RS485_PIN 4 
#define DOOR_IN_1 7
#define DOOR_IN_2 8
#define DOOR_OUT_1 9
#define DOOR_OUT_2 10
#define LED 11
#define J_IND1 A0
#define J_IND2 A1
#define J_IND3 A2
#define JACK A4
#define CODE_PANEL A5


SoftwareSerial rs485(2, 3); 
SoftwareSerial Sound(5, 6); 
DFRobotDFPlayerMini Elevator_Sound;

enum FromJerome:uint8_t { 
                OPEN_DOOR_IN = 0,
                OPEN_DOOR_OUT,
                CLOSE_ALL_DOOR,
                E_FROMJEROME_LENGTH
              };

struct PinJerome {
  boolean state = false;
  uint32_t timer = 0;
};

PinJerome pinJerome[E_FROMJEROME_LENGTH];
uint32_t ms_timer = 0;
uint32_t light_timer = 0;
uint32_t door_in_timer = 0;
uint32_t door_out_timer = 0;

boolean light_flag = false;
boolean jack_flag = false;
boolean code_panel_flag = false;



void setup() {

  delay(500);
  Serial.begin(9600);
  init_GPIO();
  init_mp3_players();
  init_rs485();
  delay(500);

}


void loop() {

//  Serial.print(analogRead(CODE_PANEL));
//  Serial.print("|");
//  Serial.print(analogRead(J_IND1));
//  Serial.print("|");
//  Serial.print(analogRead(J_IND2));
//  Serial.print("|");
//  Serial.println(analogRead(J_IND3));
  main_controller();
  jerome_controller();
  light_controller();
  door_controller();

}



void main_controller() {

  if (jack_flag == false) {
    if (digitalRead(JACK) == HIGH) {
      jack_flag = true;
      open_door(DOOR_IN_1, DOOR_IN_2);
    }
  }

  if (jack_flag == true) {
    if (digitalRead(JACK) == LOW) {
      jack_flag = false;
    }
  }


  if (code_panel_flag == false) {
    if (analogRead(CODE_PANEL) > 300) {
      code_panel_flag = true;
      light_flag = true;
    }
  }

  if (code_panel_flag == true) {
    if (analogRead(CODE_PANEL) < 300) {
      code_panel_flag = false;
    }
  }

}


void jerome_controller() {

  ms_timer = millis();

  if(analogRead(J_IND1) > 500 && pinJerome[OPEN_DOOR_IN].state == false) {
    pinJerome[OPEN_DOOR_IN].timer = ms_timer;
    pinJerome[OPEN_DOOR_IN].state = true;
  }

  if(analogRead(J_IND2) > 500 && pinJerome[OPEN_DOOR_OUT].state == false) {
    pinJerome[OPEN_DOOR_OUT].timer = ms_timer;
    pinJerome[OPEN_DOOR_OUT].state = true;
  }

  if(analogRead(J_IND3) > 500 && pinJerome[CLOSE_ALL_DOOR].state == false) {
    pinJerome[CLOSE_ALL_DOOR].timer = ms_timer;
    pinJerome[CLOSE_ALL_DOOR].state = true;
  }


  if ((analogRead(J_IND1) > 500) && (ms_timer - pinJerome[OPEN_DOOR_IN].timer > JEROME_DELAY_1)) {
    open_door(DOOR_IN_1, DOOR_IN_2);
  }

  if ((analogRead(J_IND2) > 500) && (ms_timer - pinJerome[OPEN_DOOR_OUT].timer > JEROME_DELAY_1)) {
    open_door(DOOR_OUT_1, DOOR_OUT_2);
  }

  if ((analogRead(J_IND3) > 500) && (ms_timer - pinJerome[CLOSE_ALL_DOOR].timer > JEROME_DELAY_1)) {
    close_door(DOOR_IN_1, DOOR_IN_2);
    close_door(DOOR_OUT_1, DOOR_OUT_2);
  }


  if(analogRead(J_IND1) < 500 && pinJerome[OPEN_DOOR_IN].state == true && (ms_timer - pinJerome[OPEN_DOOR_IN].timer) > JEROME_DELAY_2) {
    pinJerome[OPEN_DOOR_IN].timer = ms_timer;
    pinJerome[OPEN_DOOR_IN].state = false;
  }

  if(analogRead(J_IND2) < 500 && pinJerome[OPEN_DOOR_OUT].state == true && (ms_timer - pinJerome[OPEN_DOOR_OUT].timer) > JEROME_DELAY_2) {
    pinJerome[OPEN_DOOR_OUT].timer = ms_timer;
    pinJerome[OPEN_DOOR_OUT].state = false;
  }

  if(analogRead(J_IND3) < 500 && pinJerome[CLOSE_ALL_DOOR].state == true && (ms_timer - pinJerome[CLOSE_ALL_DOOR].timer) > JEROME_DELAY_2) {
    pinJerome[CLOSE_ALL_DOOR].timer = ms_timer;
    pinJerome[CLOSE_ALL_DOOR].state = false;
  }

}


void light_controller() {

  if (light_flag == true) {
    light_flag = false;
    light_timer = millis();
    digitalWrite(LED, ACTIVATE);
    Elevator_Sound.play(1);
  }

  if (light_timer != 0) {
    if((light_flag == true) && (millis() - light_timer > LIGHT_DELAY)) {
      digitalWrite(LED, NONE);
      open_door(DOOR_OUT_1, DOOR_OUT_2);
      Elevator_Sound.stop();
      light_timer = 0;
    }
  }

}


void door_controller() {

  if (door_in_timer == 0) {
    if(digitalRead(DOOR_IN_1) != digitalRead(DOOR_IN_2)) {
      door_in_timer = millis();
    }
  }

  if (door_out_timer == 0) {
    if(digitalRead(DOOR_OUT_1) != digitalRead(DOOR_OUT_2)) {
      door_out_timer = millis();
    }
  }

  if(millis() - door_in_timer > DOOR_DELAY) {
    stop_door(DOOR_IN_1, DOOR_IN_2);
    door_in_timer = 0;
  }

  if(millis() - door_out_timer > DOOR_DELAY) {
    stop_door(DOOR_OUT_1, DOOR_OUT_2);
    door_out_timer = 0;
  }

}


void open_door(uint8_t pin1, uint8_t pin2) {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
}

void close_door(uint8_t pin1, uint8_t pin2) {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
}

void stop_door(uint8_t pin1, uint8_t pin2) {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
}


void init_GPIO() {
  pinMode(DOOR_IN_1, OUTPUT);
  pinMode(DOOR_IN_2, OUTPUT);
  pinMode(DOOR_OUT_1, OUTPUT);
  pinMode(DOOR_OUT_2, OUTPUT);
  pinMode(LED, OUTPUT);

  pinMode(J_IND1, INPUT);
  pinMode(J_IND2, INPUT);
  pinMode(J_IND3, INPUT);
  pinMode(JACK, INPUT);
  pinMode(CODE_PANEL, INPUT);


  digitalWrite(DOOR_IN_1, HIGH);
  digitalWrite(DOOR_IN_2, HIGH);
  digitalWrite(DOOR_OUT_1, HIGH);
  digitalWrite(DOOR_OUT_2, HIGH);
  digitalWrite(LED, HIGH);
}


void init_mp3_players() {
  Sound.begin (9600);
  Elevator_Sound.begin(Sound);
  Elevator_Sound.volume(VOLUME_SOUND);
}


void init_rs485 ()  {
  rs485.begin (9600);
  pinMode (ENABLE_RS485_PIN, OUTPUT); 
}

void fWrite (const byte what) {
  rs485.write (what);  
}
  
int fAvailable ()  {
  return rs485.available ();  
}

int fRead ()  {
  return rs485.read ();  
}



