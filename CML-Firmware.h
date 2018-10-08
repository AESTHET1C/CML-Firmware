/* Coal Mine Loader Firmware
 *
 * Allows for interactive control of the coal loader on the Eli Whitney Museum seasonal train display
 *
 * See the documentation folder for details.
 *
 * A state machine is used to keep track of progress in the motor movement routine,
 * using non-blocking code. Several features, including electromagnet "pulsing", direction reversal
 * delays, motor watchdog functionality, and error code display, are automatically handled
 * externally and are therefore not required to be included in the main loop.
 *
 * Written by Ana Tavares <tavaresa13@gmail.com>
 */

#ifndef main_h
#define main_h
#include <arduino.h>
#include "src/power.h"
#include "src/safety.h"

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

// The number of consecutive cycles a sensor (or button) must be engaged for it to register
// This is the prevent electrical noise from falsely triggering endstops and buttons
const unsigned int SENSOR_REQUIRED_COUNT = 5;

// Movement configuration
const long MOTOR_TRAVEL_TARGET = 60000;
const long MOTOR_MAX_MOVEMENT = 75000;
const long OVERSHOOT_BUFFER = 500;
const long UNDERSHOOT_BUFFER = 500;

// State delays
const unsigned int MAGNET_GRAB_DELAY = 250;
const unsigned int MOTOR_GRAB_DELAY = 500;
const unsigned int MOTOR_IDLE_DELAY = 2000;


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

const byte GO_PIN = A4;
const byte FORW_PIN = A3;
const byte BACK_PIN = A2;
const byte ENDSTOP_0_PIN = 4;
const byte ENDSTOP_1_PIN = 7;      // Unused
const byte ENDSTOP_0_LED_PIN = 5;
const byte ENDSTOP_1_LED_PIN = 6;  // Unused


/////////////////////////
// ENUMERATIONS
/////////////////////////

typedef enum {
	GO,
	FORW,
	BACK,
	ENDSTOP_0,
	ENDSTOP_1  // Unused
} sensor_t;

typedef enum {
	INIT,
	IDLE,
	DOWN,
	GRAB,
	UP,
	OVERRIDE,
	FAULTED
} state_t;


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

void initInputs();
/*
 * Initializes input pins
 * Must be called once at startup
 *
 * Initialization involves endstop and button pin configuration.
 */

bool sensorEngagedCurrent(sensor_t sensor);
/*
 * Gets the immediate state of a given sensor
 *
 * INPUT:  Sensor to check
 * OUTPUT: State of being engaged
 */


#endif
