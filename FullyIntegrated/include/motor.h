/**
 *  Motor control functions and variable definitions
 */

/* NOTE: The battery MUST be connected to LM298N Breakout for the Teensy to work since I cut
 * the connection between USB power and the Teensy's power. The LM298N Breakout has a 5V regulator,
 * which can power the Teensy if you connect Teensy's Vin ("5V") to the LM298N Breakout's +5V.
 */

#include <Arduino.h>

#define PIN_IN1 20 // connect this pin to LM298N Breakout, must be PWM
#define PIN_IN2 19 // connect this pin to LM298N Breakout
#define PIN_IN3 18 // connect this pin to LM298N Breakout
#define PIN_IN4 17 // connect this pin to LM298N Breakout, must be PWM

/*********************************************************************
 * Motor control functions
 */

/**
 * speed = 1 to 255 to move forward
 * speed = 0 to stop
 * speed = -1 -255 to move back
 */
void set_R_wheel(int speed) {
    if(speed > 255) speed = 255;
    if(speed < -255) speed = -255;
    
    if(speed > 0) {
        digitalWrite(PIN_IN2, LOW);
        analogWrite(PIN_IN1, speed);
    } else {
        digitalWrite(PIN_IN2, HIGH);
        analogWrite(PIN_IN1, 255+speed);
    }
}

/**
 * speed = 1 to 255 to move forward
 * speed = 0 to stop
 * speed = -1 -255 to move back
 */
void set_L_wheel(int speed) {
    if(speed > 255) speed = 255;
    if(speed < -255) speed = -255;
    
    if(speed > 0) {
        digitalWrite(PIN_IN3, LOW);
        analogWrite(PIN_IN4, speed);
    } else {
        digitalWrite(PIN_IN3, HIGH);
        analogWrite(PIN_IN4, 255+speed);
    }
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

  set_L_wheel(0);
  set_R_wheel(0);
}

/**
 * handle turning the robot by 90 degrees using both motors
 */
void handle_turn_ninety() {
  delay(100);
  set_L_wheel(100);
  set_R_wheel(-100);

  // wait this amount of time until at perfect 90 movement
  delay(600);
  /**
   * Alternative to this *****
   *    Could also rely on the limit switches to indicate that we've moved 90 degrees
   *    Combining a time delay and a limit switch might lead to a more precise turn
   */
}

/**
 * This function tests running both wheels, with input from computer.
 * Try typing numbers from -255 to 255 into serial monitor
 * Put this function in loop.
 */
void wheel_test() {
    if(Serial.available()) {
        int val = Serial.parseInt(); // this line is blocking code, beware.
        Serial.print("You said: ");
        Serial.println(val);
        
        set_L_wheel(val);
        set_R_wheel(val);
    }
}
