#include <Arduino.h>

#define FRONT_LIMIT_PIN 9
#define FRONT_LEFT_LIMIT_PIN 5
#define RIGHT_LIMIT_PIN 6
#define LEFT_LIMIT_PIN 8
#define BACK_LIMIT_PIN 7

void limit_switch_setup() {
    pinMode(FRONT_LEFT_LIMIT_PIN, INPUT_PULLUP);
    pinMode(FRONT_LIMIT_PIN, INPUT_PULLUP);
    pinMode(RIGHT_LIMIT_PIN, INPUT_PULLUP);
    pinMode(LEFT_LIMIT_PIN, INPUT_PULLUP);
    pinMode(BACK_LIMIT_PIN, INPUT_PULLUP);
}

bool front_left_limit_pressed() {
    return digitalRead(FRONT_LEFT_LIMIT_PIN) == LOW;
}

bool front_limit_pressed() {
    return digitalRead(FRONT_LIMIT_PIN) == LOW;
}

bool right_limit_pressed() {
    return digitalRead(RIGHT_LIMIT_PIN) == LOW;
}

bool left_limit_pressed() {
    return digitalRead(LEFT_LIMIT_PIN) == LOW;
}

bool back_limit_pressed() {
    return digitalRead(BACK_LIMIT_PIN) == LOW;
}

/**
 * Put this function in loop. Prints out which limit switches are pressed.
 */
void limit_switch_test() {
    if (front_left_limit_pressed()) {
        Serial.println("Front-left LS pressed");
    }
    if (front_limit_pressed()) {
        Serial.println("Front LS pressed");
    }
    if (right_limit_pressed()) {
        Serial.println("Right LS pressed");
    }
    if (left_limit_pressed()) {
        Serial.println("Left LS pressed");
    }
    if (back_limit_pressed()) {
        Serial.println("Back LS pressed");
    }
    if (back_limit_pressed()) {
        Serial.println("Back LS pressed");
    }
    delay(100);
}