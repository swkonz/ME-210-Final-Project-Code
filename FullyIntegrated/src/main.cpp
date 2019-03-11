#include <Arduino.h>
#include "motor.h"
#include "limitswitch.h"
#include "launcher.h"

/**
 * State declarations
 */
typedef enum {
  START_TO_BACK,
  BACK_TO_AMMO,
  AMMO_TO_LAUNCH1,
  LAUNCH1,
  LAUNCH1_TO_AMMO,
  AMMO_TO_LAUNCH2,
  LAUNCH2_TO_LAUNCH3,
  END_STATE
} state;

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

void stop_if_time_limit_reached() {
  if(millis() > 120000) { // 2 mins, for error margin
    stop_wheels();
    set_flywheel_speed(0);
    digitalWrite(PIN_FLY_EN, HIGH); // disable stepper (idk if i wrote this right)
    while(true);
  }
}

void my_delay(unsigned long time) {
  unsigned long end_time = millis() + time;
  while(millis() < end_time) {
    stop_if_time_limit_reached();
  }
}

///////////////////////////////////////////////////////
// Main control function prototypes
///////////////////////////////////////////////////////
void handle_move_to_back();
void handle_move_to_ammmo();
void handle_move_to_launch1();
void handle_launch1();
void handle_lauch1_to_ammo();
void handle_ammo_to_launch2();
void auto_shutdown();
void handle_launch2_to_launch3();

///////////////////////////////////////////////////////
// Global Variables
///////////////////////////////////////////////////////
state current_state;

void setup() {
  Serial.begin(9600);
  motor_setup();
  limit_switch_setup();
  setupLauncher();
  delay(2000);

  ///////////////////////////////////////////////////////
  // Setup state machine
  ///////////////////////////////////////////////////////
  current_state = START_TO_BACK;
}

void loop() {

  // set_flywheel_speed(255);
  // delay(10000);
  // for(int i = 0; i < 6; i++) {
  //   load_ball();
  //   delay(400);
  // }

  
  if (current_state == START_TO_BACK) {
    // control to start to back function
    handle_move_to_back();
  } else if (current_state == BACK_TO_AMMO) {
    // control to back to ammo function
    handle_move_to_ammmo();
  } else if (current_state == AMMO_TO_LAUNCH1) {
    // controll to launch1 function
    handle_move_to_launch1();
  } else if (current_state == LAUNCH1) {
    // control to launch1 function
    handle_launch1();
  } else if (current_state == LAUNCH1_TO_AMMO) {
    handle_lauch1_to_ammo();
  } else if (current_state == AMMO_TO_LAUNCH2) {
    handle_ammo_to_launch2();
  } else if(current_state == END_STATE) {
    stop_wheels();
    set_flywheel_speed(0);
  } else if(current_state == LAUNCH2_TO_LAUNCH3) {
    handle_launch2_to_launch3();
  } else {
    stop_wheels();
  } 
  
}

///////////////////////////////////////////////////////
// state helpers
///////////////////////////////////////////////////////

/**
 * Move from the start location to the back wall
 *    - Should be starting from the start location with front pointed at the back wall
 *    - robot will end with it's front pointed into the wall, fron LS depressed
 *        - Left side will be immediately adjacent to the wall
 */
void handle_move_to_back() {
  while(!front_limit_pressed()) {
    stop_if_time_limit_reached();
    set_L_wheel(255);
    set_R_wheel(255);
  }
  delay(100);
  current_state = BACK_TO_AMMO;
}

/**
 * Move from back corner to the opposite side of the arena (where the ammo button is)
 *    - Robot should start with front and left sides adjacent to the arena wall
 *    - motor will end with it's front directly in the ammo button and left side near the arena wall
 */
void handle_move_to_ammmo() {
  // turn 90
  handle_turn_right_ninety();

  // go straight until hitting the ammo button
  while(!front_limit_pressed()) {
    stop_if_time_limit_reached();
    if(left_limit_pressed()) {
      set_L_wheel(255);
      set_R_wheel(255);
    } else {
      set_R_wheel(255);
      set_L_wheel(245);             // left spins slightly slower in order to turn a bit to the 
    }
  }
  stop_wheels();

  // delay to load ammo
  delay(5000);

  current_state = AMMO_TO_LAUNCH1;
}

/**
 * Move from the ammo button to the launch1 location
 *    - Should start with front facing the wall and left side near the back wall of the arena
 *    - should end in the top corner of the arena with front and left immediately adjacent to wall
 */
