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

int adcs[cards] = {A0, A1, A2, A3};
int values[cards];
// enum card = {
//   stop,
//   adelante,
//   izquierda,
//   derecha
// }

void clear_register() {
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, clear);
  digitalWrite(latch, HIGH);
}

void set_register() {
  int value = 0;
  for (int i = cards - 1; i >= 0; i--) {
    value = (value)*2 + (values[i] > 0 ? 1 : 0);
  }

  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, value);
  digitalWrite(latch, HIGH);
}

void beep() {
  digitalWrite(buzzer, HIGH);
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);
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

  btSerial.begin(9600);
  Serial.begin(115200);
  Serial.println("...Start");

  for (int i = 0; i < 5; i++) {
    beep();
    delay(100);
  }

  clear_register();
}

void loop() {
  Serial.println("- - - - - - - - - - -  ");
  int adc = 0;
  for (int i = 0; i < cards; i++) {
    adc = analogRead(adcs[i]);
    values[i] = decoder(adc);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(values[i]);
    Serial.print(" ");
    Serial.println(adc);
    delay(10);
  }
  set_register();
  delay(100);
}