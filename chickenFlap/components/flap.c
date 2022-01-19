#include "flap.h"

void flap_init(){
	motorStartingTime = 0;
	flap.motorIsRuning = false;
	flap.actuallyStateFlap = FLAP_CLOSED;
	flap.targetStateFlap = FLAP_CLOSED;
	flap.motorOperationTime = DEFAULT_MOTOR_RUNNING_TIME;
	flap.motorOperationTimeSetted = true;
	flap.motorWaitForButton = false;
	flap.motorEnable = true;
	flap.motorButtonCtrlTime = 0;
}

void openFlap() {
	// Check if motor is enabled
	if(!flap.motorEnable)
		return;

	// Check if flap is already closed or should not be closed
	if (flap.actuallyStateFlap == FLAP_OPENED || flap.targetStateFlap == FLAP_CLOSED)
		return;

	// Open flap
	motorCtrl(FLAP_OPENING);
}

void closeFlap(){
	// Check if motor is enabled
	if(!flap.motorEnable)
		return;

	// Check if flap is already closed or should not be closed
	if (flap.actuallyStateFlap == FLAP_CLOSED || flap.targetStateFlap == FLAP_OPENED)
		return;

	// Close flap
	motorCtrl(FLAP_CLOSING);
}

void motorCtrl(uint8_t direction) {
	// Wait for button action if the movement was break up in the middle
	if(flap.motorWaitForButton)
		return;

	// Update flap state
	flap.actuallyStateFlap = direction;

	// Start timer
	if (!flap.motorIsRuning) {
		motorStartingTime = millis();
		flap.motorIsRuning = true;
	}

	// Start motor action
	if(direction == FLAP_OPENING) {
		digitalWrite(MOTOR_FLAP_CLOSE, LOW); // Prevent that never both mosfets are switched on (short circuit)
		digitalWrite(MOTOR_FLAP_OPEN, HIGH);
	} else if (direction == FLAP_CLOSING) {
		digitalWrite(MOTOR_FLAP_OPEN, LOW); // Prevent that never both mosfets are switched on (short circuit)
		digitalWrite(MOTOR_FLAP_CLOSE, HIGH);
	}

	// Calculate motor running time
	flap.motorRunningTime = millis() - motorStartingTime;

	// Measure and set operation time if flap.motorOperationTime is set to 0 before call of this function
	measureMotorOperationTime();

	// Switch to error state if the motor never stops
	if(flap.motorRunningTime >= MAX_MOTOR_RUNNING_TIME)
		error.motorMaxRunningTime = true;

	// If the motor motion was break in the middle, stop the reverse motion after the same time it was break up before
	if(flap.motorButtonCtrlTime > 0){
		if(flap.motorIsRuning && flap.motorRunningTime >= flap.motorButtonCtrlTime) {
			stopMotor();
			flap.motorButtonCtrlTime = 0;
		}
	}

	// Wait for timer and stop motor
	else if (flap.motorRunningTime >= flap.motorOperationTime && flap.motorOperationTimeSetted)
		stopMotor();
}

void stopMotor(){
	digitalWrite(MOTOR_FLAP_OPEN, LOW);
	digitalWrite(MOTOR_FLAP_CLOSE, LOW);
	// Reset motorCtrl state
	flap.motorIsRuning = false;

	// Update flap status if operation time is setting
	if(flap.motorOperationTimeSetted && !flap.motorWaitForButton){
		if(flap.actuallyStateFlap == FLAP_OPENING)
			flap.actuallyStateFlap = FLAP_OPENED;
		else if(flap.actuallyStateFlap == FLAP_CLOSING)
			flap.actuallyStateFlap = FLAP_CLOSED;
	}
}

bool safetyTest(){
	if(digitalRead(MOTOR_FLAP_OPEN) == HIGH && digitalRead(MOTOR_FLAP_CLOSE) == HIGH){
		error.safetyTest = true;
		return true;
	}
	return false;
}

uint32_t calculateMotorStopTimeWithBatCap(){
	// Check for valid battery information
	if(bms.batteryCapapcityPercentage >= 0)
		return DEFAULT_MOTOR_RUNNING_TIME;

	float tmpBatteryCapacity = bms.batteryCapapcityPercentage/60;
	if (tmpBatteryCapacity >= 1)
		tmpBatteryCapacity = 1;

	uint8_t motorSpeed = MAX_MOTOR_SPEED * tmpBatteryCapacity;

	// time = distance/speed
	return motorStartingTime + (FLAP_MOTION_DISTANCE / motorSpeed);
}

void measureMotorOperationTime(){
	// Stop if time is already set
	if(flap.motorOperationTimeSetted)
		return;

	// Calculate operating time of the motor
	if(flap.motorIsRuning){
		flap.motorOperationTime = flap.motorRunningTime;
	}
}
