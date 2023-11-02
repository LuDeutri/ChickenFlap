#include "error.h"

error_t error;
warning_t warning;
err_delay_t errTimerTimes;
err_delay_t errDisplayInit;
err_delay_t warnNoBatteryData;
err_delay_t errBatteryEmpty;
err_delay_t warnBatteryLow;


void error_init(){
	error.stateMachine = false;
	error.motorMaxRunningTime = false;
	error.safetyTest = false;
	error.timerTimes = false;
	error.displayInit = false;
	error.emptyBattery = false;
	error.watchError = false;
	error.timeNotificationShown = 0;
	strcpy(error.errorDescription, "");

	warning.startAnimation = false;
	warning.lowBattery = false;
	warning.noBatteryData = false;
	warning.timerTimes = false;
	warning.overallWarning = false;
	warning.motorSpeedIsMax = false;

	warning.accepted.motorSpeedIsMax = false;

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
	checkMotorSpeed();
	checkBatterySOC();
	checkStartAnimation();

	// Reset notification if an error was shown
	if(millis() > error.timeNotificationShown + TIMEOUT_NOTIFICATION_SHOWN)
		resetNotificationShown();
}

void checkAllErrors(){
	if(error.stateMachine || error.motorMaxRunningTime || error.safetyTest || error.timerTimes || error.displayInit || error.emptyBattery || error.watchError){
		nextState(STATE_ERROR);
		// Show error on the display
		showNotificationWindow();
	}
}

void checkForWarnings(){
	if(warning.lowBattery || warning.noBatteryData || warning.startAnimation || warning.timerTimes || warning.motorSpeedIsMax){
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
					&& timer.closeFlapTime_hour == timer.openFlapTime_hour
					&& timer.closeFlapTime_minute == timer.openFlapTime_minute;

	if(error_delay(&errTimerTimes, tmpBool, TIMEOUT_ERROR_TIMER_TIMES))
		error.timerTimes = true;

	if(tmpBool && !error.timerTimes)
		warning.timerTimes = true;
	else if(!error.timerTimes) // delet warning if failure is fixed
		warning.timerTimes = false;
	else if(error.timerTimes)
		warning.timerTimes = false;
}

void checkDisplayInit(){
	if(error_delay(&errDisplayInit, SSD1306.Initialized == 0, TIMEOUT_ERROR_DISPLAY_INIT))
		error.displayInit = true;
}

void checkBatterySOC(){
	// If no batterie is used stop error checking
	#if !(defined(USE_BATTERY) && defined(ENABLE_BATTERY_ERR_CHECK))
		return;
	#endif

	// <5% value means the ADC cannot work properly
	// In this case the flap should not switch into the error state
	if(error_delay(&warnNoBatteryData, bms.soc < 5, TIMEOUT_WARN_NO_BATTERY_DATA))
		warning.noBatteryData = true;
	else {
		warning.noBatteryData = false;

		// Set empty battery error
		if(error_delay(&errBatteryEmpty, bms.soc < ERROR_BATTERY_EMPTY_PERCENTAGE , TIMEOUT_ERROR_BATTERY_EMPTY))
			error.emptyBattery = true;
	}

	// Set low battery warning. Set to false if the error is setting true
	if(error_delay(&warnBatteryLow, bms.soc < WARNING_BATTERY_LOW_PERCENTAGE && !error.emptyBattery, TIMEOUT_WARN_BATTERY_LOW))
		warning.lowBattery = true;
	else if(error.emptyBattery)
		warning.lowBattery = false;
}

void checkStartAnimation(){
	#ifndef SSD13006_ENABLE_START_ANIMATION
		return;
	#endif

	if(warning.startAnimation)
		// Stop start animation in error case
		startAnimation.enable = false;
}

void checkMotorSpeed(){
	// If warning was already shown and accepted, ignore the warning
	if(warning.accepted.motorSpeedIsMax){
		warning.motorSpeedIsMax = false;
		return;
	} else if(warning.motorSpeedIsMax && !display.notificationWindowActive)
		warning.accepted.motorSpeedIsMax = true;

	// Set warning if speed is set to max value
	warning.motorSpeedIsMax = (flap.motorSpeed == 100);
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
	if(!error.watchError || (error.watchError && error.notifications[ERROR_RTC])) stop &= true;
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
	if(!warning.motorSpeedIsMax || (warning.motorSpeedIsMax && error.notifications[WARNING_MOTORSPEED_MAX])) stop &= true;
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
	char headline[8] = "";
	if(errorNotific)
		strcat(headline, "Error");
	else
		strcat(headline, "Warning");

	// Create error / warning description
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
	#ifdef USE_GERMAN_LANGUAGE
		if(error.stateMachine)
			strcat(error.errorDescription, "StateMachine:/Unerwarteter/Zustand.");
		else if(error.motorMaxRunningTime)
			strcat(error.errorDescription, "Motor stoppt nicht.");
		else if(error.safetyTest)
			strcat(error.errorDescription, "Sofware Fehler/erkannt. Beide/MOSFET`s sind/gleichzeitig aktiv");
		else if(error.timerTimes)
			strcat(error.errorDescription, "Timer oeffnen und/schliessen Zeit/duerfen nicht/gleich sein./Neustart notwendig.");
		else if(error.watchError)
			strcat(error.errorDescription, "Uhrzeit kalibrierung/nicht möglich.");
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
		else if(warning.motorSpeedIsMax)
			strcat(error.errorDescription, "Motorgeschwindigkeit/ist auf 100%/eingestellt");

	#else
		if(error.stateMachine)
			strcat(error.errorDescription, "Statemachine error:/Unexpected state.");
		else if(error.motorMaxRunningTime)
			strcat(error.errorDescription, "Motor running time/timed out. Motor/never stops");
		else if(error.safetyTest)
			strcat(error.errorDescription, "Sofware error/detected./Both MOSFET´s were/switched on at the/same time.");
		else if(error.timerTimes)
			strcat(error.errorDescription, "Timer Open and/closing time must/not be the same/Flap is deactivated./Restart the flap.");
		else if(error.watchError)
			strcat(error.errorDescription, "Watch not working.");
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
			char strTimeout[3] = "";
			sprintf(strTimeout, "%d",(TIMEOUT_ERROR_TIMER_TIMES/60000));
			char strWarn[255] = "Timer Open and/closing time must/not be the same./Error occours/in: ";
			strcat(strWarn, strTimeout);
			strcat(strWarn, " minutes");
			strcat(error.errorDescription, strWarn);
		}
		else if(warning.motorSpeedIsMax)
			strcat(error.errorDescription, "Motorspeed is/adjusted to/maximum");
	#endif
}