void handle_move_to_launch1() {
  // turn 90
  handle_turn_right_ninety();

  while(!front_limit_pressed()) {
    stop_if_time_limit_reached();
    if (left_limit_pressed()) {
      set_L_wheel(255);
      set_R_wheel(255);
    } else {
      set_R_wheel(200);
      set_L_wheel(185);  
    }
  }
  
  stop_wheels();
  current_state = LAUNCH1;  
}

/**
 * Handle the launching of a ball at launch1 location 1
 *    - Starts with front and left immediately adjacent to wall
 *    - First will turn 90 degrees
 *    - After turn of 90, will launch 1 ball
 */
void handle_launch1() {
  // turn 90
  handle_turn_right_ninety();

  // turn back slightly
  handle_turn_left_X(280);

  // turn on flywheel
  set_flywheel_speed(255);
  delay(10000);

  // launcher should be pointed at the target - SEND IT
  for(int i=0; i < 6; i++) {
    load_ball();
    delay(300);

    handle_turn_right_X(80);
    delay(150);
  }

  // turn off flywheel
  set_flywheel_speed(0);
  delay(1000);

  current_state = LAUNCH1_TO_AMMO;
}

/**
 * Handle movement from launch 1 back to the ammo button
 *    - Starts with left and back sides adjacent to the walls at launch1 location
 */
void handle_lauch1_to_ammo() {

  Serial.println("start move from launch1 - > ammo");

  // more forward a bit to get away from the wall
  set_L_wheel(255);
  set_R_wheel(255);
  delay(800);
  stop_wheels();

  delay(200);

  // turn 90 to right to get back to ammo
  handle_turn_right_ninety();
  delay(100);

  // turn a bit back to align
  // handle_turn_left_X(200);
  handle_turn_right_X(80);

  // drive back to the ammo button - offset to point towards the wall
  while(!front_limit_pressed()) {
    stop_if_time_limit_reached();
    set_R_wheel(255);
    set_L_wheel(255); 
  }
  stop_wheels();
  delay(500);

  // turn right to the ammo button
  handle_turn_right_ninety();

  handle_turn_right_X(270);

  // drive into the ammo button
  while(!front_limit_pressed()) {
    stop_if_time_limit_reached();
    set_R_wheel(200);
    set_L_wheel(180); 
  }

  delay(300);

  stop_wheels();

  // wait for loading
  delay(5000);
  Serial.println("done waiting for load");

  current_state = AMMO_TO_LAUNCH2;
}

/**
 * Handle movement from ammo button to launch 2 location
 *    - state with the front and the left side facing the walls in the ammo corner
 */
void handle_ammo_to_launch2() {

  Serial.println("entering launch ammo->2");

  // turn 90
  handle_turn_right_ninety();

  // handle_drive_straight();
  while(!front_limit_pressed()) {
    stop_if_time_limit_reached();
    if (left_limit_pressed()) {
      set_L_wheel(255);
      set_R_wheel(255);
    } else {
      set_R_wheel(200);
      set_L_wheel(185);             // left spins slightly slower in order to turn a bit to the 
    }
  }
  
  stop_wheels();

  // turn right towards the center
  handle_turn_right_ninety();

  // drive to the center
  set_L_wheel(170);
  set_R_wheel(200);
  delay(3880); //3780

  // stop at center
  stop_wheels();

  // turn on flywheel
  set_flywheel_speed(255);
  delay(10000);

  // launcher should be pointed at the target - SEND IT
  for(int i=0; i < 3; i++) {
    load_ball();
    delay(400);

    // handle_turn_right_X(40);
    // delay(150);
    set_flywheel_speed(180);
    if(i == 0) {
      delay(2000);
    }
  }

  // turn off flywheel
  set_flywheel_speed(0);
  delay(100);

  Serial.println("entering launch2->3");


  current_state = LAUNCH2_TO_LAUNCH3;

}

/**
 * 
 */
void handle_launch2_to_launch3() {
  Serial.println("entering launch2->3");

  // drive to next launch
  set_L_wheel(180);
  set_R_wheel(200);
  delay(4200); // 3700

  stop_wheels();

  // turn on flywheel
  set_flywheel_speed(255);
  delay(10000);

  // launcher should be pointed at the target - SEND IT
  for(int i=0; i < 3; i++) {
    load_ball();
    delay(400);
    Serial.println("launch");

    // handle_turn_right_X(40);
    // delay(150);
    // set_flywheel_speed(180);
    // if(i == 0) {
    //   delay(2000);
    // }
  }

  // turn off flywheel
  set_flywheel_speed(0);
  delay(1000);  
  Serial.println("done launching stuff at launch3");

  current_state = END_STATE;

}
