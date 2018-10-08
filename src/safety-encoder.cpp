#include "safety-encoder.h"

encoder_data_t Encoder_Data;

void initEncoder() {

  // Configure encoder sense pins as input pullups
  pinMode(ENC_A_PIN, INPUT_PULLUP);
  pinMode(ENC_B_PIN, INPUT_PULLUP);

  // Set starting state
  Encoder_Data.position = 0;
  delayMicroseconds(2000);
  Encoder_Data.state = ((PIND & (0b00001100)) >> 2);

  // Enable Ext pin interrupts
  EICRA = ((1 << ISC10) | (1 << ISC00));
  EIMSK = ((1 << INT1) | (1 << INT0));

  return;
}

int32_t getEncoderPos() {
  noInterrupts();
  int32_t Return_Value = Encoder_Data.position;
  interrupts();
  return Return_Value;
}

void homeEncoder() {
  noInterrupts();
  Encoder_Data.position = 0;
  interrupts();
  return;
}

ISR(INT0_vect) {
  asm volatile (
    // Get the state (old + new) of the encoder
    "ld   r22, X"       "\n\t"  // r22 = old state
    "in   r23, %[pind]" "\n\t"
    "andi r23, %[mask]" "\n\t"
    "or   r22, r23"     "\n\t"  // r22 = new + old state

    // Calculate the correct action to take
    "ldi  r30, lo8(pm(L%=table))" "\n\t"
    "ldi  r31, hi8(pm(L%=table))" "\n\t"
    "add  r30, r22"               "\n\t"
    "adc  r31, __zero_reg__"      "\n\t"

    // Save the new state of the encoder
    "asr  r22"       "\n\t"
    "asr  r22"       "\n\t"
    "st   X+, r22"   "\n\t"

    // Load the position of the encoder for updating
    "ld   r22, X+"   "\n\t"
    "ld   r23, X+"   "\n\t"
    "ld   r24, X+"   "\n\t"
    "ld   r25, X+"   "\n\t"
    "ijmp"           "\n\t"
  "L%=table:"        "\n\t"
    "rjmp L%=end"    "\n\t"  // 0000
    "rjmp L%=plus1"  "\n\t"  // 0001
    "rjmp L%=minus1" "\n\t"  // 0010
    "rjmp L%=minus2" "\n\t"  // 0011
    "rjmp L%=minus1" "\n\t"  // 0100
    "rjmp L%=end"    "\n\t"  // 0101
    "rjmp L%=plus2"  "\n\t"  // 0110
    "rjmp L%=plus1"  "\n\t"  // 0111
    "rjmp L%=plus1"  "\n\t"  // 1000
    "rjmp L%=plus2"  "\n\t"  // 1001
    "rjmp L%=end"    "\n\t"  // 1010
    "rjmp L%=minus1" "\n\t"  // 1011
    "rjmp L%=minus2" "\n\t"  // 1100
    "rjmp L%=minus1" "\n\t"  // 1101
    "rjmp L%=plus1"  "\n\t"  // 1110
    "rjmp L%=end"    "\n\t"  // 1111
  "L%=minus2:"       "\n\t"
    "subi r22, 2"    "\n\t"
    "sbci r23, 0"    "\n\t"
    "sbci r24, 0"    "\n\t"
    "sbci r25, 0"    "\n\t"
    "rjmp L%=store"  "\n\t"
  "L%=minus1:"       "\n\t"
    "subi r22, 1"    "\n\t"
    "sbci r23, 0"    "\n\t"
    "sbci r24, 0"    "\n\t"
    "sbci r25, 0"    "\n\t"
    "rjmp L%=store"  "\n\t"
  "L%=plus2:"        "\n\t"
    "subi r22, 254"  "\n\t"
    "rjmp L%=z"      "\n\t"
  "L%=plus1:"        "\n\t"
    "subi r22, 255"  "\n\t"
  "L%=z:"            "\n\t"
    "sbci r23, 255"  "\n\t"
    "sbci r24, 255"  "\n\t"
    "sbci r25, 255"  "\n\t"
  "L%=store:"        "\n\t"
    "st -X, r25"     "\n\t"
    "st -X, r24"     "\n\t"
    "st -X, r23"     "\n\t"
    "st -X, r22"     "\n\t"
  "L%=end:"          "\n"
  :
  : "x" (&Encoder_Data),
    [pind] "I" (_SFR_IO_ADDR(PIND)),
    [mask] "I" (0b00001100)
  : "r22",
    "r23",
    "r24",
    "r25",
    "r30",
    "r31"
  );
  return;
}

