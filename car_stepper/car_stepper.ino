#include <SoftwareSerial.h>
#include <TimerOne.h>

#define led 13

#define buzzer A4
#define led_lft A3
#define led_fwd A2
#define led_pwr A1
#define led_rgt A0

#define txPin 10
#define rxPin 11

#define m1n1 5
#define m1n2 4
#define m1n3 3
#define m1n4 2

#define m2n1 6
#define m2n2 7
#define m2n3 8
#define m2n4 9

static int move = 2048 + 512;
static int turn = 512 + 14;

int dir_1 = 0;
int dir_2 = 0;
int seq_1 = 0;
int seq_2 = 0;
int move_c = 0;
int turn_c = 0;
int on = 0;

enum state_t { st_idle, st_bck, st_fwd, st_right, st_left };

state_t state = st_idle;

SoftwareSerial btSerial(rxPin, txPin);

void beep() {
  digitalWrite(buzzer, HIGH);
  digitalWrite(led, HIGH);
  delay(30);
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);
}

void beep_beep(int t) {
  for (int i = 0; i < t; i++) {
    beep();
    delay(50);
  }
}

void off() {
  digitalWrite(m1n1, LOW);
  digitalWrite(m1n2, LOW);
  digitalWrite(m1n3, LOW);
  digitalWrite(m1n4, LOW);

  digitalWrite(m2n1, LOW);
  digitalWrite(m2n2, LOW);
  digitalWrite(m2n3, LOW);
  digitalWrite(m2n4, LOW);

  digitalWrite(led_pwr, HIGH);
}

void off_led() {
  digitalWrite(led_fwd, LOW);
  digitalWrite(led_lft, LOW);
  digitalWrite(led_rgt, LOW);
}

void process_command() {
  char c;
  while (btSerial.available()) {
    c = btSerial.read();
    Serial.println(c);
    beep_beep(2);

    switch (c) {
    case 'f':
      if (state != st_fwd)
        off();
      state = st_fwd;
      dir_1 = 0;
      dir_2 = 0;
      move_c = 0;
      break;
    case 'b':

      if (state != st_bck)
        off();
      state = st_bck;
      dir_1 = 1;
      dir_2 = 1;
      move_c = 0;
      break;
    case 'l':

      if (state != st_left)
        off();
      state = st_left;
      dir_1 = 1;
      dir_2 = 0;
      turn_c = 0;

      break;
    case 'r':

      if (state != st_right)
        off();
      dir_1 = 0;
      dir_2 = 1;
      turn_c = 0;
      state = st_right;

      break;
    case 's':
      state = st_idle;
      off();
      break;

    default:
      state = st_idle;
      off();
      break;
    }
  }
}

int sequence[] = {
    0b00001100,
    0b00000110,
    0b00000011,
    0b00001001,
};

void stepper() {
  seq_1 = dir_1 ? seq_1 + 1 : seq_1 - 1;
  seq_2 = dir_2 ? seq_2 + 1 : seq_2 - 1;
  if (seq_2 > 3)
    seq_2 = 0;
  if (seq_1 > 3)
    seq_1 = 0;
  if (seq_2 < 0)
    seq_2 = 3;
  if (seq_1 < 0)
    seq_1 = 3;
  digitalWrite(m1n1, (sequence[seq_1] & 1));
  digitalWrite(m1n2, (sequence[seq_1] >> 1) & 1);
  digitalWrite(m1n3, (sequence[seq_1] >> 2) & 1);
  digitalWrite(m1n4, (sequence[seq_1] >> 3) & 1);

  digitalWrite(m2n1, (sequence[seq_2] & 1));
  digitalWrite(m2n2, (sequence[seq_2] >> 1) & 1);
  digitalWrite(m2n3, (sequence[seq_2] >> 2) & 1);
  digitalWrite(m2n4, (sequence[seq_2] >> 3) & 1);
}

void blink() {
  digitalWrite(led, !digitalRead(led));

  switch (state) {
  case st_idle:
    off();
    off_led();
    break;
  case st_fwd:
    digitalWrite(led_fwd, HIGH);
    move_c++;
    if (move_c >= move) {
      state = st_idle;
    }
    stepper();

    break;
  case st_bck:
    move_c++;
    if (move_c >= move) {
      state = st_idle;
    }
    stepper();
    break;
  case st_left:
    digitalWrite(led_lft, HIGH);

    turn_c++;
    if (turn_c >= turn) {
      state = st_idle;
    }
    stepper();
    break;
  case st_right:
    digitalWrite(led_rgt, HIGH);

    turn_c++;
    if (turn_c >= turn) {
      state = st_idle;
    }
    stepper();
    break;
  default:
    break;
  }
}

void setup() {

  pinMode(led, OUTPUT);

  pinMode(m1n1, OUTPUT);
  pinMode(m1n2, OUTPUT);
  pinMode(m1n3, OUTPUT);
  pinMode(m1n4, OUTPUT);

  pinMode(m2n1, OUTPUT);
  pinMode(m2n2, OUTPUT);
  pinMode(m2n3, OUTPUT);
  pinMode(m2n4, OUTPUT);

  pinMode(led_lft, OUTPUT);
  pinMode(led_fwd, OUTPUT);
  pinMode(led_pwr, OUTPUT);
  pinMode(led_rgt, OUTPUT);

  off();
  // Timer1.initialize(18000); // uS
  Timer1.initialize(1800); // uS
  Timer1.attachInterrupt(blink);
  btSerial.begin(9600);
  Serial.begin(115200);
  beep_beep(3);
  Serial.println("...Start car");
}

void loop() { process_command(); }