/* Safety Module
 *
 * Handles error codes and motor/encoder watchdog and links supporting modules
 *
 * This includes error code display and motor/encoder watchdog functionality.
 *
 * The Timer2 unit is used to regularly update the error code display and check encoder movement.
 * It runs at approximately 61 Hz. If the motor is found to not be moving a sufficient amount
 * between interrupts, the motor is haulted and an error is flagged.
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
/*
 * Configures and initializes motor watchdog and error codes
 * Should be called before using motor functions
 */

void enableWatchdog();
/*
 * Enables the motor watchdog
 * Should be called by the Power Module immediately before enabling motor movement
 *
 * Affects Watchdog_Enabled, Watchdog_Queue[]
 */

void disableWatchdog();
/*
 * Disables the motor watchdog
 * Should be called by the Power Module immediately before disabling motor movement
 *
 * Affects Watchdog_Enabled
 */

bool isFaulted();
/*
 * Returns true if motor is faulted
 *
 * OUTPUT: State of motor fault
 */

void clearFaults();
/*
 * Removes motor fault flag
 *
 * Affects Is_Faulted
 */


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

void raiseWatchdogError();
/*
 * Flags the motor as faulted and takes appropriate actions
 * Used by the Timer2 overflow interrupt
 *
 * This includes halting the motor and flagging the appropriate error code for display.
 *
 * Affects Is_Faulted, Motor_Movement, Last_Motor_Disable, Last_Relay_Change, Motor_Enabled,
 * Error_Status[3]
 */


#endif
