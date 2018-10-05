#include "power.h"

motor_speed_t Motor_Speed = SLOW;
motor_movement_t Motor_Movement = HALT;
bool Motor_Enabled = false;
bool Magnet_Enabled = false;
unsigned long Magnet_Count = 0;

unsigned long Last_Relay_Change = 0;
unsigned long Last_Motor_Disable = 0;


void initPowerOutputs() {

	// Configure and enable Timer1 unit
	TCCR1A = ((1 << COM1A1) | (1 << COM1B1) | (1 << WGM10));
	TCCR1B = (1 << CS12);

	// Set pins as outputs
	pinMode(MOTOR_DIR_PIN, OUTPUT);
	pinMode(MOTOR_PWM_PIN, OUTPUT);
	pinMode(MAGNET_PWM_PIN, OUTPUT);
}

void setMagnetOutput(bool enable) {
	Magnet_Enabled = enable;
	if(Magnet_Enabled) {
		OCR1BL = PWM_MAGNET_PULSE;
		Magnet_Count = 0;
		enablePulseCounter();
	}
	else {
		disablePulseCounter();
		OCR1BL = 0;
	}
}

void setMotorOutput(motor_movement_t movement) {

	// Skip if no change needed
	if(Motor_Movement == movement) {
		return;
	}

	switch(movement) {
		case FORWARD: {
			if(Motor_Movement == BACKWARD) {
				OCR1AL = 0;
				disableWatchdog();
				Last_Motor_Disable = millis();
				delay(MOTOR_FLYBACK_DELAY);
				digitalWrite(MOTOR_DIR_PIN, LOW);
				Last_Relay_Change = millis();
			}
			while((millis() - Last_Relay_Change) < MOTOR_RELAY_CHANGE_DELAY) {
				// Let the relay settle
			}
			OCR1AL = ((Motor_Speed == SLOW) ? PWM_SPEED_SLOW : PWM_SPEED_FAST);
			enableWatchdog();
			Motor_Enabled = true;
			break;
		}
		case BACKWARD: {
			if(Motor_Movement == FORWARD) {
				OCR1AL = 0;
				disableWatchdog();
				Last_Motor_Disable = millis();
			}
			while((millis() - Last_Motor_Disable) < MOTOR_FLYBACK_DELAY) {
				// Let the motor discharge
			}
			digitalWrite(MOTOR_DIR_PIN, HIGH);
			Last_Relay_Change = millis();
			delay(MOTOR_RELAY_CHANGE_DELAY);
			OCR1AL = ((Motor_Speed == SLOW) ? PWM_SPEED_SLOW : PWM_SPEED_FAST);
			enableWatchdog();
			Motor_Enabled = true;
			break;
		}
		default:
		case HALT: {
			OCR1AL = 0;
			disableWatchdog();
			Last_Motor_Disable = millis();
			if(Motor_Movement == BACKWARD) {
				delay(MOTOR_FLYBACK_DELAY);
				digitalWrite(MOTOR_DIR_PIN, LOW);
				Last_Relay_Change = millis();
			}
			Motor_Enabled = false;
			break;
		}
	}
	Motor_Movement = movement;
	return;
}

void setMotorSpeed(motor_speed_t speed) {
	Motor_Speed = speed;
	if(Motor_Enabled) {
		OCR1AL = ((Motor_Speed == SLOW) ? PWM_SPEED_SLOW : PWM_SPEED_FAST);
	}
	return;
}

bool motorEnabled() {
	return Motor_Enabled;
}

void enablePulseCounter() {
	TIMSK1 = (1 << TOIE1);
	return;
}

void disablePulseCounter() {
	TIMSK1 = 0;
	return;
}

ISR(TIMER1_OVF_vect) {
	if (Magnet_Count++ >= MAGNET_PULSE_LENGTH) {
		OCR1BL = PWM_MAGNET_HOLD;
		disablePulseCounter();
	}
	return;
}
