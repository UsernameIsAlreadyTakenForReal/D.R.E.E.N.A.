// 
// 
// 

#include "SSegmentDisplay.h"
#include "Definitions.h"

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
    case gripModes::fist: {
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