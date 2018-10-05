/* Encoder Module
 *
 * Used to asynchronously keep track of motor position
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

int32_t getEncoderPos();

void homeEncoder();


#endif
