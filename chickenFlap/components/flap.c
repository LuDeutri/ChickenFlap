#include "flap.h"

void flap_init(){
	motorStartingTime = 0;
	flap.motorIsRuning = false;
	flap.actuallyStateFlap = FLAP_CLOSED;
	flap.targetStateFlap = FLAP_CLOSED;
}

void openFlap() {
	// Check if flap is already closed or should not be closed
	if (flap.actuallyStateFlap == FLAP_OPENED || flap.targetStateFlap == FLAP_CLOSED)
		return;

	// Open flap
	motorCtrl(FLAP_OPENED);
}

void closeFlap(){
	// Check if flap is already closed or should not be closed
	if (flap.actuallyStateFlap == FLAP_CLOSED || flap.targetStateFlap == FLAP_OPENED)
		return;

	// Close flap
	motorCtrl(FLAP_CLOSED);
}

void motorCtrl(uint8_t direction) {
	// Start timer
	if (!flap.motorIsRuning) {
		motorStartingTime = millis();
		flap.motorIsRuning = true;
	}

	// Start motor action
	if(direction == FLAP_OPENED) {
		digitalWrite(MOTOR_FLAP_CLOSE, LOW); // Prevent that never both mosfets are switched on (short circuit)
		digitalWrite(MOTOR_FLAP_OPEN, HIGH);
	} else if (direction == FLAP_CLOSED) {
		digitalWrite(MOTOR_FLAP_OPEN, LOW); // Prevent that never both mosfets are switched on (short circuit)
		digitalWrite(MOTOR_FLAP_CLOSE, HIGH);
	}

	// Wait for timer and stop motor
	if (millis() >= motorStartingTime + MAX_MOTOR_RUNNING_TIME || (millis() >= calculateMotorStopTime())) {
		digitalWrite(MOTOR_FLAP_OPEN, LOW);
		digitalWrite(MOTOR_FLAP_CLOSE, LOW);

		// Reset motorCtrl state
		flap.motorIsRuning = false;
	}
}

void stopMotor(){
	digitalWrite(MOTOR_FLAP_OPEN, LOW);
	digitalWrite(MOTOR_FLAP_CLOSE, LOW);
	// Reset motorCtrl state
	flap.motorIsRuning = false;
}

bool safetyTest(){
	if(digitalRead(MOTOR_FLAP_OPEN) == HIGH && digitalRead(MOTOR_FLAP_CLOSE) == HIGH)
		return true;
	return false;
}

uint32_t calculateMotorStopTime(){
	float tmpBatteryCapacity = bms.batteryCapapcityPercentage/60;
	if (tmpBatteryCapacity >= 1)
		tmpBatteryCapacity = 1;

	uint8_t motorSpeed = MAX_MOTOR_SPEED * tmpBatteryCapacity;

	// time = distance/speed
	return motorStartingTime + (FLAP_MOTION_DISTANCE / motorSpeed);
}
