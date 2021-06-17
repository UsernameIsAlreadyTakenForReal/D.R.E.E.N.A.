// Author: Cislianu Radu 
// Project: Myoelectric prosthesis || Digital Recreation of Ejected Extremities, Namely Arms (D.R.E.E.N.A.)

#include "Definitions.h"
#include <Servo.h>
#include <avr/sleep.h>

#define Btn_1 9
#define Btn_2 21

#define LEDTemp 12
#define servoDelay 5
#define slowServoDelay 15
#define maxIddleTime 2000

#define sensorThreshold 2.5
#define sensorPin A11 
#define sensorDelay 5   // so we do not overload the pin and the sensor. The point to make here is that we want to read the sensor at all times
                        // so that means inside the loop() function, yet we should delay it by this amount so we don't overload. At the time of
                        // writing this, no test has yet been undergone and I cannot say for sure if it will interfere with the button and dis-
                        // play functions. Let's hope not

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
bool are_servers_locked = false;

int angle = 0;
bool end;

// used in calculating the amount of time and number of times the button has been pressed
int button_state = 1;
int last_button_state = 1;
int press_start = 2147483647;
int press_stop = 2147483647; // MAX_INT to avoid having the program think there has been a very long button press when the button is pressed by the user for the first time. 
int press_time;
int iddle_time;
int previous_time = 0; // 1 - short press, 2 - long press
bool short_time = false;
bool button_action_needs_consumption = false;
int press_counter = 0;
int aux_timer_1;

// EXTRA stuff
int sensor_state = 0;
int sensor_last_state = 0;
bool system_firing = false;
int led_time = 0;

// 
opMode currentMode = opMode::freeMovement;
freeModes currentFree = freeModes::noThumb; 
gripModes currentGrip = gripModes::fist;
gripGroups currentGroup = gripGroups::basic;

// used in Display Interruptions (Battery level and Locking Servos)
int display_interruption_start_time;
int display_interruption_type; // 0 - none, 1 - battery, 2 - lock
int aux_timer_2;

// At last, the sensor itself
int sensor_value;           // for analogRead()
float voltage;              // to get an actual value that makes sense

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

    // Moving each servo to its 0 position
    thumb.write(thumbMinAngle);
    index.write(indexMinAngle);
    middle.write(middleMinAngle);
    ring.write(ringMinAngle);
    pinky.write(pinkyMinAngle);

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

    // Testing servos when reseting
    /*FlexThumb();
    delay(500);
    ExtendThumb();
    delay(500);

    FlexIndex();
    delay(500);
    ExtendIndex();
    delay(500);

    FlexMiddle();
    delay(500);
    ExtendMiddle();
    delay(500);

    FlexRing();
    delay(500);
    ExtendRing();
    delay(500);

    FlexPinky();
    delay(500);
    ExtendPinky();
    delay(1000);*/

    digitalWrite(LEDTemp, LOW);
}

