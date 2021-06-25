
#include "Definitions.h"
#include <Arduino.h>

// Display from Int
void DisplayCharFromInt(int let) {

    switch (let) 
    {
        case 0: { // display 1, actually
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, HIGH);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, LOW);
            digitalWrite(disP5, LOW);
            digitalWrite(disP6, LOW);
            digitalWrite(disP7, LOW);
            break;
        }
        case 1: { // display 2, actually
            digitalWrite(disP1, HIGH);
            digitalWrite(disP2, HIGH);
            digitalWrite(disP3, LOW);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, HIGH);
            digitalWrite(disP6, LOW);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 2: { // display 3, actually
            digitalWrite(disP1, HIGH);
            digitalWrite(disP2, HIGH);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, LOW);
            digitalWrite(disP6, LOW);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 3: { // display 4, actually
            digitalWrite(disP1, LOW);
            digitalWrite(disP2, HIGH);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, LOW);
            digitalWrite(disP5, LOW);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, HIGH);
            break;
        }
        case 4: { // display 5, actually
            digitalWrite(disP1, HIGH);
            digitalWrite(disP2, LOW);
            digitalWrite(disP3, HIGH);
            digitalWrite(disP4, HIGH);
            digitalWrite(disP5, LOW);
            digitalWrite(disP6, HIGH);
            digitalWrite(disP7, HIGH);
            break;
        }
    }
}

// Display 
void DisplayChar(char let) {
    switch (let)
    {
    case 'F': {
        digitalWrite(disP1, HIGH);
        digitalWrite(disP2, LOW);
        digitalWrite(disP3, LOW);
        digitalWrite(disP4, LOW);
        digitalWrite(disP5, HIGH);
        digitalWrite(disP6, HIGH);
        digitalWrite(disP7, HIGH);
        break;
    }
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
    case '2': {
        digitalWrite(disP1, HIGH);
        digitalWrite(disP2, HIGH);
        digitalWrite(disP3, LOW);
        digitalWrite(disP4, HIGH);
        digitalWrite(disP5, HIGH);
        digitalWrite(disP6, LOW);
        digitalWrite(disP7, HIGH);
        break;
    }
    case '3': {
        digitalWrite(disP1, HIGH);
        digitalWrite(disP2, HIGH);
        digitalWrite(disP3, HIGH);
        digitalWrite(disP4, HIGH);
        digitalWrite(disP5, LOW);
        digitalWrite(disP6, LOW);
        digitalWrite(disP7, HIGH);
        break;
    }
    case '4': {
        digitalWrite(disP1, LOW);
        digitalWrite(disP2, HIGH);
        digitalWrite(disP3, HIGH);
        digitalWrite(disP4, LOW);
        digitalWrite(disP5, HIGH);
        digitalWrite(disP6, HIGH);
        digitalWrite(disP7, HIGH);
        break;
    }
    case '5': {
        digitalWrite(disP1, HIGH);
        digitalWrite(disP2, LOW);
        digitalWrite(disP3, HIGH);
        digitalWrite(disP4, HIGH);
        digitalWrite(disP5, LOW);
        digitalWrite(disP6, HIGH);
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