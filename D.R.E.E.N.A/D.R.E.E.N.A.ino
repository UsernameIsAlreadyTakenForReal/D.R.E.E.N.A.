// Author: Cislianu Radu 
// Project: Myoelectric prosthesis || Digital Recreation of Ejected Extremities, Namely Arms (D.R.E.E.N.A.)

#include "Definitions.h"
#include <Servo.h>

#define Btn_1 9
#define Btn_2 10
#define servoDelay 5
#define slowServoDelay 15

int angle = 0;
bool end;

// used in calculating the amount of time and number of times the button has been pressed
int button_state = 0;
int last_button_state = 0;
int press_start;
int press_stop;
int press_time;
int iddle_time;
int previous_time = 0; // 1 - short press, 2 - long press
bool short_time = false;
bool button_action_needs_consumption = false;

// 
gripModes currentGrip = gripModes::fist;
gripModes lastGrip;
gripGroups currentGroup = gripGroups::basic;



// The setup() function runs once each time the micro-controller starts
void setup()
{
    Serial.begin(9600);

    Serial.println("\n\n!!New session!!");

    // Attaching the pins to each servo 
    thumb.attach(servo_Thumb_Pin);
    //index.attach(servo_Index_Pin);
    //middle.attach(servo_Middle_Pin);
    //ring.attach(servo_Ring_Pin);
    //pinky.attach(servo_Pinky_Pin);

    //pinMode(Btn_1, INPUT);
    //pinMode(Btn_2, INPUT);

    //// Moving each servo to 0
    thumb.write(0);
    //index.write(0);
    //middle.write(0);
    //ring.write(0);
    //pinky.write(0);
}

// Loop() function, this bit of code runs forever on the board
void loop()
{
    button_state = digitalRead(Btn_1);

    if (button_state != last_button_state) {
        UpdateButtonState();
    }

    last_button_state = button_state;

    // since we can call millis() very quickly here, we can establish now if only one button was pressed
    if ((short_time == false && button_action_needs_consumption == true) && (millis() - press_stop > 1500)) {
        TreatButtonAction();
    }
}

void UpdateButtonState() {

    if (button_state == HIGH) {
        press_start = millis();
        iddle_time = press_start - press_stop;

        if (iddle_time > 100 && iddle_time < 1500) { // 
            short_time = true;
            
            if (press_time >= 100 && press_time < 1000)
                previous_time = 1; // previous button was a short press
            if (press_time >= 1000 && press_time < 3000)
                previous_time = 2; // previous button was a long press
        }
    }
    else {
        press_stop = millis();
        button_action_needs_consumption = true;
        press_time = press_stop - press_start;

        if (short_time == true) {
            TreatButtonAction();
        }
    }
}

void TreatButtonAction() {

    switch (short_time)
    {
    case false: // a single press of the button

        if (press_time >= 100 && press_time < 1000) { // using [100, 1000] interval, instead of [0, 1000], to avoid any undesired impulse to be seen as an input here
            Serial.println("Butonul a fost apasat o jumatate de secunda");
            ChangeGripMode();
        }

        if (press_time >= 1000 && press_time < 3000) {
            Serial.println("Butonul a fost apasat 1-3 secunde");
            ++currentGroup;
            UpdateGripMode();
        }

        if (press_time >= 3000) {
            Serial.println("Shut down");
        }

        break;

    case true:

        if (previous_time == 2 && (press_time >= 1000 && press_time < 3000)) { // two long presses
            //LockServos();
            Serial.println("...Locking Servos...");
        }

        if (previous_time == 1 && (press_time >= 100 && press_time < 1000)) { // two short presses
            Serial.println("...Displaying Battery Level...");
            //DisplayBatteryLevel();
        }
        break;
    }

    button_action_needs_consumption = false;
    short_time = false;
    previous_time = 0;
    iddle_time = 0;    
}

// goes to the first grip mode in a group when the group is changed
void UpdateGripMode() {
    
    switch (currentGroup)
    {
    case basic:
        currentGrip = gripModes::fist;
        break;
    case pinches:
        currentGrip = gripModes::pinch;
        break;
    case tripods:
        currentGrip = gripModes::tripod;
        break;
    case extraGrips:
        currentGrip = gripModes::extra1;
        break;
    }

    PrintGripMode();
}

// goes to the next grip mode
void ChangeGripMode() {

    switch (currentGroup) {

    case gripGroups::basic :
        if (currentGrip == gripModes::fist) {
            currentGrip = gripModes::grip;
        }
        else if (currentGrip == gripModes::grip)
            currentGrip = gripModes::fist;
        break;
    case gripGroups::pinches :
        if (currentGrip == gripModes::pinch) {
            currentGrip = gripModes::pinchNoFingers;
        }
        else if (currentGrip == gripModes::pinchNoFingers)
            currentGrip = gripModes::pinch;
        break;
    case gripGroups::tripods :
        if (currentGrip == gripModes::tripod) {
            currentGrip = gripModes::tripodNoFingers;
        }
        else if (currentGrip == gripModes::tripodNoFingers)
            currentGrip = gripModes::tripod;
        break;
    case gripGroups::extraGrips :
        if (currentGrip == gripModes::extra1) {
            currentGrip = gripModes::extra2;
        }
        else if (currentGrip == gripModes::extra2)
            currentGrip = gripModes::extra1;
        break;

    }

    PrintGripMode();
}

// Dev-Only functions
void PrintGripMode() {

    switch (currentGroup)
    {
    case gripGroups::basic:
        switch (currentGrip)
        {
        case gripModes::fist:
            Serial.println("Current: Basic - Fist");
            break;
        case gripModes::grip:
            Serial.println("Current: Basic - Grip");
            break;
        }
        break;

    case gripGroups::pinches:
        switch (currentGrip)
        {
        case gripModes::pinch:
            Serial.println("Current: Pinches - Pinch");
            break;
        case gripModes::pinchNoFingers:
            Serial.println("Current: Pinches - PinchNoFist");
            break;
        }
        break;

    case gripGroups::tripods:
        switch (currentGrip)
        {
        case gripModes::tripod:
            Serial.println("Current: Tripods - Tripod");
            break;
        case gripModes::tripodNoFingers:
            Serial.println("Current: Tripods - TripodhNoFist");
            break;
        }
        break;

    case gripGroups::extraGrips:
        switch (currentGrip)
        {
        case gripModes::extra1:
            Serial.println("Current: ExtraGrips - Extra1");
            break;
        case gripModes::extra2:
            Serial.println("Current: ExtraGrips - Extra2");
            break;
        }
        break;
    }
}