// Loop() function, this bit of code runs forever on the board
void loop()
{
    //////////////// Test code ////////////////
    //digitalWrite(LEDTemp, LOW);

    ////////////////// Good code ////////////////
    
    //// Temp -- will be replaced with the sensor variant
    sensor_state = digitalRead(Btn_1);

    /*sensor_value = analogRead(sensorPin);
    voltage = sensor_value * (5.0 / 1023.0);

    Serial.println(voltage);

    if (voltage > sensorThreshold) {
        sensor_state = 1;
    }

    else {
        sensor_state = 0;
    }*/

    if (sensor_state != sensor_last_state) {

        if (sensor_state == 1) {

            if (system_firing == false) {

                system_firing = true;
                Serial.println("On");

                if (are_servers_locked == false) {
                    if (currentMode == opMode::grips)
                        FlexFingersWithGrip();
                }
            }

            else {

                system_firing = false;
                Serial.println("Off"); 
                
                if (are_servers_locked == false) {
                    if (currentMode == opMode::grips)
                        ExtendFingersWithGrip();
                }
            }
        }
    }

    sensor_last_state = sensor_state;

    // Free mode rotations
    if (are_servers_locked == false) {
        if (currentMode == opMode::freeMovement) {

            if (system_firing == true && sensor_state == 1) { // if the button is pressed and "ON" || if the sensor senses signal and „ON”

                if (angle < 180) {
                    angle++;


                    if (angle < indexMaxAngle) {
                        index.write(angle);
                    }

                    if (middleMinAngle - angle >= middleMaxAngle) {
                        middle.write(middleMinAngle - angle);
                    }

                    if (angle < ringMaxAngle) {
                        ring.write(angle);
                    }

                    if (pinkyMinAngle - angle >= pinkyMaxAngle) {
                        pinky.write(pinkyMinAngle - angle);
                    }

                    if (currentFree == freeModes::withThumb) {

                        if (angle < thumbMaxAngle) {
                            thumb.write(angle);
                        }
                    }
                    delay(slowServoDelay);
                }

            }

            if (system_firing == false && sensor_state == 1) { // if the button is pressed and "OFF"

                if (angle > 0) {
                    angle--;

                    if (angle > indexMinAngle) {
                        index.write(angle);
                    }

                    if (middleMinAngle - angle >= middleMaxAngle) {
                        middle.write(middleMinAngle - angle);
                    }

                    if (angle > ringMinAngle) {
                        ring.write(angle);
                    }

                    if (pinkyMinAngle - angle >= pinkyMaxAngle) {
                        pinky.write(pinkyMinAngle - angle);
                    }

                    if (currentFree == freeModes::withThumb) {
                        if (angle > thumbMinAngle) {
                            thumb.write(angle);
                        }
                    }
                    delay(slowServoDelay);
                }
            }
        }
    }

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

    if (button_state = 1) {
        led_time = millis() - press_start;

        if (led_time > 980 && led_time < 1020) {
            digitalWrite(LEDTemp, LOW);
            delay(10);
            digitalWrite(LEDTemp, HIGH);
        }

        if (led_time > 1980 && led_time < 2020) {
            digitalWrite(LEDTemp, LOW);
            delay(10);
            digitalWrite(LEDTemp, HIGH);
        }

        if (led_time > 2980 && led_time < 3020) {
            digitalWrite(LEDTemp, LOW);
            delay(10);
            digitalWrite(LEDTemp, HIGH);
        }

        if (led_time > 3980 && led_time < 4020) {
            digitalWrite(LEDTemp, LOW);
            delay(10);
            digitalWrite(LEDTemp, HIGH);
        }

        if (led_time > 4980 && led_time < 5020) {
            digitalWrite(LEDTemp, LOW);
            delay(10);
            digitalWrite(LEDTemp, HIGH);
        }
    }

    delay(sensorDelay);
}

// ----------------------------- SLEEP MODE MANAGER -----------------------------
void SleepMode() {

    DisplayClear();
    sleep_enable();
    attachInterrupt(2, WakeUp, LOW);
    Serial.println("1");
    set_sleep_mode(SLEEP_MODE_STANDBY);
    //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    delay(1000);
    sleep_cpu();
}

