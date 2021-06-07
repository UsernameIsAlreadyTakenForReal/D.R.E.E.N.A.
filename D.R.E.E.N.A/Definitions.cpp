#include <Servo.h>
//#include "Definitions.h"

// Defining ++ and -- for gripModes, since it will be easier to use this instead of a switch() when user sends input
//inline gripModes& operator++ (gripModes& m) { // ++
//	return m = (m == gripModes::extra2) ? gripModes::fist : static_cast<gripModes>(static_cast<int>(m) + 1);
//}
//
//inline gripModes& operator-- (gripModes& m) { // --
//	return m = (m == gripModes::fist) ? gripModes::extra2 : static_cast<gripModes>(static_cast<int>(m) - 1);
//}