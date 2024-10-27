
#define led 13

#define cards 4

void setup()
{
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  Serial.println("...Start");
}

int count = 0;
void loop()
{
  delay(1000);
  Serial.print("... count");
  Serial.print(count);
  Serial.println();
  count++;
  digitalWrite(led, !digitalRead(led));
}