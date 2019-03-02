#include <Arduino.h>

////////////////////////////////////////////////////////////
// Feeder pin definitions
#define PIN_FEED_STEP 6 // connect to STEP input on Pololu 8825
#define PIN_FEED_DIR 7  // connect to the DIR input on the pololu 8825

////////////////////////////////////////////////////////////
// Flywheel pin definitions
#define PIN_FLY_ENA 5 // connect this pin to LM298N Breakout, must be PWM
#define PIN_FLY_IN1 4 // connect this pin to LM298N Breakout
#define PIN_FLY_IN2 3 // connect this pin to LM298N Breakout

////////////////////////////////////////////////////////////
// Helper CONSTANTS
const uint8_t STEPS_TO_NEXT_BALL = 70;    // Number of steps for the motor to take in order to load the next ball
const uint32_t FEEDER_FREQ = 10000;

////////////////////////////////////////////////////////////
// Helper global variables
volatile bool feedDirection = true;      // direction of motion for the motor
volatile uint16_t step_count = 0;         // count of the number of steps taken on this iteration

////////////////////////////////////////////////////////////
// Timers
IntervalTimer feederStepTimer;

/* speed = 1 to 255 to move forward
 * speed = 0 to stop
 * speed = -1 -255 to move back
 */
void set_flywheel_speed(int speed) {
  digitalWrite(PIN_FLY_IN1, speed >= 0);
  digitalWrite(PIN_FLY_IN2, speed <= 0);
  analogWrite(PIN_FLY_ENA, abs(speed));
}

////////////////////////////////////////////////////////////
// Launcher feeder motor control functions

/**
 * Toggle the output of the square wave for controlling the lazy susan feeder motor
 *    This controls the output square wave being sent to the pololu DRV8825 driver for the feeder stepper motor
 */
void toggleOutputSquare() {
    digitalWrite(PIN_FEED_STEP, !digitalRead(PIN_FEED_STEP));
    step_count++;
}

/**
 * Load the next ball into the launcher tube
 */
void load_ball() {

  Serial.println("Loading ball");
  // reset tracking vars
  step_count = 0;

  // start timer for motor on
  feederStepTimer.begin(toggleOutputSquare, FEEDER_FREQ);
}

/**
 *  Helper function for stepper motor control - pause the stepper motion, i.e. stop sending rising edges
 */
void pauseFeed() {
  // can just stop the timer from updating the output
  feederStepTimer.end();
}

////////////////////////////////////////////////////////////
// Core Teensy control
////////////////////////////////////////////////////////////
void setup() {
  // setup Serial
  Serial.begin(9600);

  // setup Flywheel pins
  pinMode(PIN_FLY_IN1, OUTPUT);
  pinMode(PIN_FLY_IN2, OUTPUT);
  pinMode(PIN_FLY_ENA, OUTPUT);

  // setup flywheel initial
  set_flywheel_speed(0);

  // setup feeder motor pins
  pinMode(PIN_FEED_DIR, OUTPUT);
  pinMode(PIN_FEED_STEP, OUTPUT);

  // setup feeder initial
  feederStepTimer.begin(toggleOutputSquare, FEEDER_FREQ);
  digitalWrite(PIN_FEED_DIR, feedDirection);

}

void loop() {  

  // test the motor for the flywheel
  // motor test code with input from computer
  // // try typing numbers from -255 to 255 into serial monitor to set the speed
  if(Serial.available()) {
    int val = Serial.parseInt(); 
    Serial.print("You said: ");
    Serial.println(val);

    set_flywheel_speed(val);
    load_ball();
  }

  // control the feeder movement
  if (step_count >= STEPS_TO_NEXT_BALL) {
    pauseFeed();
  }

}
