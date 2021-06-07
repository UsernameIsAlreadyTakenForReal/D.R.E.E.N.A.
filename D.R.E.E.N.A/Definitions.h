#pragma once

//#include "References.h" // could have worked, but when moving the project out of C:/ (Arduino libraries are located in C:/), the compiler doesn't find Servo.h if it's not #included in the main .ino file
#include <Servo.h>

// using SG90 servos, wiring is as follows:
// Red:    VCC    (+)
// Brown:  Ground (-)
// Orange: PWM	  (!)
Servo thumb;
Servo index;
Servo middle;
Servo ring;
Servo pinky;

// Arduino pins for each servo PWM pin
#define servo_Thumb_Pin 3
#define servo_Index_Pin 4
#define servo_Middle_Pin 5
#define servo_Ring_Pin 6
#define servo_Pinky_Pin 7


// some servos don't need to go all the way
#define pinkyMaxAngle 180
#define ringMaxAngle 150
#define middleMaxAngle 180 
#define indexMaxAngle 180
#define thumbMaxAngle 180

// since we control each finger individually, we should define multiple ways of closing the fingers
// fist - fingers first, then thumb
// grip - fingers only
// pinch - thumb, then index
// tripod - thumb, then index + middle
// extra1 - peace among worlds
// extra2 - \m/, just because we have a servo on the pinky
enum gripModes {
	fist,
	grip,
	pinch,
	tripod,
	extra1,
	extra2
};

// Functions
void changeGripMode();
void updateButtonState();

// Dev-Only functions
void printGripMode();