// Author: Cislianu Radu 
// Project: Myoelectric prosthesis || Digital Recreation of Ejected Extremities, Namely Arms (D.R.E.E.N.A.)

#include "Definitions.h"
#include <Servo.h>

#define Btn_1 9
#define Btn_2 11
#define LEDTemp 13
#define servoDelay 5
#define slowServoDelay 15
#define maxIddleTime 2000

// using SG90 servos, wiring is as follows:
// Red:    VCC    (+)
// Brown:  Ground (-)
// Orange: PWM	  (!)
// The Servo objects for each finger
Servo thumb;
Servo index;
Servo middle;
Servo ring;
Servo pinky;

int angle = 0;
bool end;

// used in calculating the amount of time and number of times the button has been pressed
int button_state = 1;
int last_button_state = 1;
int press_start;
int press_stop = 2147483647;
int press_time;
int iddle_time;
int previous_time = 0; // 1 - short press, 2 - long press
bool short_time = false;
bool button_action_needs_consumption = false;
int press_counter = 0;
int aux_timer_1;

// temp stuff
int temp_button_state = 0;
int temp_button_last_state = 0;
bool temp_button_firing = false;

// 
opMode currentMode = opMode::grips;

freeModes currentFree = freeModes::noThumb;

gripModes currentGrip = gripModes::fist;
gripGroups currentGroup = gripGroups::basic;

// used in Display Interruptions (Battery level and Locking Servos)
int display_interruption_start_time;
int display_interruption_type; // 0 - none, 1 - battery, 2 - lock
int aux_timer_2;


// The setup() function runs once each time the micro-controller starts
void setup()
{
    Serial.begin(9600);

    Serial.println("\n\n!!New session!!");

    // Attaching the pins to each servo 
    thumb.attach(servoThumbPin);
    index.attach(servoIndexPin);
    middle.attach(servoMiddlePin);
    ring.attach(servoRingPin);
    pinky.attach(servoPinkyPin);

    //// Moving each servo to 0
    thumb.write(0);
    index.write(0);
    middle.write(0);
    ring.write(0);
    pinky.write(0);

    pinMode(Btn_1, INPUT);
    pinMode(Btn_2, INPUT_PULLUP);

    pinMode(LEDTemp, OUTPUT);

    pinMode(disD1, OUTPUT);
    pinMode(disD2, OUTPUT);
    pinMode(disD3, OUTPUT);
    pinMode(disD4, OUTPUT);
    pinMode(disP1, OUTPUT);
    pinMode(disP1, OUTPUT);
    pinMode(disP2, OUTPUT);
    pinMode(disP3, OUTPUT);
    pinMode(disP4, OUTPUT);
    pinMode(disP5, OUTPUT);
    pinMode(disP6, OUTPUT);
    pinMode(disP7, OUTPUT);
}

