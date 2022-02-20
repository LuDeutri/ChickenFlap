#include "error.h"

void error_init(){
	error.stateMachine = false;
	error.motorMaxRunningTime = false;
	error.safetyTest = false;
	error.timerTimes = false;
	error.displayInit = false;
	error.emptyBattery = false;
	error.timeNotificationShown = 0;
	strcpy(error.errorDescription, "");

	warning.startAnimation = false;
	warning.lowBattery = false;
	warning.noBatteryData = false;
	warning.timerTimes = false;
	warning.overallWarning = false;

	reset_error_delay(&errTimerTimes);
	reset_error_delay(&errDisplayInit);
	reset_error_delay(&warnNoBatteryData);
	reset_error_delay(&errBatteryEmpty);
	reset_error_delay(&warnBatteryLow);

	for(uint8_t i=0; i < sizeof(error.notifications)/sizeof(error.notifications[0]); i++)
		error.notifications[i] = false;
}

void error_update(){
	checkAllErrors();
	checkForWarnings();

	setMotorEnable();
	checkTimerTimes();
	checkDisplayInit();

	if(millis() > error.timeNotificationShown + TIMEOUT_NOTIFICATION_SHOWN)
		resetNotificationShown();

	#ifdef ENABLE_BATTERY_CAPACITY_CHECK
		checkBatteryCapacity();
	#endif
	#ifdef SSD13006_ENABLE_START_ANIMATION
		checkStartAnimation();
	#endif
}

void checkAllErrors(){
	if(error.stateMachine || error.motorMaxRunningTime || error.safetyTest || error.timerTimes || error.displayInit || error.emptyBattery){
		nextState(STATE_ERROR);
		// Show error on the display
		showNotificationWindow();
	}
}

void checkForWarnings(){
	if(warning.lowBattery || warning.noBatteryData || warning.startAnimation || warning.timerTimes){
		warning.overallWarning = true;
		// Show warning on the display
		showNotificationWindow();
	} else
		warning.overallWarning = false;
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

	if(tmpBool && !error.timerTimes)
		warning.timerTimes = true;
	else if(error.timerTimes)
		warning.timerTimes = false;
}

void checkDisplayInit(){
	if(error_delay(&errDisplayInit, SSD1306.Initialized == 0, TIMEOUT_ERROR_DISPLAY_INIT))
		error.displayInit = true;
}

void checkBatteryCapacity(){
	// <5% value means the ADC cannot work properly
	// In this case the flap should not switch into the error state
	if(error_delay(&warnNoBatteryData, bms.batteryCapapcityPercentage < 5, TIMEOUT_WARN_NO_BATTERY_DATA)) {
		warning.noBatteryData = true;
		return;
	}

	// Set low battery warning. Set to false if the error is setting true
	if(error_delay(&warnBatteryLow, bms.batteryCapapcityPercentage < WARNING_BATTERY_LOW_PERCENTAGE && !error.emptyBattery, TIMEOUT_WARN_BATTERY_LOW))
		warning.lowBattery = true;
	else if(error.emptyBattery)
		warning.lowBattery = false;

	// Set empty battery error
	if(error_delay(&errBatteryEmpty, bms.batteryCapapcityPercentage < ERROR_BATTERY_EMPTY_PERCENTAGE, TIMEOUT_ERROR_BATTERY_EMPTY))
		error.emptyBattery = true;
}

void checkStartAnimation(){
	if(warning.startAnimation)
		// Stop start animation in error case
		startAnimation.enable = false;
}

