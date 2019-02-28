#include <Arduino.h>
#include "motor.h"
#include "limitswitch.h"

void setup() {
  Serial.begin(9600);
  motor_setup();
  limit_switch_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
    // motor test code with input from computer
  /*if(Serial.available()) {
    int val = Serial.parseInt(); // try typing numbers from -255 to 255 into serial monitor
    Serial.print("You said: ");
    Serial.println(val);

    set_L_wheel(val);
    set_R_wheel(val);
  }*/
  limit_switch_test();
}