ISR(INT1_vect) {
  asm volatile (
    // Get the state (old + new) of the encoder
    "ld   r22, X"       "\n\t"  // r22 = old state
    "in   r23, %[pind]" "\n\t"
    "andi r23, %[mask]" "\n\t"
    "or   r22, r23"     "\n\t"  // r22 = new + old state

    // Calculate the correct action to take
    "ldi  r30, lo8(pm(L%=table))" "\n\t"
    "ldi  r31, hi8(pm(L%=table))" "\n\t"
    "add  r30, r22"               "\n\t"
    "adc  r31, __zero_reg__"      "\n\t"

    // Save the new state of the encoder
    "asr  r22"       "\n\t"
    "asr  r22"       "\n\t"
    "st   X+, r22"   "\n\t"

    // Load the position of the encoder for updating
    "ld   r22, X+"   "\n\t"
    "ld   r23, X+"   "\n\t"
    "ld   r24, X+"   "\n\t"
    "ld   r25, X+"   "\n\t"
    "ijmp"           "\n\t"
  "L%=table:"        "\n\t"
    "rjmp L%=end"    "\n\t"  // 0000
    "rjmp L%=plus1"  "\n\t"  // 0001
    "rjmp L%=minus1" "\n\t"  // 0010
    "rjmp L%=plus2"  "\n\t"  // 0011
    "rjmp L%=minus1" "\n\t"  // 0100
    "rjmp L%=end"    "\n\t"  // 0101
    "rjmp L%=minus2" "\n\t"  // 0110
    "rjmp L%=plus1"  "\n\t"  // 0111
    "rjmp L%=plus1"  "\n\t"  // 1000
    "rjmp L%=minus2" "\n\t"  // 1001
    "rjmp L%=end"    "\n\t"  // 1010
    "rjmp L%=minus1" "\n\t"  // 1011
    "rjmp L%=plus2"  "\n\t"  // 1100
    "rjmp L%=minus1" "\n\t"  // 1101
    "rjmp L%=plus1"  "\n\t"  // 1110
    "rjmp L%=end"    "\n\t"  // 1111
  "L%=minus2:"       "\n\t"
    "subi r22, 2"    "\n\t"
    "sbci r23, 0"    "\n\t"
    "sbci r24, 0"    "\n\t"
    "sbci r25, 0"    "\n\t"
    "rjmp L%=store"  "\n\t"
  "L%=minus1:"       "\n\t"
    "subi r22, 1"    "\n\t"
    "sbci r23, 0"    "\n\t"
    "sbci r24, 0"    "\n\t"
    "sbci r25, 0"    "\n\t"
    "rjmp L%=store"  "\n\t"
  "L%=plus2:"        "\n\t"
    "subi r22, 254"  "\n\t"
    "rjmp L%=z"      "\n\t"
  "L%=plus1:"        "\n\t"
    "subi r22, 255"  "\n\t"
  "L%=z:"            "\n\t"
    "sbci r23, 255"  "\n\t"
    "sbci r24, 255"  "\n\t"
    "sbci r25, 255"  "\n\t"
  "L%=store:"        "\n\t"
    "st -X, r25"     "\n\t"
    "st -X, r24"     "\n\t"
    "st -X, r23"     "\n\t"
    "st -X, r22"     "\n\t"
  "L%=end:"          "\n"
  :
  : "x" (&Encoder_Data),
    [pind] "I" (_SFR_IO_ADDR(PIND)),
    [mask] "I" (0b00001100)
  : "r22",
    "r23",
    "r24",
    "r25",
    "r30",
    "r31"
  );
  return;
}
