// Author: Cislianu Radu
// Project: Myoelectric prosthesis

#include "Definitions.h"
#include <Servo.h>
//#include "Servo.h"

#define Btn_1 9
#define Btn_2 10
#define servoDelay 5
#define slowServoDelay 15

int angle = 0;
bool end;

// used in calculating the amount of time the button has been pressed
int buttonState = 0;
int lastButtonState = 0;
int pressStart;
int pressStop;
int pressTime;
int iddleTime;

// 
gripModes currentGrip;
gripModes lastGrip;

// The setup() function runs once each time the micro-controller starts
void setup()
{
    Serial.begin(9600);

    // Attaching the pins to each servo 
    //thumb.attach(servo_Thumb_Pin);
    //index.attach(servo_Index_Pin);
    //middle.attach(servo_Middle_Pin);
    //ring.attach(servo_Ring_Pin);
    //pinky.attach(servo_Pinky_Pin);

    //pinMode(Btn_1, INPUT);
    //pinMode(Btn_2, INPUT);

    //// Moving each servo to 0
    //thumb.write(pinkyMaxAngle);
    //index.write(0);
    //middle.write(0);
    //ring.write(0);
    //pinky.write(0);
}

// Add the main program code into the continuous loop() function
void loop()
{
    buttonState = digitalRead(Btn_1);

    if (buttonState != lastButtonState) {
        updateButtonState();
    }

    lastButtonState = buttonState;

    /*if (digitalRead(Btn_1)) {
        Serial.println("Buton 1");
        if (end == false) {
            for (angle = 0; angle < pinkyMaxAngle; angle++) {
                thumb.write(angle);
                delay(servoDelay);
            }
            end = true;
        }
    }

    if (digitalRead(Btn_2)) {
        Serial.println("Buton 2");
        if (end == true) {
            for (angle = pinkyMaxAngle; angle > 0; angle--) {
                thumb.write(angle);
                delay(servoDelay);
            }
            end = false;
        }
    }*/
}

void updateButtonState() {
    // the button has been pressed, hopefully 
    if (buttonState == HIGH) {
        pressStart = millis();
        iddleTime = pressStart - pressStop;
    }
    else {
        pressStop = millis();
        pressTime = pressStop - pressStart;

        if (pressTime >= 100 && pressTime < 1000) {
            Serial.println("Butonul a fost apasat o jumatate de secunda");
            changeGripMode();
        }

        if (pressTime >= 1000) {
            Serial.println("Butonul a fost apasat o secunda");
        }
    }
}

// goes to the next grip mode
void changeGripMode() {

    switch (currentGrip) {

    case gripModes::fist:
        currentGrip = gripModes::grip;
        break;
    case gripModes::grip:
        currentGrip = gripModes::pinch;
        break;
    case gripModes::pinch:
        currentGrip = gripModes::tripod;
        break;
    case gripModes::tripod:
        currentGrip = gripModes::extra1;
        break;
    case gripModes::extra1:
        currentGrip = gripModes::extra2;
        break;
    case gripModes::extra2:
        currentGrip = gripModes::fist;
        break;
    }

    printGripMode();
}

// Dev-Only functions
void printGripMode() {

    switch (currentGrip) {

    case gripModes::fist:
        Serial.println("Current pua grip mode: Fist");
        break;
    case gripModes::grip:
        Serial.println("Current pua grip mode: Grip");
        break;
    case gripModes::pinch:
        Serial.println("Current pua grip mode: Pinch");
        break;
    case gripModes::tripod:
        Serial.println("Current pua grip mode: Tripod");
        break;
    case gripModes::extra1:
        Serial.println("Current pua grip mode: Extra1");
        break;
    case gripModes::extra2:
        Serial.println("Current pua grip mode: Extra2");
        break;
    }
}