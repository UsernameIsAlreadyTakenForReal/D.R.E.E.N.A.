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

// 
gripModes currentGrip = gripModes::fist;
gripModes lastGrip;
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
    //thumb.attach(servo_Thumb_Pin);
    //index.attach(servo_Index_Pin);
    //middle.attach(servo_Middle_Pin);
    //ring.attach(servo_Ring_Pin);
    //pinky.attach(servo_Pinky_Pin);

    //// Moving each servo to 0
    //thumb.write(0);
    //index.write(0);
    //middle.write(0);
    //ring.write(0);
    //pinky.write(0);

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
    /////// Good code ////////////////
    
    // Due to the nature of the Arcade button, and using INPUT_PULLUP for it to work, when not pressed, digitalRead(Btn_2) will return HIGH;
     
    if (display_interruption_type == 0) {
        UpdateDisplay();    // this has to keep the display alive (since we use more than 1 letter)
    }                       // for the "interrupting" displays ( B A t -- flash -- %  || L o c 0 / L o c 1) I will have a bool "NeedsDisplayInterrupt" that, if on true, will go
                            // on to display the 2x2 possible thingies, and when the timer (i.e. millis() - interrupting_display_time_start) goes beyond 2 seconds, bool goes false

    if (display_interruption_type != 0) {
        DisplayInterrupt();
        aux_timer_2 = millis() - display_interruption_start_time;
        if (aux_timer_2 > 2000) {
            display_interruption_type = 0;
        }
    }
     
    button_state = digitalRead(Btn_2);  

    if (button_state != last_button_state) {
        UpdateButtonState();
    }

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

void UpdateDisplay() {

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

void DisplayChooseDigit(int i) //light up a 7-segment display
{
    //The 7-segment LED display is a common-cathode one. So also use digitalWrite to  set d1 as high and the LED will go out
    digitalWrite(disD1, HIGH);
    digitalWrite(disD2, HIGH);
    digitalWrite(disD3, HIGH);
    digitalWrite(disD4, HIGH);

    switch (i)
    {
    case 0:
        digitalWrite(disD1, LOW);//Light d1 up 
        break;
    case 1:
        digitalWrite(disD3, LOW); //Light d2 up 
        break;
    case 2:
        digitalWrite(disD2, LOW); //Light d3 up 
        break;
    default:
        digitalWrite(disD4, LOW); //Light d4 up 
        break;
    }
}

void DisplayChar(char let) {
    switch (let)
    {
        case 'b': {
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 'A': {
            digitalWrite(disP1, HIGH);
            digitalWrite(disP2, HIGH);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, LOW);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 'S': {
            digitalWrite(disP1, HIGH);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, LOW);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 'P': {
            digitalWrite(disP1, HIGH);
            digitalWrite(disP2, HIGH);
            digitalWrite(disP3, LOW);
            digitalWrite(disP4, LOW);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 'i': {
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, LOW);
            digitalWrite(disP4, LOW);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, LOW);
            digitalWrite(disP7, LOW);
            break;
        }
        case 'n': {
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, LOW);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, LOW);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 't': {
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, LOW);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 'r': {
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, LOW);
            digitalWrite(disP4, LOW);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, LOW);
            digitalWrite(disP7, HIGH);
            break;
        }
        case '0': {
            digitalWrite(disP1, HIGH);
            digitalWrite(disP2, HIGH);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, LOW);
            break;
        }
        case '1': {
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, HIGH);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, LOW);
            digitalWrite(disP5, LOW);
            digitalWrite(disP6, LOW);
            digitalWrite(disP7, LOW);
            break;
        }
        case 'E': {
            digitalWrite(disP1, HIGH);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, LOW);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 'X': {
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, HIGH);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, LOW);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 'L': {
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, LOW);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, LOW);
            break;
        }
        case 'o': {
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, LOW);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 'c': {
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, LOW);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, LOW);
            digitalWrite(disP7, HIGH);
            break;
        }
    }
}

void DisplayClear() {
    digitalWrite(disP1, LOW);
    digitalWrite(disP2, LOW);
    digitalWrite(disP3, LOW);
    digitalWrite(disP4, LOW);
    digitalWrite(disP5, LOW);
    digitalWrite(disP6, LOW);
    digitalWrite(disP7, LOW);
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
            Serial.println("Butonul a fost apasat 1-3 secunde");
            ++currentGroup;
            UpdateGripMode();
        }

        if (press_time >= 3000 && press_time <5000) {
            Serial.println("...Locking Servos...");
            display_interruption_start_time = millis();
            display_interruption_type = 2;
            //LockServos();
        }

        if (press_time >= 5000) {
            Serial.println("...Shut down...");
            //ShutDown();
        }

        break;

    case true:

        Serial.println("...Displaying Battery Level...");
        display_interruption_start_time = millis();
        display_interruption_type = 1;
        //DisplayBatteryLevel();

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
