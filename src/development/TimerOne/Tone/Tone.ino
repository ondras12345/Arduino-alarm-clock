#define pin_buzzer 9
void setup() {
  // put your setup code here, to run once:
  //pinMode(pin_buzzer, OUTPUT); // not needed
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(pin_buzzer, 1000);
  delay(1000);
  noTone(pin_buzzer);
  delay(1000);
}