// Loop() function, this bit of code runs forever on the board
void loop()
{
    //////////////// Test code ////////////////

    /*FlexThumb();
    FlexIndex();
    FlexMiddle();
    FlexRing();
    FlexPinky();
    delay(500);
    ExtendThumb();
    ExtendIndex();
    ExtendMiddle();
    ExtendRing();
    ExtendPinky();
    delay(1000);*/

    //////////////// Good code ////////////////
    
    // Temp
    temp_button_state = digitalRead(Btn_1);

    if (temp_button_state != temp_button_last_state) {

        if (temp_button_state == 1) {

            if (temp_button_firing == false) {

                temp_button_firing = true;
                Serial.println("On");

                if (currentMode == opMode::grips) 
                    FlexFingersWithGrip();
                if (currentMode == opMode::freeMovement)
                    FlexFingersFree();
            }

            else {

                temp_button_firing = false;
                Serial.println("Off"); 
                
                if (currentMode == opMode::grips)
                    ExtendFingersWithGrip();
                if (currentMode == opMode::freeMovement)
                    ExtendFingersFree();
            }
        }
    }

    temp_button_last_state = temp_button_state;

    // this has to keep the display alive (since we use more than 1 letter)
    // for the "interrupting" displays ( B A t -- flash -- %  || L o c 0 / L o c 1) I will have a bool "NeedsDisplayInterrupt" that, if on true, will go
    // on to display the 2x2 possible thingies, and when the timer (i.e. millis() - interrupting_display_time_start) goes beyond 2 seconds, bool goes false
    if (display_interruption_type == 0) {
     
        if (currentMode == opMode::freeMovement) {
            UpdateDisplayFree();
        }

        if (currentMode == opMode::grips) {
            UpdateDisplayGrips();                   
        }                                           
    }        
    
    // if there is any kind of display interruption, we will display it accordignly
    if (display_interruption_type != 0) {
        DisplayInterrupt();
        aux_timer_2 = millis() - display_interruption_start_time;
        if (aux_timer_2 > 2000) {
            display_interruption_type = 0;
        }
    }
     
    // Due to the nature of the Arcade button, and using INPUT_PULLUP for it to work, when not pressed, digitalRead(Btn_2) will return HIGH;
    // we take the current state
    button_state = digitalRead(Btn_2);  

    if (button_state != last_button_state) {    // if it has changed
        UpdateButtonState();                    // fire the function
    }

    // then this piece of magic
    last_button_state = button_state;

    // since we can call millis() very quickly here, we can establish now if only one button was pressed
    if (short_time == false) {
        if (button_action_needs_consumption == true) {
            aux_timer_1 = millis() - press_stop;
            if (aux_timer_1 > maxIddleTime) {

                Serial.print("millis() - press_stop: "); Serial.print(aux_timer_1); Serial.print("\n");
                TreatButtonAction();
            }
        }
    }
}

// ----------------------------- Functions implementations -----------------------------
// of functions that couldn't be implemented in Definitions.cpp


//////////////// Servo movements ////////////////
// Thumb
void FlexThumb() {

    for (angle = 0; angle < thumbMaxAngle; angle++) {
        thumb.write(angle);
        delay(servoDelay);
    }
}
void ExtendThumb() {

    for (angle = thumbMaxAngle; angle > 0; angle--) {
        thumb.write(angle);
        delay(servoDelay);
    }
}

// Index
void FlexIndex() {

    for (angle = 0; angle < indexMaxAngle; angle++) {
        index.write(angle);
        delay(servoDelay);
    }
}
void ExtendIndex() {

    for (angle = indexMaxAngle; angle > 0; angle--) {
        index.write(angle);
        delay(servoDelay);
    }
}

// Middle
void FlexMiddle() {

    for (angle = 0; angle < middleMaxAngle; angle++) {
        middle.write(angle);
        delay(servoDelay);
    }
}
void ExtendMiddle() {

    for (angle = middleMaxAngle; angle > 0; angle--) {
        middle.write(angle);
        delay(servoDelay);
    }
}

// Ring
void FlexRing() {

    for (angle = 0; angle < ringMaxAngle; angle++) {
        ring.write(angle);
        delay(servoDelay);
    }
}
void ExtendRing() {

    for (angle = ringMaxAngle; angle > 0; angle--) {
        ring.write(angle);
        delay(servoDelay);
    }
}

// Ring
void FlexPinky() {

    for (angle = 0; angle < pinkyMaxAngle; angle++) {
        pinky.write(angle);
        delay(servoDelay);
    }
}
void ExtendPinky() {

    for (angle = pinkyMaxAngle; angle > 0; angle--) {
        pinky.write(angle);
        delay(servoDelay);
    }
}

