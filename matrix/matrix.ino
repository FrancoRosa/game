#include <SoftwareSerial.h>

#define led 13

#define data 2
#define enable 3
#define latch 4
#define clock 5

#define button 9
#define buzzer 12

#define rxPin 10
#define txPin 11

#define cards 4

SoftwareSerial btSerial(rxPin, txPin);

int decoder(int adc) {
  int levels[] = {100, 200, 300, 400, 500, 600, 680, 800};
  for (int i = 0; i < 8; i++) {
    if (adc < levels[i]) {
      return i;
    }
  }
}

int clear = 0b00000000;
int indicators[] = {0b00000001, 0b00000010, 0b00000100, 0b00001000,
                    0b00010000, 0b00100000, 0b01000000, 0b10000000};

int adcs[cards] = {A0, A1, A2, A3};
int values[cards];

void clear_register() {
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, clear);
  digitalWrite(latch, HIGH);
}

void set_register(int value) {
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, value);
  digitalWrite(latch, HIGH);
}

void set_cards() {
  int value = 0;
  for (int i = cards - 1; i >= 0; i--) {
    value = (value)*2 + (values[i] > 0 ? 1 : 0);
  }
  set_register(value);
}

void send_command(int command) { Serial.println(command); }

void beep() {
  digitalWrite(buzzer, HIGH);
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);
}

void send_commands() {
  Serial.println("...sending commands");
  beep_beep(3);
  clear_register();
  delay(1000);
  for (int i = 0; i < cards; i++) {
    set_register(indicators[i]);
    send_command(values[i]);
    beep();
    delay(1000);
  }
  delay(1000);
  beep_beep(3);
  Serial.println("...completed");
}

void beep_beep(int t) {
  for (int i = 0; i < t; i++) {
    beep();
    delay(100);
  }
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  pinMode(data, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);

  analogReference(INTERNAL);

  digitalWrite(enable, LOW);
  digitalWrite(buzzer, LOW);

  clear_register();

  btSerial.begin(9600);
  Serial.begin(115200);
  Serial.println("...Start");
  beep_beep(3);
}

void read_cards() {
  int adc = 0;
  for (int i = 0; i < cards; i++) {
    adc = analogRead(adcs[i]);
    values[i] = decoder(adc);
    delay(10);
  }
}

void loop() {
  read_cards();
  set_cards();
  if (!digitalRead(button)) {
    digitalWrite(led, !digitalRead(led));
    send_commands();
    while (!digitalRead(button)) {
      delay(10);
    }
  }
}