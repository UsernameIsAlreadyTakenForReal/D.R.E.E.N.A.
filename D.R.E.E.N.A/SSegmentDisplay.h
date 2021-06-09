// SSegmentDisplay.h

#ifndef _SSegmentDisplay_h
#define _SSegmentDisplay_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif



void UpdateDisplay();
void DisplayInterrupt();
void DisplayChooseDigit(int i);
void DisplayChar(char let);
void DisplayClear();

#endif