void showNotificationWindow(){
	bool stop;
	// Stop if window is active
	if(display.notificationWindowActive)
		return;

	// Stop if notification was already shown
	stop = true;
	if(!error.stateMachine || (error.stateMachine && error.notifications[ERROR_STATE_MACHINE])) stop &= true;
	else stop = false;
	if(!error.motorMaxRunningTime || (error.motorMaxRunningTime && error.notifications[ERROR_MOTOR_MAX_RUNNING_TIME])) stop &= true;
	else stop = false;
	if(!error.safetyTest || (error.safetyTest && error.notifications[ERROR_SAFETY_TEST])) stop &= true;
	else stop = false;
	if(!error.timerTimes || (error.timerTimes && error.notifications[ERROR_TIMER_TIMES])) stop &= true;
	else stop = false;
	if(!error.displayInit || (error.displayInit && error.notifications[ERROR_DISPLAY_INIT])) stop &= true;
	else stop = false;
	if(!error.emptyBattery || (error.emptyBattery && error.notifications[ERROR_EMPTY_BATTERY])) stop &= true;
	else stop = false;
	if(!warning.lowBattery || (warning.lowBattery && error.notifications[WARNING_LOW_BATTERY])) stop &= true;
	else stop = false;
	if(!warning.noBatteryData || (warning.noBatteryData && error.notifications[WARNING_NO_BATTERY_DATA])) stop &= true;
	else stop = false;
	if(!warning.startAnimation || (warning.startAnimation && error.notifications[WARNING_START_ANIMATION])) stop &= true;
	else stop = false;
	if(!warning.timerTimes || (warning.timerTimes && error.notifications[WARNING_TIMER_TIMES])) stop &= true;
	else stop = false;
	if(stop)
		return;

	display.notificationWindowActive = true;

	// Decide between warning or error notification
	bool errorNotific = false; // If false, its only a warning. True means that an error occured.
	if(stateMachine.state == STATE_ERROR)
		errorNotific = true;

	// Reset display
	ssd1306_Fill(Black);

	// Set headline and description (error or warning)
	char headline[] = "";
	if(errorNotific)
		strcat(headline, "Error");
	else
		strcat(headline, "Warning");

	// Create error / warining description
	setErrorDescription();

	// Draw window structure and write the headline
	ssd1306_DrawRectangle(1, 1, SSD1306_WIDTH-1, SSD1306_HEIGHT-1, White);
	ssd1306_Line(1,15,SSD1306_WIDTH-1,15, White);
	ssd1306_SetCursor(4,4);
	ssd1306_WriteString(headline, Font_6x8, White );

	// Split the string if its to long for one line.
	char *splitedStr;
	// Split string if its to long for one line
	if(strlen(error.errorDescription)*6 > (SSD1306_WIDTH)-8){
		char delimiter[] = "/";
		splitedStr = strtok(error.errorDescription,delimiter);

		uint8_t loopRound = 0;
		while (splitedStr != NULL && SSD1306_HEIGHT-(loopRound*8)-2 > 0){
			ssd1306_SetCursor(4,19+(8*loopRound));
			ssd1306_WriteString(splitedStr, Font_6x8, White);
			loopRound++;
			splitedStr = strtok(NULL,delimiter);
		}
	} else {
		ssd1306_SetCursor(4,19);
		ssd1306_WriteString(error.errorDescription, Font_6x8, White);
	}

	// Update display
	ssd1306_UpdateScreen();

	// Set notification as shown
	setNotificationShown();
}

void setErrorDescription(){
	// Reset string
	strcpy(error.errorDescription, "");

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// One line is separated by '/'
	// A maximum of 5 lines can be displayed,
	// anything in excess will be ignored.
	// A maximum of 20 characters can be displayed per line.
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	#ifdef ENABLE_GERMAN_LANGUAGE
		if(error.stateMachine)
			strcat(error.errorDescription, "StateMachine:/Unerwarteter/Zustand.");
		else if(error.motorMaxRunningTime)
			strcat(error.errorDescription, "Motor stoppt nicht.");
		else if(error.safetyTest)
			strcat(error.errorDescription, "Sofware Fehler/erkannt. Beide/MOSFET`s sind/gleichzeitig aktiv");
		else if(error.timerTimes)
			strcat(error.errorDescription, "Timer oeffnen und/schliessen Zeit/duerfen nicht/gleich sein./Neustart notwendig.");
		else if(error.displayInit)
			strcat(error.errorDescription, "Display antwortet/nicht.");
		else if(error.emptyBattery)
			strcat(error.errorDescription, "Batterie ist leer./Klappe deaktiviert./Batterien wechseln!");
		else if(warning.startAnimation)
			strcat(error.errorDescription, "Startanimation/hat einen/Fehler");
		else if(warning.lowBattery)
			strcat(error.errorDescription, "Batterie ist/fast leer./Laden oder neue/Batterien einlegen.");
		else if(warning.noBatteryData)
			strcat(error.errorDescription, "Batteriestand/nicht lesbar");
		else if(warning.timerTimes){
			char strTimeout[2] = "";
			sprintf(strTimeout, "%d",(TIMEOUT_ERROR_TIMER_TIMES/60000));
			char strWarn[255] = "Timer oeffnen und/schliessen Zeit/duerfen nicht/gleich sein. Error/in: ";
			strcat(strWarn, strTimeout);
			strcat(strWarn, " Minuten");
			strcat(error.errorDescription, strWarn);
		}
	#endif

	#ifdef ENABLE_ENGLISH_LANGUAGE
		if(error.stateMachine)
			strcat(error.errorDescription, "Statemachine error:/Unexpected state.");
		else if(error.motorMaxRunningTime)
			strcat(error.errorDescription, "Motor running time/timed out. Motor/never stops");
		else if(error.safetyTest)
			strcat(error.errorDescription, "Sofware error/detected./Both MOSFET´s were/switched on at the/same time.");
		else if(error.timerTimes)
			strcat(error.errorDescription, "Timer Open and/closing time must/not be the same/Flap is deactivated./Restart the flap.");
		else if(error.displayInit)
			strcat(error.errorDescription, "Hardware error/detected./Display does not/initialize.");
		else if(error.emptyBattery)
			strcat(error.errorDescription, "Battery is empty./Flap is deactivated./Change batteries!");
		else if(warning.startAnimation)
			strcat(error.errorDescription, "Startanimation/failed.");
		else if(warning.lowBattery)
			strcat(error.errorDescription, "Battery is low./Reload or switch/the batteries.");
		else if(warning.noBatteryData)
			strcat(error.errorDescription, "Battery data/are not available");
		else if(warning.timerTimes){
			char strTimeout[2] = "";
			sprintf(strTimeout, "%d",(TIMEOUT_ERROR_TIMER_TIMES/60000));
			char strWarn[255] = "Timer Open and/closing time must/not be the same./Error occours/in: ";
			strcat(strWarn, strTimeout);
			strcat(strWarn, " minutes");
			strcat(error.errorDescription, strWarn);
		}
	#endif
}

