#include <TimerOne.h>
// https://github.com/PaulStoffregen/TimerOne
// can be installed using Arduino IDE's library manager

// # TODO test !!

#define pin_buzzer 9

unsigned long previousMillis = 0;
unsigned int previousAmplitude = 0;
boolean beeping = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Timer1.initialize();

  _tone(1000, 250);
  delay(1000);
  _tone(1000, 500);
  delay(1000);
  _noTone();
  Serial.println("boot");
}

void loop() {
  // put your main code here, to run repeatedly:

  if ((unsigned long)(millis() - previousMillis) >= 500) {
    if (beeping) {
      _noTone();
    }
    else {
      prevousAmplitude += 100;
      if (previousAmplitude >= 1000) previousAmplitude = 0;
      _tone(1000, previousAmplitude);
  }



  Serial.println("t");
  // this tests if Serial can be used normally
  // (there were some issues reported in the past)
  delay(500);

}

void _tone(unsigned int frequency, unsigned int duty) {
  if (duty > 1000) duty = 1000; // 100% duty (1023) wouldn't make any sound

  unsigned long period = 1000000 / frequency;

  Timer1.setPeriod(period);
  Timer1.PWM(pin_buzzer, duty);

}

void _noTone() {
  Timer1.disablePWM(pin_buzzer);
}
