#include <SoftwareSerial.h>
#include <TimerOne.h>

#define led 13
#define buz 12
#define txPin 11
#define rxPin 10

#define m1n1 2
#define m1n2 3
#define m1n3 4
#define m1n4 5

#define m2n1 6
#define m2n2 7
#define m2n3 8
#define m2n4 9

#define m2n4 A0
#define m2n4 A1
#define m2n4 A2

static int move = 2048;
static int turn = 1024;

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

void off() {
  digitalWrite(m1n1, LOW);
  digitalWrite(m1n2, LOW);
  digitalWrite(m1n3, LOW);
  digitalWrite(m1n4, LOW);

  digitalWrite(m2n1, LOW);
  digitalWrite(m2n2, LOW);
  digitalWrite(m2n3, LOW);
  digitalWrite(m2n4, LOW);
}

void process_command() {
  char c;
  while (btSerial.available()) {
    c = btSerial.read();
    Serial.println(c);
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
    break;
  case st_fwd:
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
    turn_c++;
    if (turn_c >= turn) {
      state = st_idle;
    }
    stepper();
    break;
  case st_right:
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

  off();
  Timer1.initialize(1800); // uS
  Timer1.attachInterrupt(blink);
  btSerial.begin(9600);
  Serial.begin(115200);
  Serial.println("...Start");
}

void loop() { process_command(); }