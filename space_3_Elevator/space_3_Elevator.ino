
#define JEROME_DELAY_1 400 //for stable signal from jerome
#define JEROME_DELAY_2 50 //for stable signal from jerome
#define LIGHT_DELAY 16*1000

#define DOOR_IN 2
#define DOOR_OUT 3
#define LED 4
#define JACK 8
#define J_IND1 A0
#define J_IND2 A1
#define J_IND3 A2
#define CODE_PANEL A5

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

boolean light_flag = false;
boolean jack_flag = false;
boolean code_panel_flag = false;
boolean open_door_flag = false;



void setup() {

  delay(500);
  Serial.begin(9600);
  init_GPIO();
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

}



void main_controller() {

  if (jack_flag == false) {
    if (digitalRead(JACK) == HIGH) {
      jack_flag = true;
      digitalWrite(DOOR_IN, HIGH);
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
    digitalWrite(DOOR_IN, HIGH);
  }

  if ((analogRead(J_IND2) > 500) && (ms_timer - pinJerome[OPEN_DOOR_OUT].timer > JEROME_DELAY_1)) {
    digitalWrite(DOOR_OUT, HIGH);
  }

  if ((analogRead(J_IND3) > 500) && (ms_timer - pinJerome[CLOSE_ALL_DOOR].timer > JEROME_DELAY_1)) {
    digitalWrite(DOOR_IN, LOW);
    digitalWrite(DOOR_OUT, LOW);
    open_door_flag = false;
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
    digitalWrite(LED, HIGH);
    open_door_flag = true;
  }

  if((light_flag == false) && (millis() - light_timer > LIGHT_DELAY)) {
    digitalWrite(LED, LOW);
    if(open_door_flag == true) {
      digitalWrite(DOOR_OUT, HIGH);
      open_door_flag = false;
    }
  }

}



void init_GPIO() {
  pinMode(DOOR_IN, OUTPUT);
  pinMode(DOOR_OUT, OUTPUT);
  pinMode(LED, OUTPUT);

  pinMode(JACK, INPUT);
  pinMode(J_IND1, INPUT);
  pinMode(J_IND2, INPUT);
  pinMode(J_IND3, INPUT);
  pinMode(CODE_PANEL, INPUT);


  digitalWrite(DOOR_IN, LOW);
  digitalWrite(DOOR_OUT, LOW);
  digitalWrite(LED, LOW);
}