void setNotificationShown(){
	if(error.stateMachine && !error.notifications[ERROR_STATE_MACHINE]) 					error.notifications[ERROR_STATE_MACHINE] = true;
	else if(error.motorMaxRunningTime && !error.notifications[ERROR_MOTOR_MAX_RUNNING_TIME])error.notifications[ERROR_MOTOR_MAX_RUNNING_TIME] = true;
	else if(error.safetyTest && !error.notifications[ERROR_SAFETY_TEST])					error.notifications[ERROR_SAFETY_TEST] = true;
	else if(error.timerTimes && !error.notifications[ERROR_TIMER_TIMES]) 					error.notifications[ERROR_TIMER_TIMES] = true;
	else if(error.watchError && !error.notifications[ERROR_RTC])						error.notifications[ERROR_RTC] = true;
	else if(error.displayInit && !error.notifications[ERROR_DISPLAY_INIT]) 					error.notifications[ERROR_DISPLAY_INIT] = true;
	else if(error.emptyBattery && !error.notifications[ERROR_EMPTY_BATTERY]) 				error.notifications[ERROR_EMPTY_BATTERY] = true;
	else if(warning.lowBattery && !error.notifications[WARNING_LOW_BATTERY]) 				error.notifications[WARNING_LOW_BATTERY] = true;
	else if(warning.noBatteryData && !error.notifications[WARNING_NO_BATTERY_DATA])			error.notifications[WARNING_NO_BATTERY_DATA] = true;
	else if(warning.startAnimation && !error.notifications[WARNING_START_ANIMATION]) 		error.notifications[WARNING_START_ANIMATION] = true;
	else if(warning.timerTimes && !error.notifications[WARNING_TIMER_TIMES])				error.notifications[WARNING_TIMER_TIMES] = true;
	else if(warning.motorSpeedIsMax && !error.notifications[WARNING_MOTORSPEED_MAX])		error.notifications[WARNING_MOTORSPEED_MAX] = true;
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
	else if (millis() > 16000){
		error.displayInit = 0;
		nextState(STATE_FLAP_CLOSE);
	}

	// Empty battery error
	if(millis() > 20000 && millis() < 30000)
		error.emptyBattery = 1;
	else if (millis() > 31000) {
		error.emptyBattery = 0;
		nextState(STATE_FLAP_CLOSE);
	}

	// Motor max runnung time error
	if(millis() > 35000 && millis() < 45000)
		error.motorMaxRunningTime = 1;
	else if (millis() > 46000){
		error.motorMaxRunningTime = 0;
		nextState(STATE_FLAP_CLOSE);
	}

	// Safety test error
	if(millis() > 50000 && millis() < 60000)
		error.safetyTest = 1;
	else if (millis() > 61000) {
		error.safetyTest = 0;
		nextState(STATE_FLAP_CLOSE);
	}

	// Statemachine error
	if(millis() > 65000 && millis() < 75000)
		error.stateMachine = 1;
	else if (millis() > 76000) {
		error.stateMachine = 0;
		nextState(STATE_FLAP_CLOSE);
	}

	// Timer times error
	if(millis() > 80000 && millis() < 90000)
		error.timerTimes = 1;
	else if (millis() > 91000) {
		error.timerTimes = 0;
		nextState(STATE_FLAP_CLOSE);
	}

	// Low battery warning
	if(millis() > 95000 && millis() < 105000)
		warning.lowBattery = 1;
	else if (millis() > 106000)
		warning.lowBattery = 0;


	// No battery data warning
	if(millis() > 110000 && millis() < 120000)
		warning.noBatteryData = 1;
	else if (millis() > 121000)
		warning.noBatteryData = 0;


	// Start animation warning
	if(millis() > 125000 && millis() < 135000)
		warning.startAnimation = 1;
	else if (millis() > 136000)
		warning.startAnimation = 0;


	// Timer times warning
	if(millis() > 140000 && millis() < 150000)
		warning.timerTimes = 1;
	else if (millis() > 151000)
		warning.timerTimes = 0;


	// 3 Errors at the same time
	if(millis() > 160000 && millis() < 180000){
		error.displayInit = 1;
		error.emptyBattery = 1;
		error.stateMachine = 1;
	}else if (millis() > 181000){
		error.displayInit = 0;
		error.emptyBattery = 0;
		error.stateMachine = 0;
		nextState(STATE_FLAP_CLOSE);
	}

	// Display RTC error
	if(millis() > 185000 && millis() < 195000)
		error.watchError = 1;
	else if (millis() > 196000) {
		error.watchError = 0;
		nextState(STATE_FLAP_CLOSE);
	}

	// Warning motor has max speed adjusted
	if(millis() > 200000 && millis() < 210000)
		warning.motorSpeedIsMax = 1;
	else if (millis() > 211000)
		warning.motorSpeedIsMax = 0;
}
