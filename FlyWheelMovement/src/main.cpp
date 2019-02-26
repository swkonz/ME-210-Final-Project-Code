#include <Arduino.h>

#define PIN_FLY_ENA 5 // connect this pin to LM298N Breakout, must be PWM
#define PIN_FLY_IN1 4 // connect this pin to LM298N Breakout
#define PIN_FLY_IN2 3 // connect this pin to LM298N Breakout

/* speed = 1 to 255 to move forward
 * speed = 0 to stop
 * speed = -1 -255 to move back
 */
void set_flywheel_speed(int speed) {
  digitalWrite(PIN_FLY_IN1, speed >= 0);
  digitalWrite(PIN_FLY_IN2, speed <= 0);
  analogWrite(PIN_FLY_ENA, abs(speed));
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_FLY_IN1, OUTPUT);
  pinMode(PIN_FLY_IN2, OUTPUT);
  pinMode(PIN_FLY_ENA, OUTPUT);

  set_flywheel_speed(0);
}

// motor test code with input from computer
// try typing numbers from -255 to 255 into serial monitor to set the speed
void loop() {  
  if(Serial.available()) {
    int val = Serial.parseInt(); 
    Serial.print("You said: ");
    Serial.println(val);

    set_flywheel_speed(val);
  }
}
