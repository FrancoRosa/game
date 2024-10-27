#define swh 2
#define clk 3
#define dta 4
#include "HID-Project.h"

enum { idle, vol_up, vol_down, play_pause };

int state = idle;

void setup() {
  Keyboard.begin();
  pinMode(swh, INPUT_PULLUP);
  pinMode(clk, INPUT_PULLUP);
  pinMode(dta, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(swh), handleSWH, FALLING);
  attachInterrupt(digitalPinToInterrupt(clk), handleCLK, FALLING);
}

void handleSWH() { state = play_pause; }

void handleCLK() { state = digitalRead(dta) ? vol_up : vol_down; }

void loop() {
  switch (state) {
  case idle:
    delay(50);
    break;
  case vol_up:
    Keyboard.write(MEDIA_VOL_UP);
    state = idle;
    break;
  case vol_down:
    Keyboard.write(MEDIA_VOL_DOWN);
    state = idle;
    break;
  case play_pause:
    Keyboard.write(MEDIA_PLAY_PAUSE);
    state = idle;
    break;
  default:
    state = idle;
    break;
  }
}
