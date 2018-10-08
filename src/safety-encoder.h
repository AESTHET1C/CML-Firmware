/* Encoder Module
 *
 * Used to asynchronously keep track of motor position using a quadrature encoder
 *
 * This is a sub-module of the Safety Module.
 *
 * This module can theoretically track encoder movement up to roughly 640,000 updates per second.
 * In practice, fewer than ~150,000 updates must occur per second to allow additional code to run.
 *
 * Encoder position is updated when either of the encoder pins change state, as handled by the
 * INT0 and INT1 interrupts. These interrupt routines are written in assembly to minimize
 * processing time by fast encoder movement. In these routines, the current and previous states
 * of the two encoder pins are arranged into a 4-bit number, and that number is used to quickly
 * jump to code that modifies the encoder position data by the correct about.
 * The table of encoder position changes is shown below.
 *
 *             ______      ______
 * pin0  _____|     |_____|     |_____
 *          ______      ______      __
 * pin1  __|     |_____|     |_____|
 *    <--- negative         positive --->
 *
 *       4-bit number
 *  ----------------------
 *  new   new   old   old   delta   delta
 *  pin1  pin0  pin1  pin0  (INT1)  (INT0)
 *  ----  ----  ----  ----  ------  -------
 *  0     0     0     0     0       0
 *  0     0     0     1     +1      +1
 *  0     0     1     0     -1      -1
 *  0     0     1     1     +2      -2
 *  0     1     0     0     -1      -1
 *  0     1     0     1     0       0
 *  0     1     1     0     -2      +2
 *  0     1     1     1     +1      +1
 *  1     0     0     0     +1      +1
 *  1     0     0     1     -2      +2
 *  1     0     1     0     0       0
 *  1     0     1     1     -1      -1
 *  1     1     0     0     +2      -2
 *  1     1     0     1     -1      -1
 *  1     1     1     0     +1      +1
 *  1     1     1     1     0       0
 *
 * Based on Paul Stoffregen's Encoder library <http://www.pjrc.com/teensy/td_libs_Encoder.html>
 * Written by Ana Tavares <tavaresa13@gmail.com>
 */

#ifndef safety_encoder_h
#define safety_encoder_h
#include <arduino.h>

/////////////////////////
// PIN DEFINITIONS
/////////////////////////

const byte ENC_A_PIN = 2;
const byte ENC_B_PIN = 3;


/////////////////////////
// DATA STRUCTURES
/////////////////////////

typedef struct {
  uint8_t state;
  int32_t position;
} encoder_data_t;

/////////////////////////
// AVAILABLE FUNCTIONS
/////////////////////////

void initEncoder();
/*
 * Initializes encoder tracking
 * Should be called before using motor functions
 *
 * Initialization involves pin and interrupt configuration, and determining initial pin states.
 *
 * Affects Encoder_Data
 */

int32_t getEncoderPos();
/*
 * Returns the current absolute position of the encoder
 *
 * OUTPUT: Encoder position
 */

void homeEncoder();
/*
 * Sets the current encoder position to "0"
 *
 * Affects Encoder_Data
 */


#endif
