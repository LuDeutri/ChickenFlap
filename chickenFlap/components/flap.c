#include "flap.h"

flap_t flap;
util_time_t motorStartingTime;

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
	flap.lastTimeMotorRuns = 0;
	flap.motorSpeed = 90;

	// Disable low side switches
	digitalWrite(MOTOR_ENABLE_FLAP_OPEN, LOW);
	digitalWrite(MOTOR_ENABLE_FLAP_CLOSE, LOW);
	digitalWrite(MOTOR_VCC_CTRL, VCC_OPEN);

	// Start PWM timer
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

void openFlap() {
	// Check if motor is enabled
	if(!flap.motorEnable)
		return;

	// Check if flap is already open or should not be open
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

	if (!flap.motorIsRuning){
		// Switch VCC ctrl relay and update first GPIOs before go further
		switch(direction){
		case FLAP_OPENING:
			digitalWrite(MOTOR_VCC_CTRL, VCC_OPEN);
			break;
		case FLAP_CLOSING:
			digitalWrite(MOTOR_VCC_CTRL, VCC_CLOSE);
			break;
		default:
			// Should never be here
			break;
		}
		// Delay for switching relay
		dartDigital_update();
		delayMillis(500);

		// Start timer
		motorStartingTime = millis();
		flap.motorIsRuning = true;
	} else
		flap.lastTimeMotorRuns = millis();

	// Start motor action
	// Update pwm generator
	TIM1->CCR3 = (uint32_t)(MAX_TIMER_PWM * ((float)flap.motorSpeed/100));

	if(direction == FLAP_OPENING)
		digitalWrite(MOTOR_ENABLE_FLAP_OPEN, HIGH);
	else if (direction == FLAP_CLOSING)
		digitalWrite(MOTOR_ENABLE_FLAP_CLOSE, HIGH);

	// Calculate motor running time
	flap.motorRunningTime = millis() - motorStartingTime;

	// Measure and set operation time if flap.motorOperationTime is set to 0 before call of this function
	measureMotorOperationTime();

	// Switch to error state if the motor never stops or is running 3 Seconds about setted runningTime
	if(flap.motorRunningTime >= TIMEOUT_ERROR_MAX_MOTOR_RUNNING_TIME || ((flap.motorRunningTime >= flap.motorOperationTime+3000) && flap.motorOperationTimeSetted))
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
	// Update pwm generator
	TIM1->CCR3 = 0;

	// Reset motorCtrl state
	flap.motorIsRuning = false;

	// Disable low side switches
	digitalWrite(MOTOR_ENABLE_FLAP_OPEN, LOW);
	digitalWrite(MOTOR_ENABLE_FLAP_CLOSE, LOW);
	digitalWrite(MOTOR_VCC_CTRL, VCC_OPEN);

	// Update flap status if operation time is setting
	if(flap.motorOperationTimeSetted && !flap.motorWaitForButton){
		if(flap.actuallyStateFlap == FLAP_OPENING)
			flap.actuallyStateFlap = FLAP_OPENED;
		else if(flap.actuallyStateFlap == FLAP_CLOSING)
			flap.actuallyStateFlap = FLAP_CLOSED;
	}
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

void setMotorSpeed(){
	// Value is only adjustable if the value is screend on the lcd screen
	if(display.displayPage != DISPLAY_CONFIG_MOTORSPEED)
		return;

	float raw = mapADCVoltage(ADC_MOTORSPEED, ADC_VREF);
	if(raw == 0) flap.motorSpeed = 0;
	else flap.motorSpeed = (uint8_t)(raw/ADC_VREF*100);
}
