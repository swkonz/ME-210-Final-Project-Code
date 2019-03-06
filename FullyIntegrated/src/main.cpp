#include <Arduino.h>
#include "motor.h"
#include "limitswitch.h"
#include "launcher.h"


/* Put this function in loop.
 * This function is a proof-of-concept of using limit switches to wall bounce.
 * If this fails, we might need to implement line following.
 * The car will travel forwards bouncing along the LEFT wall, until it hits something
 * in front, at which point it will stop moving indefinitely.
 */
void simple_wall_bouncing_test() {
  if (left_limit_pressed()) {
    set_L_wheel(255);
    set_R_wheel(-200);
  } else {
    set_L_wheel(170);
    set_R_wheel(255);
  }
  if(front_limit_pressed()) {
    stop_wheels();
    while(true);
  }
}

void setup() {
  Serial.begin(9600);
  motor_setup();
  limit_switch_setup();
  setupLauncher();
  delay(2000);
}

void loop() {
  // motor test code with input from computer
  // wheel_test();

  // limit_switch_test();

  // simple_wall_bouncing_test();

  delay(1000);
  load_ball();

  // flywheel testing
  // flywheel_test();
}