//////////////// Servo discerning ////////////////
void FlexFingersWithGrip() {
    switch (currentGrip)
    {
        case gripModes::fist: {

            for (angle = 0; angle < 180; angle++) {

                if (angle <= indexMaxAngle)
                    index.write(angle);

                if (angle <= middleMaxAngle)
                    middle.write(angle);

                if (angle <= ringMaxAngle)
                    ring.write(angle);

                if (angle <= pinkyMaxAngle)
                    pinky.write(angle);

                delay(servoDelay);
            }
            break;
        }
        case gripModes::grip: {

            for (angle = 0; angle < thumbMaxAngle; angle++) {

                thumb.write(angle);
                delay(servoDelay);
            }

            delay(500);

            for (angle = 0; angle < 180; angle++) {

                if (angle <= indexMaxAngle)
                    index.write(angle);

                if (angle <= middleMaxAngle)
                    middle.write(angle);

                if (angle <= ringMaxAngle)
                    ring.write(angle);

                if (angle <= pinkyMaxAngle)
                    pinky.write(angle);

                delay(servoDelay);
            }
            break;
        }
        case gripModes::pinch: {
            
            for (angle = 0; angle < thumbMaxAngle; angle++) {

                thumb.write(angle);
                delay(servoDelay);
            }   

            delay(500);

            for (angle = 0; angle < indexMaxAngle; angle++) {

                index.write(angle);
                delay(servoDelay);
            }
            break;
        }
        case gripModes::pinchNoFingers: {

            for (angle = 0; angle < thumbMaxAngle; angle++) {

                thumb.write(angle);
                delay(servoDelay);
            }

            delay(500);

            for (angle = 0; angle < indexMaxAngle; angle++) {

                index.write(angle);
                delay(servoDelay);
            }
            break;
        }
        case gripModes::tripod: {

            for (angle = 0; angle < thumbMaxAngle; angle++) {

                thumb.write(angle);
                delay(servoDelay);
            }

            delay(500);

            for (angle = 0; angle < 180; angle++) {

                if (angle <= indexMaxAngle)
                    index.write(angle);

                if (angle <= middleMaxAngle)
                    middle.write(angle);

                delay(servoDelay);
            }
            break;
        }
        case gripModes::tripodNoFingers: {

            for (angle = 0; angle < thumbMaxAngle; angle++) {

                thumb.write(angle);
                delay(servoDelay);
            }

            delay(500);

            for (angle = 0; angle < 180; angle++) {

                if (angle <= indexMaxAngle)
                    index.write(angle);

                if (angle <= middleMaxAngle)
                    middle.write(angle);

                delay(servoDelay);
            }
            break;
        }
        case gripModes::extra1: {
            
            for (angle = 0; angle < 180; angle++) {

                if (angle <= middleMaxAngle)
                    middle.write(angle);

                if (angle <= ringMaxAngle)
                    ring.write(angle);

                delay(servoDelay);
            }
            break;
        }
        case gripModes::extra2: {

            for (angle = 0; angle < 180; angle++) {

                if (angle <= indexMaxAngle)
                    index.write(angle);

                if (angle <= ringMaxAngle)
                    ring.write(angle);

                if (angle <= pinkyMaxAngle)
                    pinky.write(angle);

                delay(servoDelay);
            }
            break;
        }
    }
}

