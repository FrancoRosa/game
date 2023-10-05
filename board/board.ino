#define led 13

#define enable 5
#define data 2
#define clock 4
#define latch 3

#define i0 A0
#define i1 A1
#define i2 A2

#define button A3
#define buzzer A4
#define cards 4
#define reeds 3

int values[cards][reeds] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

bool running = false;
bool a = false;
int i = 0;
int card[] = {
    0b00000010, 0b00000100, 0b00001000, 0b00010000,

    // 0b11111101,
    // 0b11111011,
    // 0b11110111,
    // 0b11101111,
};
int clear = 0b00000000;
// int clear = 0b11111111;

int val = 0;

bool getval(int i) {
  // values[i][0] = digitalRead(i1);
  // values[i][1] = digitalRead(i0);
  // values[i][2] = digitalRead(i2);
  // return values[i][0] > 500 || values[i][1] > 500 || values[i][2] > 500;

  Serial.print(!digitalRead(i1));
  Serial.print(!digitalRead(i0));
  Serial.print(!digitalRead(i2));
  Serial.println();
}

void print_reeds() {
  for (int i = 0; i < cards; i++) {
    print_reed(i);
  }
}

void print_reed(int i) {
  Serial.print("card ");
  Serial.print(i);
  Serial.print(": ");
  Serial.print(values[i][0]);
  Serial.print(" ");
  Serial.print(values[i][1]);
  Serial.print(" ");
  Serial.print(values[i][2]);
  Serial.println();
}

void beep() {
  digitalWrite(buzzer, HIGH);
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);
}

void setup() {
  Serial.begin(9600);
  Serial.println("... start");

  pinMode(led, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);

  pinMode(enable, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  pinMode(i0, INPUT_PULLUP);
  pinMode(i1, INPUT_PULLUP);
  pinMode(i2, INPUT_PULLUP);

  digitalWrite(enable, LOW);
  digitalWrite(buzzer, LOW);
  for (int i = 0; i < 5; i++) {
    beep();
    delay(100);
  }
}

void clear_register() {
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, clear);
  digitalWrite(latch, HIGH);
}

void loop() {
  if (!digitalRead(button)) {
    running = !running;
    beep();
    print_reeds();
    delay(200);
  }

  for (int i = 0; i < 4; i++) {
    digitalWrite(latch, LOW);
    shiftOut(data, clock, MSBFIRST, card[i]);
    digitalWrite(latch, HIGH);

    getval(i);
    // print_reed(i);
    delay(5000);
  }
  // clear_register();
  // print_reeds();
  // delay(500);
}