#include <SoftwareSerial.h>
#include <TimerOne.h>

#define ledPin 13
#define rxPin 10
#define txPin 11

#define mr1l 2
#define mr1r 3
#define ml1l 6
#define ml1r 7

int counter = 0;

enum state_t { st_idle, st_bck, st_fwd, st_right, st_left };

state_t state = st_idle;

SoftwareSerial btSerial(rxPin, txPin);

void fwr() {
  digitalWrite(mr1l, HIGH);
  digitalWrite(ml1l, HIGH);
}

void bck() {
  digitalWrite(mr1r, HIGH);
  digitalWrite(ml1r, HIGH);
}

void lft() {
  digitalWrite(mr1l, HIGH);
  digitalWrite(ml1r, HIGH);
}

void rgt() {
  digitalWrite(mr1r, HIGH);
  digitalWrite(ml1l, HIGH);
}

void off() {
  digitalWrite(mr1l, LOW);
  digitalWrite(mr1r, LOW);
  digitalWrite(ml1l, LOW);
  digitalWrite(ml1r, LOW);
}

void process_command() {
  char c;
  while (btSerial.available()) {
    c = btSerial.read();
    Serial.println(c);
    counter = 0;
    switch (c) {
    case 'f':
      if (state != st_fwd)
        off();
      state = st_fwd;
      break;
    case 'b':
      if (state != st_bck)
        off();
      state = st_bck;
      break;
    case 'l':
      if (state != st_left)
        off();
      state = st_left;
      break;
    case 'r':
      if (state != st_right)
        off();
      state = st_right;
      break;

    default:
      state = st_idle;
      break;
    }
  }
}

void blink() {
  digitalWrite(led, !digitalRead(led));
  counter++;
  if (counter > 4) {
    counter = 0;
    state = st_idle;
  }
  switch (state) {
  case st_idle:
    off();
    break;
  case st_fwd:
    fwr();
    break;
  case st_bck:
    bck();
    break;
  case st_left:
    lft();
    break;
  case st_right:
    rgt();
    break;
  default:
    break;
  }
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(mr1l, OUTPUT);
  pinMode(mr1r, OUTPUT);
  pinMode(ml1l, OUTPUT);
  pinMode(ml1r, OUTPUT);
  off();
  Timer1.initialize(100000); // uS
  Timer1.attachInterrupt(blink);
  btSerial.begin(9600);
  Serial.begin(115200);
  Serial.println("...Start");
}

void loop() { process_command(); }