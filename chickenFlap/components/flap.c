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
	flap.lastTimeMotorRuns = 0;

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
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

	// Start timer
	if (!flap.motorIsRuning) {
		motorStartingTime = millis();
		flap.motorIsRuning = true;
	} else
		flap.lastTimeMotorRuns = millis();

	// Start motor action
	if(direction == FLAP_OPENING) {
		setDutyCycle(MOTOR_FLAP_OPEN, MOTOR_SPEED_PERCENTAGE);
	} else if (direction == FLAP_CLOSING) {
		setDutyCycle(MOTOR_FLAP_CLOSE, MOTOR_SPEED_PERCENTAGE);
	}

	// Calculate motor running time
	flap.motorRunningTime = millis() - motorStartingTime;

	// Measure and set operation time if flap.motorOperationTime is set to 0 before call of this function
	measureMotorOperationTime();

	// Switch to error state if the motor never stops
	if(flap.motorRunningTime >= TIMEOUT_ERROR_MAX_MOTOR_RUNNING_TIME)
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

void setDutyCycle(uint8_t motorDirection, uint8_t duty){
	if((motorDirection != MOTOR_FLAP_OPEN && motorDirection != MOTOR_FLAP_CLOSE && motorDirection != MOTOR_FLAP_BOTH_DIRECTIONS) || duty < 0 || duty > 100)
		return;

	switch(motorDirection){
	case MOTOR_FLAP_OPEN:
		TIM4->CCR4 = 0;
		TIM4->CCR3 = (uint32_t)(MAX_TIMER_PWM * ((float)duty/100));
		break;
	case MOTOR_FLAP_CLOSE:
		TIM4->CCR3 = 0;
		TIM4->CCR4 = (uint32_t)(MAX_TIMER_PWM * ((float)duty/100));
		break;
	case MOTOR_FLAP_BOTH_DIRECTIONS:
		TIM4->CCR3 = 0;
		TIM4->CCR4 = 0;
	}
}

void stopMotor(){
	setDutyCycle(MOTOR_FLAP_BOTH_DIRECTIONS, 0);

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

void measureMotorOperationTime(){
	// Stop if time is already set
	if(flap.motorOperationTimeSetted)
		return;

	// Calculate operating time of the motor
	if(flap.motorIsRuning){
		flap.motorOperationTime = flap.motorRunningTime;
	}
}
