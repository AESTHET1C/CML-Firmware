#include "CML-Firmware.h"

unsigned int Sensor_Count[4] = {0, 0, 0, 0};
bool Sensor_Engaged[4] = {false, false, false, false};
motor_movement_t Override_Type = HALT;
state_t Current_State = INIT;

unsigned long State_Start = 0;

void setup() {
	Serial.begin(115200);
	initInputs();
	initWatchdog();
	initPowerOutputs();
	setMotorSpeed(SLOW);
	setMotorOutput(BACKWARD);
}

void loop() {

	// Handle endstop sensing
	for(byte Sensor = 0; Sensor <= ENDSTOP_0; Sensor++) {
		if(sensorEngagedCurrent(Sensor)) {
			if(++Sensor_Count[Sensor] >= SENSOR_REQUIRED_COUNT) {
				Sensor_Engaged[Sensor] = true;
				Sensor_Count[Sensor] -= 1;
			}
		}
		else {
			Sensor_Engaged[Sensor] = false;
			Sensor_Count[Sensor] = 0;
		}
	}

	// Handle motor faults
	if(isFaulted()) {
		Current_State = FAULTED;
	}

	// Handle motor overrides
	if(Sensor_Engaged[FORW] || Sensor_Engaged[BACK]) {
		if(Sensor_Engaged[FORW] && Sensor_Engaged[BACK]) {
			Override_Type = HALT;
		}
		else if(Sensor_Engaged[FORW]) {
			Override_Type = FORWARD;
		}
		else {
			Override_Type = BACKWARD;
		}
		clearFaults();
		Current_State = OVERRIDE;
	}
	else if(Current_State == OVERRIDE) {
		setMotorOutput(HALT);
		Current_State = IDLE;
	}

	// State machine
	switch(Current_State) {
		case INIT: {
			if(Sensor_Engaged[ENDSTOP_0]) {
				setMotorOutput(HALT);
				homeEncoder();
				State_Start = millis();
				Current_State = IDLE;
			}
			break;
		}
		case IDLE: {
			if(Sensor_Engaged[GO] && ((millis() - State_Start) >= MOTOR_IDLE_DELAY)) {
				State_Start = millis();
				setMotorSpeed(FAST);
				setMotorOutput(FORWARD);
				Current_State = DOWN;
			}
			break;
		}
		case DOWN: {
			if(getEncoderPos() >= MOTOR_TRAVEL_TARGET) {
				setMotorOutput(HALT);
				setMagnetOutput(true);
				Serial.print("TRAVEL TIME: ");
				Serial.print(millis() - State_Start);
				Serial.print("\n");
				State_Start = millis();
				Current_State = GRAB;
			}
			break;
		}
		case GRAB: {
			if((millis() - State_Start) >= MOTOR_GRAB_DELAY) {
				setMotorOutput(BACKWARD);
				State_Start = millis();
				Current_State = UP;
			}
			break;
		}
		case UP: {
			if(getEncoderPos() <= -OVERSHOOT_BUFFER) {
				setMotorOutput(HALT);
				setMagnetOutput(false);
				flagError(2);
				Serial.print("END @ POS: ");
				Serial.print(getEncoderPos());
				Serial.print(" (OVERSHOT!)\n\n");
				State_Start = millis();
				Current_State = IDLE;
			}
			else if(Sensor_Engaged[ENDSTOP_0]) {
				setMotorOutput(HALT);
				setMagnetOutput(false);
				Serial.print("END @ POS: ");
				Serial.print(getEncoderPos());

				if(getEncoderPos() >= UNDERSHOOT_BUFFER) {
					flagError(1);
					Serial.print(" (UNDERSHOT!)");
				}

				Serial.print("\n\n");
				homeEncoder();
				State_Start = millis();
				Current_State = IDLE;
			}
			break;
		}
		case OVERRIDE: {
			setMotorSpeed(SLOW);
			setMagnetOutput(false);
			switch(Override_Type) {
				case FORWARD:
					if(getEncoderPos() < MOTOR_MAX_MOVEMENT) {
						setMotorOutput(FORWARD);
					}
					else {
						setMotorOutput(HALT);
						setMotorOutput(FORWARD);
					}
					break;
				case BACKWARD:
					if(Sensor_Engaged[ENDSTOP_0]) {
						homeEncoder();
						setMotorOutput(HALT);
					}
					else {
						setMotorOutput(BACKWARD);
					}
					break;
				default:
				case HALT:
					setMotorOutput(HALT);
					break;
			}
			break;
		}
		default:
		case FAULTED: {
			setMagnetOutput(false);
			setMotorOutput(HALT);
			break;
		}
	}
}

void initInputs() {
	pinMode(GO_PIN, INPUT_PULLUP);
	pinMode(FORW_PIN, INPUT_PULLUP);
	pinMode(BACK_PIN, INPUT_PULLUP);
	pinMode(ENDSTOP_0_PIN, INPUT_PULLUP);
	pinMode(ENDSTOP_0_LED_PIN, OUTPUT);
	return;
}

bool sensorEngagedCurrent(sensor_t sensor) {
	bool Value;
	switch(sensor){
		case GO:
			return(!digitalRead(GO_PIN));
		case FORW:
			return(!digitalRead(FORW_PIN));
		case BACK:
			return(!digitalRead(BACK_PIN));
		case ENDSTOP_0:
			Value = digitalRead(ENDSTOP_0_PIN);
			digitalWrite(ENDSTOP_0_LED_PIN, !Value);
			return(Value);
		default:
			return false;
	}
}