void setNotificationShown(){
	if(error.stateMachine && !error.notifications[ERROR_STATE_MACHINE]) 					error.notifications[ERROR_STATE_MACHINE] = true;
	else if(error.motorMaxRunningTime && !error.notifications[ERROR_MOTOR_MAX_RUNNING_TIME])error.notifications[ERROR_MOTOR_MAX_RUNNING_TIME] = true;
	else if(error.safetyTest && !error.notifications[ERROR_SAFETY_TEST])					error.notifications[ERROR_SAFETY_TEST] = true;
	else if(error.timerTimes && !error.notifications[ERROR_TIMER_TIMES]) 					error.notifications[ERROR_TIMER_TIMES] = true;
	else if(error.displayInit && !error.notifications[ERROR_DISPLAY_INIT]) 					error.notifications[ERROR_DISPLAY_INIT] = true;
	else if(error.emptyBattery && !error.notifications[ERROR_EMPTY_BATTERY]) 				error.notifications[ERROR_EMPTY_BATTERY] = true;
	else if(warning.lowBattery && !error.notifications[WARNING_LOW_BATTERY]) 				error.notifications[WARNING_LOW_BATTERY] = true;
	else if(warning.noBatteryData && !error.notifications[WARNING_NO_BATTERY_DATA])			error.notifications[WARNING_NO_BATTERY_DATA] = true;
	else if(warning.startAnimation && !error.notifications[WARNING_START_ANIMATION]) 		error.notifications[WARNING_START_ANIMATION] = true;
	else error.timeNotificationShown = 0;

	for(uint8_t i; i<sizeof(error.notifications)/sizeof(error.notifications[0]); i++){
		if(error.notifications[i] && error.timeNotificationShown == 0)
			error.timeNotificationShown = millis();
	}
}

void resetNotificationShown(){
	for(uint8_t i=0; i < sizeof(error.notifications)/sizeof(error.notifications[0]); i++)
		error.notifications[i] = false;
}

void errorTest(){
	// Display init error
	if(millis() > 5000 && millis() < 15000)
		error.displayInit = 1;
	else if (millis() > 16000)
		error.displayInit = 0;

	// Empty battery error
	if(millis() > 20000 && millis() < 30000)
		error.emptyBattery = 1;
	else if (millis() > 31000)
		error.emptyBattery = 0;

	// Motor max runnung time error
	if(millis() > 35000 && millis() < 45000)
		error.motorMaxRunningTime = 1;
	else if (millis() > 46000)
		error.motorMaxRunningTime = 0;

	// Safety test error
	if(millis() > 50000 && millis() < 60000)
		error.safetyTest = 1;
	else if (millis() > 61000)
		error.safetyTest = 0;

	// Statemachine error
	if(millis() > 75000 && millis() < 85000)
		error.stateMachine = 1;
	else if (millis() > 86000)
		error.stateMachine = 0;

	// Timer times error
	if(millis() > 70000 && millis() < 80000)
		error.timerTimes = 1;
	else if (millis() > 81000)
		error.timerTimes = 0;

	// Low battery warning
	if(millis() > 105000 && millis() < 115000)
		warning.lowBattery = 1;
	else if (millis() > 116000)
		warning.lowBattery = 0;


	// No battery data warning
	if(millis() > 120000 && millis() < 130000)
		warning.noBatteryData = 1;
	else if (millis() > 131000)
		warning.noBatteryData = 0;


	// Start animation warning
	if(millis() > 135000 && millis() < 145000)
		warning.startAnimation = 1;
	else if (millis() > 146000)
		warning.startAnimation = 0;


	// Timer times warning
	if(millis() > 150000 && millis() < 160000)
		warning.timerTimes = 1;
	else if (millis() > 161000)
		warning.timerTimes = 0;


	// 3 Errors to the same time
	if(millis() > 170000 && millis() < 190000){
		error.displayInit = 1;
		error.emptyBattery = 1;
		error.stateMachine = 1;
	}else if (millis() > 191000){
		error.displayInit = 0;
		error.emptyBattery = 0;
		error.stateMachine = 0;
	}
}
