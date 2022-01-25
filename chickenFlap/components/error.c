#include "error.h"

void error_init(){
	error.stateMachine = false;
	error.motorMaxRunningTime = false;
	error.safetyTest = false;
	error.timerTimes = false;
	error.displayInit = false;
	error.emptyBattery = false;

	warning.startAnimation = false;
	warning.lowBattery = false;
	warning.noBatteryData = false;

	reset_error_delay(&errTimerTimes);
	reset_error_delay(&errDisplayInit);
	reset_error_delay(&warnNoBatteryData);
}

void error_update(){
	checkAllErrors();

	setMotorEnable();
	checkTimerTimes();
	checkDisplayInit();

	#ifdef ENABLE_BATTERY_CAPACITY_CHECK
		checkBatteryCapacity();
	#endif
	#ifdef SSD13006_ENABLE_START_ANIMATION
		checkStartAnimation();
	#endif
}

void checkAllErrors(){
	if(
			error.stateMachine || error.motorMaxRunningTime || error.safetyTest || error.timerTimes
			|| error.displayInit || error.emptyBattery
		)
		nextState(STATE_ERROR);
}

void setMotorEnable(){
	if(error.motorMaxRunningTime || error.safetyTest)
		flap.motorEnable = false;
}

void checkTimerTimes(){
	// tmpBool is true if both timer actions are activated and the times are the same
	bool tmpBool = timer.timerState == TIMER_ACTIVE
					&& timer.closeFlapTime_Dec_H == timer.openFlapTime_Dec_H
					&& timer.closeFlapTime_One_H == timer.openFlapTime_One_H
					&& timer.closeFlapTime_Dec_M == timer.openFlapTime_Dec_M
					&& timer.closeFlapTime_One_M == timer.openFlapTime_One_M;

	if(error_delay(&errTimerTimes, tmpBool, TIMEOUT_ERROR_TIMER_TIMES))
		error.timerTimes = true;
}

void checkDisplayInit(){
	if(error_delay(&errDisplayInit, SSD1306.Initialized == 0, TIMEOUT_ERROR_DISPLAY_INIT))
		error.displayInit = true;
}

void checkBatteryCapacity(){
	if(bms.batteryCapapcityPercentage < WARNING_BATTERY_LOW_PERCENTAGE)
		warning.lowBattery = true;

	// 0% value means the ADC cannot work properly
	// In this case the error state should not be switched to
	if(error_delay(&warnNoBatteryData, bms.batteryCapapcityPercentage == 0, TIMEOUT_WARN_NO_BATTERY_DATA)) {
		warning.noBatteryData = true;
		return;
	}

	if(bms.batteryCapapcityPercentage < ERROR_BATTERY_EMPTY_PERCENTAGE)
		error.emptyBattery = true;
}

void checkStartAnimation(){
	if(warning.startAnimation)
		// Stop start animation in error case
		startAnimation.enable = false;
}