void ExtendFingersWithGrip() {
    switch (currentGrip)
    {
    case gripModes::fist: {

        for (angle = 180; angle > 0; angle--) {

            if (angle <= indexMaxAngle)
                index.write(angle);

            if (angle <= middleMaxAngle)
                middle.write(angle);

            if (angle <= ringMaxAngle)
                ring.write(angle);

            if (angle <= pinkyMaxAngle)
                pinky.write(angle);

            delay(servoDelay);
        }
        break;
    }
    case gripModes::grip: {

        for (angle = thumbMaxAngle; angle > 0; angle--) {

            thumb.write(angle);
            delay(servoDelay);
        }

        delay(500);

        for (angle = 180; angle > 0; angle--) {

            if (angle <= indexMaxAngle)
                index.write(angle);

            if (angle <= middleMaxAngle)
                middle.write(angle);

            if (angle <= ringMaxAngle)
                ring.write(angle);

            if (angle <= pinkyMaxAngle)
                pinky.write(angle);

            delay(servoDelay);
        }
        break;
    }
    case gripModes::pinch: {

        for (angle = thumbMaxAngle; angle > 0; angle--) {

            thumb.write(angle);
            delay(servoDelay);
        }

        delay(500);

        for (angle = indexMaxAngle; angle > 0; angle--) {

            index.write(angle);
            delay(servoDelay);
        }
        break;
    }
    case gripModes::pinchNoFingers: {

        for (angle = thumbMaxAngle; angle > 0; angle--) {

            thumb.write(angle);
            delay(servoDelay);
        }

        delay(500);

        for (angle = indexMaxAngle; angle > 0; angle--) {

            index.write(angle);
            delay(servoDelay);
        }
        break;
    }
    case gripModes::tripod: {

        for (angle = thumbMaxAngle; angle > 0; angle--) {

            thumb.write(angle);
            delay(servoDelay);
        }

        delay(500);

        for (angle = 180; angle > 0; angle--) {

            if (angle <= indexMaxAngle)
                index.write(angle);

            if (angle <= middleMaxAngle)
                middle.write(angle);

            delay(servoDelay);
        }
        break;
    }
    case gripModes::tripodNoFingers: {

        for (angle = thumbMaxAngle; angle > 0; angle--) {

            thumb.write(angle);
            delay(servoDelay);
        }

        delay(500);

        for (angle = 180; angle > 0; angle--) {

            if (angle <= indexMaxAngle)
                index.write(angle);

            if (angle <= middleMaxAngle)
                middle.write(angle);

            delay(servoDelay);
        }
        break;
    }
    case gripModes::extra1: {

        for (angle = 180; angle > 0; angle--) {

            if (angle <= middleMaxAngle)
                middle.write(angle);

            if (angle <= ringMaxAngle)
                ring.write(angle);

            delay(servoDelay);
        }
        break;
    }
    case gripModes::extra2: {

        for (angle = 180; angle > 0; angle--) {

            if (angle <= indexMaxAngle)
                index.write(angle);

            if (angle <= ringMaxAngle)
                ring.write(angle);

            if (angle <= pinkyMaxAngle)
                pinky.write(angle);

            delay(servoDelay);
        }
        break;
    }
    }
}

void FlexFingersFree() {

    Serial.println("FLEXAM CU SPOR");
}

void ExtendFingersFree() {

    Serial.println("EXTINDEM CU SPOR");
}

void DisplayInterrupt() {

    switch (display_interruption_type)
    {
        case 1: {
            DisplayChooseDigit(0);
            DisplayChar('b');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('A');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('t');
            delay(5);
            break;
        }

        case 2: {
            DisplayChooseDigit(0);
            DisplayChar('L');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('o');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('c');
            delay(5);
            break;
        }
    }
}

void UpdateDisplayFree() {

    switch (currentFree)
    {
        case freeModes::noThumb: {
            DisplayChooseDigit(0);
            DisplayChar('F');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('r');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('E');
            delay(5);
            DisplayChooseDigit(3);
            DisplayChar('0');
            delay(5);
            break;
        }
        case freeModes::withThumb: {
            DisplayChooseDigit(0);
            DisplayChar('F');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('r');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('E');
            delay(5);
            DisplayChooseDigit(3);
            DisplayChar('1');
            delay(5);
            break;
        }
    }
}

