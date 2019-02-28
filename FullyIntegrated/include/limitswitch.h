#include <Arduino.h>

#define FRONT_LIMIT_PIN 9
#define RIGHT_LIMIT_PIN 6
#define LEFT_LIMIT_PIN 8
#define BACK_LIMIT_PIN 6

void limit_switch_setup() {
    pinMode(FRONT_LIMIT_PIN, INPUT_PULLUP);
    pinMode(RIGHT_LIMIT_PIN, INPUT_PULLUP);
    pinMode(LEFT_LIMIT_PIN, INPUT_PULLUP);
    pinMode(BACK_LIMIT_PIN, INPUT_PULLUP);
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