void WakeUp() {
    Serial.println("muie a todos");
    sleep_disable();
    detachInterrupt(Btn_2);
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

    for (angle = 0; angle < 180; angle++) {
        middle.write(middleMinAngle - angle);
        delay(servoDelay);
    }
}
void ExtendMiddle() {

    for (angle = 180; angle > 0; angle--) {
        middle.write(middleMinAngle - angle);
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

// Pinky
void FlexPinky() {

    for (angle = 0; angle < 180; angle++) {
        pinky.write(pinkyMinAngle - angle);
        delay(servoDelay);
    }
}
void ExtendPinky() {

    for (angle = 180; angle > 0; angle--) {
        pinky.write(pinkyMinAngle - angle);
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

                if (middleMinAngle - angle >= middleMaxAngle)
                    middle.write(middleMinAngle - angle);

                if (angle <= ringMaxAngle)
                    ring.write(angle);

                if (pinkyMinAngle - angle >= pinkyMaxAngle)
                    pinky.write(pinkyMinAngle - angle);

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

                if (middleMinAngle - angle >= middleMaxAngle)
                    middle.write(middleMinAngle - angle);

                if (angle <= ringMaxAngle)
                    ring.write(angle);

                if (pinkyMinAngle - angle >= pinkyMaxAngle)
                    pinky.write(pinkyMinAngle - angle);

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

                if (middleMinAngle - angle >= middleMaxAngle)
                    middle.write(middleMinAngle - angle);

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

                if (middleMinAngle - angle >= middleMaxAngle)
                    middle.write(middleMinAngle - angle);

                delay(servoDelay);
            }
            break;
        }
        case gripModes::extra1: {
            
            for (angle = 0; angle < 180; angle++) {

                if (middleMinAngle - angle >= middleMaxAngle)
                    middle.write(middleMinAngle - angle);

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

                if (pinkyMinAngle - angle >= pinkyMaxAngle)
                    pinky.write(pinkyMinAngle - angle);

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

            if (middleMinAngle - angle >= middleMaxAngle)
                middle.write(middleMinAngle - angle);

            if (angle <= ringMaxAngle)
                ring.write(angle);

            if (pinkyMinAngle - angle >= pinkyMaxAngle)
                pinky.write(pinkyMinAngle - angle);

            delay(servoDelay);
        }
        break;
    }
    case gripModes::grip: {

        for (angle = 180; angle > 0; angle--) {

            if (angle <= indexMaxAngle)
                index.write(angle);

            if (middleMinAngle - angle >= middleMaxAngle)
                middle.write(middleMinAngle - angle);

            if (angle <= ringMaxAngle)
                ring.write(angle);

            if (pinkyMinAngle - angle >= pinkyMaxAngle)
                pinky.write(pinkyMinAngle - angle);

            delay(servoDelay);
        }

        delay(500);

        for (angle = thumbMaxAngle; angle > 0; angle--) {

            thumb.write(angle);
            delay(servoDelay);
        }
        break;
    }
    case gripModes::pinch: {

        for (angle = indexMaxAngle; angle > 0; angle--) {

            index.write(angle);
            delay(servoDelay);
        }

        delay(500);

        for (angle = thumbMaxAngle; angle > 0; angle--) {

            thumb.write(angle);
            delay(servoDelay);
        }
        break;
    }
    case gripModes::pinchNoFingers: {

        for (angle = indexMaxAngle; angle > 0; angle--) {

            index.write(angle);
            delay(servoDelay);
        }

        delay(500);

        for (angle = thumbMaxAngle; angle > 0; angle--) {

            thumb.write(angle);
            delay(servoDelay);
        }
        break;
    }
    case gripModes::tripod: {

        for (angle = 180; angle > 0; angle--) {

            if (angle <= indexMaxAngle)
                index.write(angle);

            if (middleMinAngle - angle >= middleMaxAngle)
                middle.write(middleMinAngle - angle);

            delay(servoDelay);
        }

        delay(500);

        for (angle = thumbMaxAngle; angle > 0; angle--) {

            thumb.write(angle);
            delay(servoDelay);
        }
        break;
    }
    case gripModes::tripodNoFingers: {

        for (angle = 180; angle > 0; angle--) {

            if (angle <= indexMaxAngle)
                index.write(angle);

            if (middleMinAngle - angle >= middleMaxAngle)
                middle.write(middleMinAngle - angle);

            delay(servoDelay);
        }

        delay(500);

        for (angle = thumbMaxAngle; angle > 0; angle--) {

            thumb.write(angle);
            delay(servoDelay);
        }
        break;
    }
    case gripModes::extra1: {

        for (angle = 180; angle > 0; angle--) {

            if (middleMinAngle - angle >= middleMaxAngle)
                middle.write(middleMinAngle - angle);

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

            if (pinkyMinAngle - angle >= pinkyMaxAngle)
                pinky.write(pinkyMinAngle - angle);

            delay(servoDelay);
        }
        break;
    }
    }
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
        Serial.println("aici??");
            
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
            if (are_servers_locked == false) {
                Serial.println("...Changing Grip...");
                ChangeGripMode();
            }
        }

        if (press_time >= 1000 && press_time < 3000) {
            Serial.println("...Locking Servos...");
            display_interruption_start_time = millis();
            display_interruption_type = 2;

            if (are_servers_locked == false) {
                are_servers_locked = true;
            }
            else {
                are_servers_locked = false;
            }
        }

        if (press_time >= 3000 && press_time <5000) {
            if (are_servers_locked == false) {
                Serial.println("...Changing Mode...");
                currentMode++;
            }
        }

        if (press_time >= 5000) {
            if (are_servers_locked == false) {
                Serial.println("...Shut down...");
                //SleepMode();
            }
        }

        break;

    case true:

        if (are_servers_locked == false) {

            if (currentMode == opMode::grips) {
                ++currentGroup;
                UpdateGripMode();
            }
        }
        break;
    }

    button_action_needs_consumption = false;
    short_time = false;
    press_counter = 0;
    previous_time = 0;
    iddle_time = 0;    
    press_start = 2147483647;
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
                    FlexPinky();
                    FlexRing();
                    FlexMiddle();
                }
                else if (currentGrip == gripModes::pinchNoFingers) {
                    currentGrip = gripModes::pinch;
                    ExtendPinky();
                    ExtendRing();
                    ExtendMiddle();
                }
                break;
            case gripGroups::tripods:
                if (currentGrip == gripModes::tripod) {
                    currentGrip = gripModes::tripodNoFingers;
                    FlexPinky();
                    FlexRing();
                }
                else if (currentGrip == gripModes::tripodNoFingers) {
                    currentGrip = gripModes::tripod;
                    ExtendPinky();
                    ExtendRing();
                }
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