void UpdateDisplayGrips() {

    switch (currentGrip) {
        case gripModes::fist : {
            DisplayChooseDigit(0);
            DisplayChar('b');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('A');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('S');
            delay(5);
            DisplayChooseDigit(3);
            DisplayChar('0');
            delay(5);
            break;
        }
        case gripModes::grip: {
            DisplayChooseDigit(0);
            DisplayChar('b');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('A');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('S');
            delay(5);
            DisplayChooseDigit(3);
            DisplayChar('1');
            delay(5);
            break;
        }

        case gripModes::pinch: {
            DisplayChooseDigit(0);
            DisplayChar('P');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('i');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('n');
            delay(5);
            DisplayChooseDigit(3);
            DisplayChar('0');
            delay(5);
            break;
        }
        case gripModes::pinchNoFingers: {
            DisplayChooseDigit(0);
            DisplayChar('P');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('i');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('n');
            delay(5);
            DisplayChooseDigit(3);
            DisplayChar('1');
            delay(5);
            break;
        }

        case gripModes::tripod: {
            DisplayChooseDigit(0);
            DisplayChar('t');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('r');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('i');
            delay(5);
            DisplayChooseDigit(3);
            DisplayChar('0');
            delay(5);
            break;
        }
        case gripModes::tripodNoFingers: {
            DisplayChooseDigit(0);
            DisplayChar('t');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('r');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('i');
            delay(5);
            DisplayChooseDigit(3);
            DisplayChar('1');
            delay(5);
            break;
        }

        case gripModes::extra1: {
            DisplayChooseDigit(0);
            DisplayChar('E');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('X');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('t');
            delay(5);
            DisplayChooseDigit(3);
            DisplayChar('0');
            delay(5);
            break;
        }
        case gripModes::extra2: {
            DisplayChooseDigit(0);
            DisplayChar('E');
            delay(5);
            DisplayChooseDigit(1);
            DisplayChar('X');
            delay(5);
            DisplayChooseDigit(2);
            DisplayChar('t');
            delay(5);
            DisplayChooseDigit(3);
            DisplayChar('1');
            delay(5);
            break;
        }
    }
}

void UpdateButtonState() {

    if (button_state == LOW) {
        press_start = millis();
        iddle_time = press_start - press_stop;
        digitalWrite(LEDTemp, HIGH);

        if (iddle_time > 50 && iddle_time < 1800) {
            short_time = true;
            Serial.print("Cretinule\n");
        }
    }
    else {
        press_stop = millis();
        press_time = press_stop - press_start;
        press_counter++;
        button_action_needs_consumption = true;
        digitalWrite(LEDTemp, LOW);

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
            Serial.println("...Locking Servos...");
            display_interruption_start_time = millis();
            display_interruption_type = 2;
            //LockServos();
        }

        if (press_time >= 3000 && press_time <5000) {
            Serial.println("...Changing Mode...");
            currentMode++;
        }

        if (press_time >= 5000) {
            Serial.println("...Shut down...");
            //ShutDown();
        }

        break;

    case true:

        ++currentGroup;
        UpdateGripMode();
        break;
    }

    Serial.print("--- Au fost "); Serial.print(press_counter); Serial.println(" apasari --- \n");
    button_action_needs_consumption = false;
    short_time = false;
    press_counter = 0;
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

    switch (currentMode)
    {
        case opMode::freeMovement: {

            if (currentFree == freeModes::noThumb) {
                currentFree = freeModes::withThumb;
                Serial.println("WITH THUMB");
            }

            else {
                currentFree = freeModes::noThumb;
                Serial.println("WITHOUT THUMP");
            }
            break;
        }

        case grips: {

            switch (currentGroup) {

            case gripGroups::basic:
                if (currentGrip == gripModes::fist) {
                    currentGrip = gripModes::grip;
                }
                else if (currentGrip == gripModes::grip)
                    currentGrip = gripModes::fist;
                break;
            case gripGroups::pinches:
                if (currentGrip == gripModes::pinch) {
                    currentGrip = gripModes::pinchNoFingers;
                }
                else if (currentGrip == gripModes::pinchNoFingers)
                    currentGrip = gripModes::pinch;
                break;
            case gripGroups::tripods:
                if (currentGrip == gripModes::tripod) {
                    currentGrip = gripModes::tripodNoFingers;
                }
                else if (currentGrip == gripModes::tripodNoFingers)
                    currentGrip = gripModes::tripod;
                break;
            case gripGroups::extraGrips:
                if (currentGrip == gripModes::extra1) {
                    currentGrip = gripModes::extra2;
                }
                else if (currentGrip == gripModes::extra2)
                    currentGrip = gripModes::extra1;
                break;

            }
            break;
        }
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