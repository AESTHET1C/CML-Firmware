/* Power Output Handling Module
 *
 * Used to initialize, control, and manage the two power outputs
 *
 * This includes PWM output for the motor and electromagnet, as well as direction of the motor.
 *
 * The motor is automatically disabled for a short while before and after switching directions.
 * This may block code from running temporarily.
 *
 * The electromagnet automatically outputs at a higher duty cycle for a short while when enabled.
 * This is referred to as the "pulse".
 *
 * The Timer1 unit is used to control the two power outputs in phase-correct PWM mode,
 * running at about 122 Hz. In addition, the overflow interrupt is used to time the magnet
 * pulse length. This interrupt is disabled when the pulse length has already been met.
 *
 * Written by Ana Tavares <tavaresa13@gmail.com>
 */

#ifndef power_h
#define power_h
#include <arduino.h>
#include "safety.h"

/////////////////////////
// CONFIGURATION VARIABLES
/////////////////////////

const byte MAGNET_PULSE_LENGTH = 15;  // Number of timer1 cycles (~122 Hz)

// PWM presets
const uint8_t PWM_SPEED_SLOW = 75;
const uint8_t PWM_SPEED_FAST = 255;
const uint8_t PWM_MAGNET_PULSE = 255;
const uint8_t PWM_MAGNET_HOLD = 100;

// Motor state delays
const unsigned int MOTOR_FLYBACK_DELAY = 100;
const unsigned int MOTOR_RELAY_CHANGE_DELAY = 250;


/////////////////////////
// PIN DEFINITIONS
/////////////////////////

const byte MOTOR_DIR_PIN = 8;
const byte MOTOR_PWM_PIN = 9;
const byte MAGNET_PWM_PIN = 10;


/////////////////////////
// ENUMERATIONS
/////////////////////////

typedef enum {
	SLOW,
	FAST
} motor_speed_t;

typedef enum {
	FORWARD,
	BACKWARD,
	HALT
} motor_movement_t;


/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initPowerOutputs();
/*
 * Initializes both power outputs
 * Must be called at startup
 *
 * Initialization involves setting status variables, pin configuration, and PWM values.
 * Initial motor direction is also set.
 */

void setMagnetOutput(bool enable);
/*
 * Enables or disables the electromagnet
 *
 * Affects Magnet_Enabled
 * INPUT:  State of being enabled
 */

void setMotorOutput(motor_movement_t movement);
/*
 * Sets the motor output type
 *
 * Affects Motor_Enabled
 * INPUT:  Type of movement
 */

void setMotorSpeed(motor_speed_t speed);
/*
 * Sets the speed of the motor
 *
 * Affects Motor_Speed
 * INPUT:  Motor speed
 */

bool motorEnabled();
/*
 * Gets the state of the motor
 *
 * OUTPUT: State of being enabled
 */


/////////////////////////
// INTERNAL FUNCTIONS
/////////////////////////

void enablePulseCounter();
/*
 * Enables the Timer1 overflow interrupt
 * Used when the magnet is enabled by setMagnetOutput()
 *
 * This interrupt is used to count cycles for the electromagnet pulse.
 * It will reduce the duty cycle of the magnet when the pulse completes.
 *
 * This function should be called when enabling the electromagnet.
 *
 * Affects timer register TIMSK1
 */

void disablePulseCounter();
/*
 * Disables the Timer1 overflow interrupt
 * Used by the Timer1 overflow interrupt
 *
 * This interrupt is used to count cycles for the electromagnet pulse.
 * If disabled, the electromagnet pulse will not terminate.
 *
 * This function should be called when disabling the electromagnet.
 *
 * Affects timer register TIMSK1
 */


#endif
