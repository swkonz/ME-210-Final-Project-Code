#include <Arduino.h>

//////////////////////////////////////////////////////////////////
// Functions and objects for controlling the Flywheel launcher motor
//      - motor should always be running, just at variable speeds
//////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------
// Helper objects for controlling the flywheel launcher
//-----------------------------------------------------------------
// Flywheel pin definitions
#define PIN_FLY_ENA 5 // connect this pin to LM298N Breakout, must be PWM
#define PIN_FLY_IN1 4 // connect this pin to LM298N Breakout
#define PIN_FLY_IN2 16 // connect this pin to LM298N Breakout

//-----------------------------------------------------------------
// Function prototypes
void set_flywheel_speed(int speed);

/*-----------------------------------------------------------------
 * Setup the flywheel launcher motor
 */
void setup_flywheel() {
    // setup Flywheel pins
    pinMode(PIN_FLY_IN1, OUTPUT);
    pinMode(PIN_FLY_IN2, OUTPUT);
    pinMode(PIN_FLY_ENA, OUTPUT);

    // setup flywheel initial   
    set_flywheel_speed(0);
}

/*-----------------------------------------------------------------
 * Change the speed of the flywheel launcher motor
 *      speed = 1 to 255 to move forward
 *      speed = 0 to stop
 *      speed = -1 -255 to move back
 * 
 * TODO: change this to use a pwm output 
 *      - Need to change the output pin for the motor
 */
void set_flywheel_speed(int speed) {
    if (speed > 255) speed = 255;
    if (speed < -255) speed = -255;

    digitalWrite(PIN_FLY_IN1, true);
    digitalWrite(PIN_FLY_IN2, false);
    analogWrite(PIN_FLY_ENA, speed);
}


//////////////////////////////////////////////////////////////////
// Functions for controlling the launcher feeder
//      - motor is controlled through the pololu driver
//      - send a step to trigger motion, series of steps to rotate a certain amount
//////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------
// Helper objects for controlling the launcher feeder
//-----------------------------------------------------------------
// Feeder pin definitions
#define PIN_FEED_STEP 6 // connect to STEP input on Pololu 8825
#define PIN_FEED_DIR 7  // connect to the DIR input on the pololu 8825

// Helper CONSTANTS
const uint8_t STEPS_TO_NEXT_BALL = 70;    // Number of steps for the motor to take in order to load the next ball
const uint32_t FEEDER_FREQ = 10000;
const uint32_t FEEDER_CHECK_FREQ = 8000;   // period for checking the that the step count has reached its target steps

// helper variables
volatile uint16_t step_count = 0;         // count of the number of steps taken on this iteration

// Timers
IntervalTimer feederStepTimer;
IntervalTimer loadTimer;


/*-----------------------------------------------------------------
 * Setup the Launcher feeder motor
 */
void setup_feeder() {
      // setup feeder motor pins
  pinMode(PIN_FEED_DIR, OUTPUT);
  pinMode(PIN_FEED_STEP, OUTPUT);

  // setup feeder initial
  digitalWrite(PIN_FEED_STEP, false);
  digitalWrite(PIN_FEED_DIR, true);
}

/*-----------------------------------------------------------------
 * Trigger a rotation to feed in the next ball
 */
void load_ball() {

    // reset tracking vars
    step_count = 0;

    // start timer for motor on
    feederStepTimer.begin(toggle_output_square, FEEDER_FREQ);
    
    // start timer for checking the step count
    loadTimer.begin(control_feeder, FEEDER_CHECK_FREQ);
}

/*-----------------------------------------------------------------
 * Toggle the output of the square wave for controlling the lazy susan feeder motor
 *    This controls the output square wave being sent to the pololu DRV8825 driver for the feeder stepper motor
 */
void toggle_output_square() {
    digitalWrite(PIN_FEED_STEP, !digitalRead(PIN_FEED_STEP));
    step_count++;
}

/*-----------------------------------------------------------------
 * Control the feeder motion 
 */
void control_feeder() {
    if (step_count >= STEPS_TO_NEXT_BALL) {
        pause_feed();
        loadTimer.end();
    }
}

/*-----------------------------------------------------------------
 *  Helper function for stepper motor control - pause the stepper motion, i.e. stop sending rising edges
 */
void pause_feed() {
  // can just stop the timer from updating the output
  feederStepTimer.end();
}



//////////////////////////////////////////////////////////////////
// Functions to control the entire launcher
//      - the client file should be able to get all info needed on the launcher and control
//      - Set and determine how many balls are remaining
//      - Launch a ball
//          - If there are no balls left, return a response.
//////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------
// Helper objects for controlling the launcher 
//-----------------------------------------------------------------
// helper variables
uint8_t numBalls;

/*-----------------------------------------------------------------
 * Handle setup for all subs of launcher and the launcher
 */
void setupLauncher() {
    // setup flywheel
    setup_flywheel();

    // setup feeder
    setup_feeder();

    // set initial variables
    numBalls = 0;

}

/*-----------------------------------------------------------------
 * Handle all mechanisms for launching a ball
 */
void launch_ball() {
    // actually start the motor for the feeder
    load_ball();

}

/*-----------------------------------------------------------------
 * Set the number of balls in the feeder
 *      - meant to emulate an object for the launcher, but the variable could be accessed directly
 */
void set_num_balls(uint8_t num) {
    numBalls = num;
}



