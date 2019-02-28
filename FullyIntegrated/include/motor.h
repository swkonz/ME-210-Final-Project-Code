/**
 *  Motor control functions and variable definitions
 */

/* NOTE: The battery MUST be connected to LM298N Breakout for the Teensy to work since I cut
 * the connection between USB power and the Teensy's power. The LM298N Breakout has a 5V regulator,
 * which can power the Teensy if you connect Teensy's Vin ("5V") to the LM298N Breakout's +5V.
 */


#define PIN_ENA 21 // connect this pin to LM298N Breakout, must be PWM
#define PIN_IN1 20 // connect this pin to LM298N Breakout
#define PIN_IN2 19 // connect this pin to LM298N Breakout
#define PIN_IN3 18 // connect this pin to LM298N Breakout
#define PIN_IN4 17 // connect this pin to LM298N Breakout
#define PIN_ENB 16 // connect this pin to LM298N Breakout, must be PWM

/*********************************************************************
 * Motor control functions
 */

/* Set the speed of the right wheel
 * speed = 1 to 255 to move forward
 * speed = 0 to stop
 * speed = -1 -255 to move back
 */
void set_R_wheel(int speed) {
  digitalWrite(PIN_IN1, speed > 0);
  digitalWrite(PIN_IN2, speed < 0);
  analogWrite(PIN_ENA, abs(speed));
}

/* Set the speed of the left wheel
 * speed = 1 to 255 to move forward
 * speed = 0 to stop
 * speed = -1 -255 to move back
 */
void set_L_wheel(int speed) {
  digitalWrite(PIN_IN4, speed > 0);
  digitalWrite(PIN_IN3, speed < 0);
  analogWrite(PIN_ENB, abs(speed));
}

/**
 * Stop both motor motion
 */
void stop_wheels() {
    set_L_wheel(0);
    set_R_wheel(0);
}

/**
 * Setup the motor interface pins
 */

void motor_setup() {
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);

  set_L_wheel(0);
  set_R_wheel(0);
}