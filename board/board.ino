
#define led 13
// shift register pinout
#define enable 5
#define data 2  // ds data (14)
#define clock 4 //  (11) SHCP
#define latch 3 //  (12) STCP

#define i0 A0
#define i1 A1
#define i2 A2

bool a = false;
int i = 0;
int card[] = {
    0b00000010,
    0b00000100,
    0b00001000,
    0b00010000,
};

int val = 0;

int getval() {
  Serial.print(analogRead(i2));
  Serial.print(" ");
  Serial.print(analogRead(i1));
  Serial.print(" ");
  Serial.println(analogRead(i0));
}

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial.println("... start");

  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);

  pinMode(enable, OUTPUT);

  digitalWrite(enable, LOW);
}

void loop() {

  for (int i = 0; i < 4; i++) {
    digitalWrite(latch, LOW);
    shiftOut(data, clock, MSBFIRST, card[i]);
    digitalWrite(latch, HIGH);
    Serial.print(i);
    Serial.print(":    ");
    delay(100);
    getval();
    delay(1000);
  }
}