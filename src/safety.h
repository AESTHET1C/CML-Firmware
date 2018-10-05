/* Safety Module
 *
 * Handles error codes and motor/encoder watchdog and links supporting modules
 *
 * This includes error code display and motor/encoder watchdog functionality.
 *
 * The Timer2 unit runs at approximately 61Hz, and once per cycle the error
 * code display is updated and the motor checked for movement. If the motor
 * is found to not be moving, the motor is haulted and an error is flagged.
 *
 * Written by Ana Tavares <tavaresa13@gmail.com>
 */

#ifndef safety
#define safety
#include <arduino.h>
#include "safety-error.h"
#include "safety-encoder.h"
#include "power.h"

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

// The number of cycles back the watchdog compares the current encoder position to.
// For example, a value of "1" causes the current position to be compared to the position
// 1/61 seconds ago, and a value of "2" compares to the position 2/61 seconds ago.
// This allows for a small amount of speed ramp-up to prevent false watchdog triggers.
#define WATCHDOG_CYCLES 3

// Minimum encoder travel (bidirectional) seen by the watchdog to consider the motor to be moving
const byte WATCHDOG_THRESHOLD = 100;


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initWatchdog();

void enableWatchdog();

void disableWatchdog();

bool isFaulted();

void clearFaults();


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

void raiseWatchdogError();


#endif
