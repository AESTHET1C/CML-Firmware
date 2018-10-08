/* Error Code Module
 *
 * Used to initialize and control the status LED to show error codes
 *
 * This is a sub-module of the Safety Module.
 *
 * Error codes are displayed by blinking the corresponding number of times to the error code.
 * If multiple errors are set, each error is displayed in increasing order.
 *
 * Updating of the error code display is not handled automatically. handleErrorCodeDisplay() must
 * be called frequently elsewhere to ensure the status LED is displaying error codes correctly.
 *
 * Each error code is displayed within a "cycle". Each cycle consists of ERROR_CODES "ticks".
 * Each tick lasts for ERROR_TICK_TIME milliseconds. At the beginning of each tick,
 * the LED may or may not turn on to represent a "blink".
 * After ERROR_BLINK_TIME elapses within a tick, the LED will be disabled.
 *
 * The following diagram assumes ERROR_CODES = 3 and the error code 2 is being displayed repeatedly:
 *
 * |_________           |_________           |                    |_________           |
 * |         |__________|         |_______________________________|         |__________| and so on...
 *  --blink--            --blink--
 *  --------tick-------- --------tick-------- --------tick--------
 * -----------------------------cycle-----------------------------
 *
 * Written by Ana Tavares <tavaresa13@gmail.com>
 */

#ifndef safety_error_h
#define safety_error_h
#include <arduino.h>

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

#define ERROR_CODES 4

const unsigned int ERROR_TICK_TIME = 250;
const unsigned int ERROR_BLINK_TIME = 100;


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

const byte ERROR_PIN = 13;


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initErrors();
/*
 * Initializes error code handling
 * Must be called at startup
 *
 * Initialization involves setting status variables and pin configuration.
 *
 * Affects Error_Status[]
 */

void handleErrorCodeDisplay();
/*
 * Updates the status LED to display error codes
 * Must be placed within a loop that executes regularly
 *
 * Affects Error_Tick_Start, Error_Tick_Curr, and Error_Cycle_Blinks
 */

void flagError(byte error);
/*
 * Sets a single error code to true
 *
 * Affects Error_Status[]
 * INPUT:  Error code to set (1-indexed)
 */

void clearErrors();
/*
 * Clears all error codes
 *
 * Affects Error_Status[]
 */


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

byte getBlinksNext(byte blinks_prev);
/*
 * Scans through active errors and determines the next one to display
 * Used by handleErrorCodeDisplay()
 *
 * If no errors are flagged, returns 0.
 *
 * INPUT:  Previous displayed error (1-indexed)
 * OUTPUT: Next error to display (1-indexed)
 */


#endif
