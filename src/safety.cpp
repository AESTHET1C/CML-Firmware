#include "safety.h"

bool Watchdog_Enabled = false;
long Watchdog_Queue[WATCHDOG_CYCLES + 1];
byte Watchdog_Queue_Ptr = 0;
bool Is_Faulted = false;

void initWatchdog() {

	initEncoder();
	initErrors();

	// Configure and enable Timer2 unit
	TCCR2B = ((1 << CS22) | (1 << CS21) | (1 << CS20));
	TIMSK2 = (1 << TOIE2);

	return;
}

void enableWatchdog() {
	long Temp_Queue_Value = getEncoderPos() - (0x80000000);
	for(byte i = 0; i <= WATCHDOG_CYCLES; i++) {
		Watchdog_Queue[i] = Temp_Queue_Value;
	}
	Watchdog_Enabled = true;
	return;
}

void disableWatchdog() {
	Watchdog_Enabled = false;
	return;
}

bool isFaulted() {
	return Is_Faulted;
}

void clearFaults() {
	Is_Faulted = false;
	clearErrors();
	return;
}

void raiseWatchdogError() {
	setMotorOutput(HALT);
	Is_Faulted = true;
	flagError(3);
	return;
}

ISR(TIMER2_OVF_vect) {
	// Handle error code updating, so main loop doesn't have to worry about it
	handleErrorCodeDisplay();

	// Handle motor/encoder watchdog
	if(Watchdog_Enabled) {
		long Current_Encoder_Pos = getEncoderPos();
		Watchdog_Queue[Watchdog_Queue_Ptr] = Current_Encoder_Pos;
		if(++Watchdog_Queue_Ptr > WATCHDOG_CYCLES) {
			Watchdog_Queue_Ptr = 0;
		}
		long Prev_Encoder_Pos = Watchdog_Queue[Watchdog_Queue_Ptr];
		if(abs(Current_Encoder_Pos - Prev_Encoder_Pos) <= WATCHDOG_THRESHOLD) {
			Serial.print("WATCHDOG ERROR @ ");
			Serial.print(Current_Encoder_Pos);
			Serial.print(" (");
			Serial.print(Prev_Encoder_Pos);
			Serial.print(")\n");
			raiseWatchdogError();
		}
	}
	return;
}
