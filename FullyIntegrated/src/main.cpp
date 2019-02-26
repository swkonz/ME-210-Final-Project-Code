#include <Arduino.h>
#include "motor.h"

void setup() {
  motor_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
    // motor test code with input from computer
  if(Serial.available()) {
    int val = Serial.parseInt(); // try typing numbers from -255 to 255 into serial monitor
    Serial.print("You said: ");
    Serial.println(val);

    set_L_wheel(val);
    set_R_wheel(val);
  }
}