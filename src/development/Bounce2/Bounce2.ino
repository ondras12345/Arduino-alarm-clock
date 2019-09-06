#include <Bounce2.h>
// https://github.com/thomasfredericks/Bounce2
// can be installed using Arduino IDE's library manager
#define pin_button 7

Bounce debouncer;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(pin_button, INPUT_PULLUP);
  
  debouncer.attach(pin_button, INPUT_PULLUP);
  debouncer.interval(5); // interval in ms
  Serial.println("boot");
}

void loop() {
  // put your main code here, to run repeatedly:
  debouncer.update();
  if(debouncer.fell()) Serial.println("fell");
  if(debouncer.rose()) Serial.println("rose");
}
