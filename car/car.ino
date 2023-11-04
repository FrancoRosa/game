#include <SoftwareSerial.h>
#include <TimerOne.h>

#define led 13
#define rxPin 10
#define txPin 11

#define encl 2
#define encr 3
#define mr1l 4 // 2
#define mr1r 5 // 3
#define ml1l 6
#define ml1r 7

#define pwm 9

static int move = 70;
static int turn = 25;

int counter = 0;
int pulses_l = 0;
int pulses_r = 0;

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

void off_l() {
  digitalWrite(ml1l, LOW);
  digitalWrite(ml1r, LOW);
}

void off_r() {
  digitalWrite(mr1l, LOW);
  digitalWrite(mr1r, LOW);
}

void pulse_l() {
  Serial.print("l");
  Serial.println(pulses_l);
  pulses_l--;
  if (pulses_l <= 0) {

    Serial.println();
    Serial.println("Apaga l");
    off_l();
    if (pulses_r <= 0) {
      off();
      state = st_idle;
    }
  }
}

void pulse_r() {
  Serial.print("r");
  Serial.println(pulses_l);

  pulses_r--;
  if (pulses_r <= 0) {
    Serial.println();
    Serial.println("Apaga r");
    off_r();
    if (pulses_l <= 0) {
      off();
      state = st_idle;
    }
  }
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
      pulses_l = move;
      pulses_r = move;
      break;
    case 'b':
      if (state != st_bck)
        off();
      state = st_bck;
      pulses_l = move;
      pulses_r = move;
      break;
    case 'l':
      if (state != st_left)
        off();
      state = st_left;
      pulses_l = turn;
      pulses_r = turn;
      break;
    case 'r':
      if (state != st_right)
        off();
      state = st_right;
      pulses_l = turn;
      pulses_r = turn;
      break;
    case 's':
      off();
      state = st_idle;
      pulses_l = 0;
      pulses_r = 0;
      break;

    default:
      state = st_idle;
      break;
    }
  }
}

void blink() {
  digitalWrite(led, !digitalRead(led));
  // counter++;
  // if (counter > 4)
  // {
  //   counter = 0;
  //   state = st_idle;
  // }
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
  pinMode(encl, INPUT_PULLUP);
  pinMode(encr, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encl), pulse_l, RISING);
  attachInterrupt(digitalPinToInterrupt(encr), pulse_r, RISING);
  pinMode(led, OUTPUT);
  pinMode(mr1l, OUTPUT);
  pinMode(mr1r, OUTPUT);
  pinMode(ml1l, OUTPUT);
  pinMode(ml1r, OUTPUT);

  pinMode(pwm, OUTPUT);
  analogWrite(pwm, 254);

  off();
  Timer1.initialize(10000); // uS
  Timer1.attachInterrupt(blink);
  btSerial.begin(9600);
  Serial.begin(115200);
  Serial.println("...Start");
}

void loop() { process_command(); }