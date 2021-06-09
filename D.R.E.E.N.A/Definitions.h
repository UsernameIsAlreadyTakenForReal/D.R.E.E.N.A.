#ifndef _Definitions_h
#define _Definitions_h


//#include "References.h"	// this could have worked, but when moving the project out of C:/ (Arduino libraries are located in C:/), 
							// the compiler doesn't find Servo.h if it's not #included in the main .ino file

#include <Servo.h>

// Arduino pins for each servo PWM pin. This is the only Servo pin directly connected to the Arduino. Servos should be powered separately (i.e. using a 9V battery)
#define servoThumbPin 3
#define servoIndexPin 4
#define servoMiddlePin 5
#define servoRingPin 6
#define servoPinkyPin 7

// some servos don't need to go all the way and should be ajusted through testing
#define pinkyMaxAngle 180
#define ringMaxAngle 150
#define middleMaxAngle 180 
#define indexMaxAngle 180
#define thumbMaxAngle 180

// Pins for the 4 Digit 7-Segment Display. D1-D4 control which Digit is changed (to select a certain digit, put its D pin on LOW!)
//		  P1
//		 ____ 
//	 P6	|    | P2
//		|_P7_| 
//	 P5 |    | P3
//		|____|
//		  P4
#define disP1 22
#define disP2 23
#define disP3 24
#define disP4 25
#define disP5 26
#define disP6 27
#define disP7 28
#define disD1 30
#define disD2 31
#define disD3 32
#define disD4 33


///////////// Groups Definitions /////////////

enum opMode {
	grips,
	freeMovement
};

enum freeModes {
	withThumb,
	noThumb
};

// since we control each finger individually, we should define multiple ways of closing the fingers
// fist - fingers first, then thumb
// grip - fingers only
// pinch - thumb, then index |
// tripod - thumb, then index + middle | 
// extra1 - peace among worlds
// extra2 - \m/, just because we have a servo on the pinky
enum gripModes {
	fist,
	grip,
	pinch,
	pinchNoFingers,
	tripod,
	tripodNoFingers,
	extra1,
	extra2
};

enum gripGroups {
	basic,
	pinches,
	tripods,
	extraGrips
};

// Defining ++ and -- operations for gripGroups, since it will be easier to use this instead of a switch() when user sends input 
// Also implementing here because I couldn't get it to work inside Definitions.cpp for whatever reason
inline gripGroups& operator++ (gripGroups& m) { // ++
	return m = (m == gripGroups::extraGrips) ? gripGroups::basic : static_cast<gripGroups>(static_cast<int>(m) + 1);
}
inline gripGroups& operator-- (gripGroups& m) { // --
	return m = (m == gripGroups::basic) ? gripGroups::extraGrips : static_cast<gripGroups>(static_cast<int>(m) - 1);
}
inline opMode& operator++ (opMode& m) {
	return m = (m == opMode::freeMovement) ? opMode::grips : static_cast<opMode>(static_cast<int>(m) + 1);
}



//////////// Function Definitions ////////////

// Button actions
void TreatButtonAction();
void UpdateButtonState();
void ChangeGripMode();
void UpdateGripMode();

// Servo Actions
void LockServos();
void FlexFingersWithGrip();
void ExtendFingersWithGrip();
void FlexFingersFree();

void FlexThumb();
void FlexIndex();
void FlexMiddle();
void FlexRing();
void FlexPinky();
void ExtendThumb();
void ExtendIndex();
void ExtendMiddle();
void ExtendRing();
void ExtendPinky();

// Display Actions
void DisplayBatteryLevel();
void DisplayChar(char let);
void UpdateDisplayGrips();
void UpdateDisplayFree();
void DisplayClear();
void DisplayChooseDigit(int i);
void DisplayInterrupt();



//////////// Dev-Only functions ////////////
void PrintGripMode();



#endif // !_Definitions_h