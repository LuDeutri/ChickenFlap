#include "display.h"

void display_init(){
	digitalWrite(DISPLAY_ENABLE, HIGH);
	display.displayPage = DISPLAY_HOME;
	display.menuSelect = MENU_SELECT_WATCH;
	display.timerSelect = TIMER_SELECT_NONE;
	display.timerTimeSelect = TIMER_TIME_SELECT_NONE;
	display.watchSelect = WATCH_SELECT_NONE;
	display.configSelect = CONFIG_SELECT_IN_USE;
	display.adjustTimeSelect = ADJUST_TIME_SELECT_NONE;
	display.notificationWindowActive = false;
	ssd1306_Init();
}

void display_update(){
	// Stop if the display is disabled
	if (SSD1306.DisplayOn == DISPLAY_OFF)
		return;

	// Stop if an error or warning window is shown and wait until one button is pushed to continue with normal display operation
	if(display.notificationWindowActive){
		closeNotificationWindow();
		return;
	}

	displayNavigation();
	displayStateMachine();
}

void displayStateMachine() {
	// Reset diplay
	ssd1306_Fill(Black);

	//---------------- Information line at the top START----------------
	// Timer status ( ON / OFF / ONLY OPENING / ONLY CLOSING)

	char strTimer[20] = "Timer:";

	switch(timer.timerState){
	case TIMER_ACTIVE:
		#ifdef ENABLE_GERMAN_LANGUAGE
			strcat(strTimer, "AN");
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			strcat(strTimer, "ON");
		#endif
		break;
	case TIMER_DEACTIVE:
		#ifdef ENABLE_GERMAN_LANGUAGE
			strcat(strTimer, "AUS");
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			strcat(strTimer, "OFF");
		#endif
		break;
	case TIMER_ONLY_OPEN:
		#ifdef ENABLE_GERMAN_LANGUAGE
			strcat(strTimer, "OEFFNEN");
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			strcat(strTimer, "OPENING");
		#endif
		break;
	case TIMER_ONLY_CLOSE:
		#ifdef ENABLE_GERMAN_LANGUAGE
			strcat(strTimer, "SCHLIESSEN");
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			strcat(strTimer, "CLOSING");
		#endif
		break;
	default:
		strcat(strTimer, "---");
		break;
	}
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString(strTimer, Font_6x8, White);

	#ifdef ENABLE_BATTERY_LOAD_STATUS
		// Battery capacity
		char strBatValue[10] = "";
		sprintf(strBatValue,"%d", bms.batteryCapapcityPercentage);
		strcat(strBatValue, "%");

		ssd1306_SetCursor(104,0);
		ssd1306_WriteString(strBatValue, Font_6x8, White);
	#endif

	char strTimerTime[20] = "";
	char strTimerOpenTimeHour[7] = "";
	sprintf(strTimerOpenTimeHour, "%d%d",
		timer.openFlapTime_Dec_H,
		timer.openFlapTime_One_H
	);

	char strTimerOpenTimeMinutes[7] = "";
	sprintf(strTimerOpenTimeMinutes, "%d%d",
		timer.openFlapTime_Dec_M,
		timer.openFlapTime_One_M
	);

	char strTimerCloseTimeHour[7] = "";
	sprintf(strTimerCloseTimeHour, "%d%d",
		timer.closeFlapTime_Dec_H,
		timer.closeFlapTime_One_H
	);

	char strTimerCloseTimeMinutes[7] = "";
	sprintf(strTimerCloseTimeMinutes, "%d%d",
		timer.closeFlapTime_Dec_M,
		timer.closeFlapTime_One_M
	);

	// If only opening function of the timer is active
	if(timer.timerState == TIMER_ONLY_OPEN){
		strcat(strTimerTime, "(");
		strcat(strTimerTime, strTimerOpenTimeHour);
		strcat(strTimerTime, ":");
		strcat(strTimerTime, strTimerOpenTimeMinutes);
		strcat(strTimerTime, ")");
	}

	// If opening and closing functions are active
	if(timer.timerState == TIMER_ACTIVE){
		strcat(strTimerTime, "(");
		strcat(strTimerTime, strTimerOpenTimeHour);
		strcat(strTimerTime, ":");
		strcat(strTimerTime, strTimerOpenTimeMinutes);
		strcat(strTimerTime, "|");
		strcat(strTimerTime, strTimerCloseTimeHour);
		strcat(strTimerTime, ":");
		strcat(strTimerTime, strTimerCloseTimeMinutes);
		strcat(strTimerTime, ")");
	}

	// If only closing function of the timer is active
	if(timer.timerState == TIMER_ONLY_CLOSE){
		strcat(strTimerTime,"(");
		strcat(strTimerTime, strTimerCloseTimeHour);
		strcat(strTimerTime, ":");
		strcat(strTimerTime, strTimerCloseTimeMinutes);
		strcat(strTimerTime, ")");
	}

	ssd1306_SetCursor(0,9);
	ssd1306_WriteString(strTimerTime, Font_6x8, White);

	// Create a blinking notification if an error or a warning is true
	if(stateMachine.state == STATE_ERROR || warning.overallWarning){
		char strFailure[10] = "";
		if(stateMachine.state == STATE_ERROR){
			#ifdef ENABLE_GERMAN_LANGUAGE
				strcat(strFailure, "FEHLER");
			#endif
			#ifdef ENABLE_ENGLISH_LANGUAGE
				strcat(strFailure, "ERROR");
			#endif
		} else if (warning.overallWarning) {
			#ifdef ENABLE_GERMAN_LANGUAGE
				strcat(strFailure, "WARNUNG");
			#endif
			#ifdef ENABLE_ENGLISH_LANGUAGE
				strcat(strFailure, "WARNING");
			#endif
		}

		// If the battery capacity is not shown, use top right of the display. Otherwise below there the capacity is shown
		if(millis() % 1000 < 700) {
			ssd1306_SetCursor(80,0);
			#ifdef ENABLE_BATTERY_LOAD_STATUS
			ssd1306_SetCursor(80,9);
			#endif
			ssd1306_WriteString(strFailure, Font_6x8, White);
		}
	}

	// Flap status (OPENED / CLOSED / OPENING / CLOSING)
	char strFlap[15] = "Flap: ";

	if (!flap.motorEnable)
		strcat(strFlap, "ERROR");
	else {
		switch(flap.actuallyStateFlap){
		case FLAP_OPENED:
			#ifdef ENABLE_GERMAN_LANGUAGE
				strcat(strFlap, "GEOEFFNET");
			#endif
			#ifdef ENABLE_ENGLISH_LANGUAGE
				strcat(strFlap, "OPENED");
			#endif
			break;
		case FLAP_OPENING:
			#ifdef ENABLE_GERMAN_LANGUAGE
				strcat(strFlap, "OEFFNET");
			#endif
			#ifdef ENABLE_ENGLISH_LANGUAGE
				strcat(strFlap, "OPENING");
			#endif
			break;
		case FLAP_CLOSING:
			#ifdef ENABLE_GERMAN_LANGUAGE
				strcat(strFlap, "SCHLIESSEND");
			#endif
			#ifdef ENABLE_ENGLISH_LANGUAGE
				strcat(strFlap, "CLOSING");
			#endif
			break;
		case FLAP_CLOSED:
			#ifdef ENABLE_GERMAN_LANGUAGE
				strcat(strFlap, "GESCHLOSSEN");
			#endif
			#ifdef ENABLE_ENGLISH_LANGUAGE
				strcat(strFlap, "CLOSED");
			#endif
			break;
		default:
			strcat(strFlap, "---");
			break;
		}
	}
	ssd1306_SetCursor(0,18);

	if(flap.motorIsRuning){
		if(millis() % 1000 > 500){ // Blinks if motor is running
			ssd1306_WriteString(strFlap, Font_6x8, White);
		}
	} else
		ssd1306_WriteString(strFlap, Font_6x8, White);

	// Show cut line
	ssd1306_Line(0,27,127,27,White);

	//---------------- Information line at the top END----------------


	// Variables used in the switch block
	char strWatchTime[20] = "";
	char strWatchTimeHours[7] ="";
	sprintf(strWatchTimeHours,"%d%d",
		watch.watchDecHour,
		watch.watchOneHour
	);

	char strWatchTimeMinutes[7] = "";
	sprintf(strWatchTimeMinutes,"%d%d",
		watch.watchDecMinute,
		watch.watchOneMinute
	);

	char strMotorOperationTime[20] = "";
	char strMotorRunningTimeSecond[5] = "";
	sprintf(strMotorRunningTimeSecond,"%d",
		(int)flap.motorOperationTime/1000
	);

	char strMotorRunningTimeTenth[5] = "";
	sprintf(strMotorRunningTimeTenth,"%d",
		(int)((flap.motorOperationTime/100) % 10)
	);

	// Add back and enter block at the bottom of the screen
	if(display.displayPage >= DISPLAY_MENU){
		addBlockDownLeft("Back");
		if (display.displayPage != DISPLAY_CONFIG_IN_USE)
		addBlockDownRight("Enter");
	}

	switch(display.displayPage) {
	case DISPLAY_HOME:

		strcat(strWatchTime, strWatchTimeHours);
		strcat(strWatchTime, ":");
		strcat(strWatchTime, strWatchTimeMinutes);

		ssd1306_SetCursor(0,35);
		ssd1306_WriteString(strWatchTime, Font_16x26, White);

		addBlockDownRight("Menu");
		break;
	case DISPLAY_MENU:
		// Mark actually chosen action
		if (display.menuSelect == MENU_SELECT_WATCH)
			ssd1306_DrawRectangle(4, 31, 40, 45, White);
		else if (display.menuSelect == MENU_SELECT_TIMER)
			ssd1306_DrawRectangle(43, 31, 80, 45, White);
		else if (display.menuSelect == MENU_SELECT_CONFIG)
			ssd1306_DrawRectangle(82, 31, 124, 45, White);

		// Watch block
		ssd1306_DrawRectangle(6, 33, 38, 43, White);
		ssd1306_SetCursor(8,35);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_SetCursor(11,35);
			ssd1306_WriteString("Uhr", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("Watch", Font_6x8, White);
		#endif

		// Timer block
		ssd1306_DrawRectangle(45, 33, 78, 43, White);
		ssd1306_SetCursor(47,35);
		ssd1306_WriteString("Timer", Font_6x8, White);

		// Config block
		ssd1306_DrawRectangle(84, 33, 122, 43, White);
		ssd1306_SetCursor(86,35);
		ssd1306_WriteString("Config", Font_6x8, White);
		break;
	case DISPLAY_WATCH:
		// Hour digits blink if ones are setting
		ssd1306_SetCursor(25,30);
		if(display.watchSelect == WATCH_SELECT_HOUR){
			if(millis() % 1000 > 300)
				ssd1306_WriteString(strWatchTimeHours, Font_11x18, White);
		} else
			ssd1306_WriteString(strWatchTimeHours, Font_11x18, White);

		ssd1306_SetCursor(55,28);
		ssd1306_WriteString(":", Font_11x18, White);

		ssd1306_SetCursor(73,30);
		if(display.watchSelect == WATCH_SELECT_MINUTE){
			if(millis() % 1000 > 300)
				ssd1306_WriteString(strWatchTimeMinutes, Font_11x18, White);
		} else
			ssd1306_WriteString(strWatchTimeMinutes, Font_11x18, White);
		break;
	case DISPLAY_TIMER:
		// Mark actually chosen action
		switch(display.timerSelect){
		case TIMER_SELECT_NONE:
			// Nothing marked
			break;
		case TIMER_SELECT_OPENING_ACTIVATE:
			ssd1306_DrawRectangle(36, 30, 51, 40, White);
			break;
		case TIMER_SELECT_OPENING_DEACTIVATE:
			ssd1306_DrawRectangle(53, 30, 73, 40, White);
			break;
		case TIMER_SELECT_OPENING_SET_TIME:
			ssd1306_DrawRectangle(75, 30, 126, 40, White);
			break;
		case TIMER_SELECT_CLOSING_ACTIVATE:
			ssd1306_DrawRectangle(36, 40, 51, 50, White);
			break;
		case TIMER_SELECT_CLOSING_DEACTIVATE:
			ssd1306_DrawRectangle(53, 40, 73, 50, White);
			break;
		case TIMER_SELECT_CLOSING_SET_TIME:
			ssd1306_DrawRectangle(75, 40, 126, 50, White);
			break;
		default:
			// Do nothing
			break;
		}
		//--------------------- OPENING Configurations ---------------------
		ssd1306_SetCursor(0,32);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_WriteString("Auf:", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("Open:", Font_6x8, White);
		#endif

		// Activate Block
		ssd1306_SetCursor(38,32);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_WriteString("AN", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("ON", Font_6x8, White);
		#endif

		// Dectivate Block
		ssd1306_SetCursor(55,32);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_WriteString("AUS", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("OFF", Font_6x8, White);
		#endif

		// Time set Block
		ssd1306_SetCursor(78,32);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_WriteString("Uhrzeit", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("Set Time", Font_6x8, White);
		#endif
		//--------------------- CLOSING Configurations ---------------------
		ssd1306_SetCursor(0,42);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_WriteString("Zu:", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("Close:", Font_6x8, White);
		#endif

		// Activate Block
		ssd1306_SetCursor(38,42);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_WriteString("AN", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("ON", Font_6x8, White);
		#endif

		// Dectivate Block
		ssd1306_SetCursor(55,42);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_WriteString("AUS", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("OFF", Font_6x8, White);
		#endif

		// Time set Block
		ssd1306_SetCursor(78,42);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_WriteString("Uhrzeit", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("Set Time", Font_6x8, White);
		#endif
		break;
	case DISPLAY_TIMER_CLOSING:
		ssd1306_SetCursor(25,30);

		if(display.timerTimeSelect == TIMER_TIME_SELECT_HOUR){
			if(millis() % 1000 > 300)
				ssd1306_WriteString(strTimerCloseTimeHour, Font_11x18, White);
		} else
			ssd1306_WriteString(strTimerCloseTimeHour, Font_11x18, White);

		ssd1306_SetCursor(55,28);
		ssd1306_WriteString(":", Font_11x18, White);

		ssd1306_SetCursor(73,30);
		if(display.timerTimeSelect == TIMER_TIME_SELECT_MINUTE){
			if(millis() % 1000 > 300)
				ssd1306_WriteString(strTimerCloseTimeMinutes, Font_11x18, White);
		} else
			ssd1306_WriteString(strTimerCloseTimeMinutes, Font_11x18, White);
		break;
	case DISPLAY_TIMER_OPENING:
		ssd1306_SetCursor(25,30);

		if(display.timerTimeSelect == TIMER_TIME_SELECT_HOUR){
			if(millis() % 1000 > 300)
				ssd1306_WriteString(strTimerOpenTimeHour, Font_11x18, White);
		} else
			ssd1306_WriteString(strTimerOpenTimeHour, Font_11x18, White);

		ssd1306_SetCursor(55,28);
		ssd1306_WriteString(":", Font_11x18, White);

		ssd1306_SetCursor(73,30);
		if(display.timerTimeSelect == TIMER_TIME_SELECT_MINUTE){
			if(millis() % 1000 > 300)
				ssd1306_WriteString(strTimerOpenTimeMinutes, Font_11x18, White);
		} else
			ssd1306_WriteString(strTimerOpenTimeMinutes, Font_11x18, White);
		break;
	case DISPLAY_CONFIG:
		// Mark actually chosen action
		if (display.configSelect == CONFIG_SELECT_IN_USE)
			ssd1306_DrawRectangle(8, 37, 64, 51, White);
		else if (display.configSelect == CONFIG_SELECT_ADJUST_TIME)
			ssd1306_DrawRectangle(72, 37, 121, 51, White);

		// Explanation
		ssd1306_SetCursor(25,29);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_WriteString("Set Motorzeit", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("Set motor time", Font_6x8, White);
		#endif

		// Set on run block
		ssd1306_DrawRectangle(10, 39, 62, 49, White);
		ssd1306_SetCursor(12,41);
		ssd1306_WriteString("SetOnRun", Font_6x8, White);

		// Set time block
		ssd1306_DrawRectangle(74, 39, 119, 49, White);
		ssd1306_SetCursor(76,41);
		ssd1306_WriteString("SetTime", Font_6x8, White);
		break;
	case DISPLAY_CONFIG_IN_USE:
		// Explanation
		ssd1306_SetCursor(5,29);
		#ifdef ENABLE_GERMAN_LANGUAGE
			ssd1306_WriteString("Use Taster FlapCTRL", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("Use button FlapCTRL", Font_6x8, White);
		#endif

		// Measured time
		strcat(strMotorOperationTime, strMotorRunningTimeSecond);
		strcat(strMotorOperationTime,".");
		strcat(strMotorOperationTime, strMotorRunningTimeTenth);
		#ifdef ENABLE_GERMAN_LANGUAGE
			strcat(strMotorOperationTime, " Sekunden");
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			strcat(strMotorOperationTime, " Seconds");
		#endif
		ssd1306_SetCursor(30,41);
		ssd1306_WriteString(strMotorOperationTime, Font_6x8, White);
		break;
	case DISPLAY_CONFIG_ADJUST_TIME:
		// Motor active time in seconds with one tenth
		strcat(strMotorOperationTime, strMotorRunningTimeSecond);
		strcat(strMotorOperationTime,".");
		strcat(strMotorOperationTime, strMotorRunningTimeTenth);
		ssd1306_SetCursor(27,30);

		if(display.adjustTimeSelect == ADJUST_TIME_SELECT_SECONDS){
			if(millis() % 1000 > 200)
				ssd1306_WriteString(strMotorOperationTime, Font_11x18, White);
		} else
			ssd1306_WriteString(strMotorOperationTime, Font_11x18, White);

		ssd1306_SetCursor(80,35);
		#ifdef ENABLE_GERMAN_LANGUAGE
		ssd1306_WriteString("Sekunden", Font_6x8, White);
		#endif
		#ifdef ENABLE_ENGLISH_LANGUAGE
			ssd1306_WriteString("Seconds", Font_6x8, White);
		#endif
		break;
	default:
		// Do nothing
		break;
	}
	// Update display
	ssd1306_UpdateScreen();
}

void displayNavigation(){
	// Stop if no button is pressed
	if (!button.buttonRight && !button.buttonLeft && !button.buttonMenuEnter && !button.buttonMenuBack && !button.buttonJoker && !button.buttonFlapCtrl){
		button.firstTimeButtonPressed = 0;
		return;
	}

	// Back Button control
	if (display.displayPage > DISPLAY_HOME && button.buttonMenuBack && button.onePingIfButtonPressed){
		switch(display.displayPage){
		case DISPLAY_MENU ... DISPLAY_WATCH:
			nextDisplayPage(display.displayPage-1);
			break;
		case DISPLAY_TIMER:
			nextDisplayPage(DISPLAY_MENU);
			break;
		case DISPLAY_TIMER_CLOSING ... DISPLAY_TIMER_OPENING:
			nextDisplayPage(DISPLAY_TIMER);
			break;
		case DISPLAY_CONFIG:
			nextDisplayPage(DISPLAY_MENU);
			break;
		case DISPLAY_CONFIG_IN_USE ... DISPLAY_CONFIG_ADJUST_TIME:
			nextDisplayPage(DISPLAY_CONFIG);
			break;
		default:
			break;
		}
	}

	switch(display.displayPage){
	case DISPLAY_HOME:
		if (button.buttonMenuEnter && button.onePingIfButtonPressed)
			nextDisplayPage(DISPLAY_MENU);
		if((stateMachine.state == STATE_ERROR || warning.overallWarning) && button.buttonRight && button.onePingIfButtonPressed)
			resetNotificationShown();
		break;
	case DISPLAY_MENU:
		// Increase Selection
		if (button.buttonRight && button.onePingIfButtonPressed) {

			// Check for validity
			if (display.menuSelect >= MENU_SELECT_BLOCKS_NUMBER)
				display.menuSelect = MENU_SELECT_BLOCKS_NUMBER;
			else
				// Increase if its all right
				display.menuSelect++;

		// Decrease Selection
		} else if (button.buttonLeft && button.onePingIfButtonPressed) {
			// Check for validity
			if (display.menuSelect <= 0)
				display.menuSelect = 0;
			else
				// Decrease if its all right
				display.menuSelect--;

		// Switch to chosen Display Page with Enter
		} else if (display.menuSelect == MENU_SELECT_WATCH && button.buttonMenuEnter && button.onePingIfButtonPressed)
			nextDisplayPage(DISPLAY_WATCH);
		else if (display.menuSelect == MENU_SELECT_TIMER && button.buttonMenuEnter && button.onePingIfButtonPressed)
			nextDisplayPage(DISPLAY_TIMER);
		else if (display.menuSelect == MENU_SELECT_CONFIG && button.buttonMenuEnter && button.onePingIfButtonPressed)
			nextDisplayPage(DISPLAY_CONFIG);
		break;
	case DISPLAY_WATCH:
		if(button.buttonMenuEnter && button.onePingIfButtonPressed){

			// With Enter set watch digits, start with hour digits
			if (display.watchSelect == WATCH_SELECT_NONE){
				display.watchSelect = WATCH_SELECT_HOUR;
			// Switch to minutes if hour is selected and Enter is pushed
			} else if (display.watchSelect == WATCH_SELECT_HOUR)
				display.watchSelect = WATCH_SELECT_MINUTE;
			// End set watch digits if minutes is selected and Enter is pushed
			else if (display.watchSelect == WATCH_SELECT_MINUTE){
				display.watchSelect = WATCH_SELECT_NONE;
				resetSecCounter();
			}
		}

		// Set watch digits
		// Set digits with the right and left button
		else if(display.watchSelect > WATCH_SELECT_NONE && (button.buttonRight || button.buttonLeft)){

			// Hour digits
			if (display.watchSelect == WATCH_SELECT_HOUR){

				// Fast rising/sinking of digits after x pressed seconds
				if (millis() > button.firstTimeButtonPressed + PRESSED_BUTTON_TIME_FOR_FAST_NUMBER_RISE) {
					if (millis() % 400 > 200) {

						// Increase if right button is pressed
						if (button.buttonRight)
							watch.watchOneHour++;

						// Decrease if left button is pressed
						else if(watch.watchOneHour != 0)
							watch.watchOneHour--;
						else if(watch.watchDecHour > 0){
							watch.watchDecHour--;
							watch.watchOneHour = 9;
						}
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else {

					// Increase if right button is pressed
					if (button.buttonRight)
						watch.watchOneHour++;

					// Decrease if left button is pressed
					else if(watch.watchOneHour != 0)
						watch.watchOneHour--;
					else if(watch.watchDecHour > 0){
						watch.watchDecHour--;
						watch.watchOneHour = 9;
					}
				}
				// Rise Dec digit if 10 ones are reached and set ones back to 0
				if (watch.watchOneHour >= 10) {
					watch.watchDecHour++;
					watch.watchOneHour = 0;
				}

				// Reset if dec: 2 and one: 4 is reached -->24:XX --> 00:XX
				if (watch.watchDecHour >= 2 && watch.watchOneHour >= 4) {
					watch.watchDecHour= 0;
					watch.watchOneHour = 0;
				}

			// Minute digits
			} else if (display.watchSelect == WATCH_SELECT_MINUTE){
				// Fast rising/sinking of digits after x pressed seconds
				if (millis() > button.firstTimeButtonPressed + PRESSED_BUTTON_TIME_FOR_FAST_NUMBER_RISE) {
					if (millis() % 100 > 50) {

						// Increase if right button is pressed
						if (button.buttonRight)
							watch.watchOneMinute++;

						// Decrease if left button is pressed
						else if(watch.watchOneMinute != 0)
							watch.watchOneMinute--;
						else if(watch.watchDecMinute > 0){
							watch.watchDecMinute--;
							watch.watchOneMinute = 9;
						}
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else {

					// Increase if right button is pressed
					if (button.buttonRight)
						watch.watchOneMinute++;

					// Decrease if left button is pressed
					else if(watch.watchOneMinute != 0)
						watch.watchOneMinute--;
					else if(watch.watchDecMinute > 0){
						watch.watchDecMinute--;
						watch.watchOneMinute = 9;
					}
				}
				// Rise Dec digit if 10 ones are reached and set ones back to 0
				if (watch.watchOneMinute >= 10) {
					watch.watchDecMinute++;
					watch.watchOneMinute = 0;
				}

				// Reset if dec: 6 and one: 0 is reached -->XX:60 --> XX:00
				if (watch.watchDecMinute >= 6 && watch.watchOneMinute >= 0) {
					watch.watchDecMinute= 0;
					watch.watchOneMinute = 0;
				}
			}
		}
		break;
	case DISPLAY_TIMER:
		// Increase Selection
		if (button.buttonRight && button.onePingIfButtonPressed) {

			// Check for validity
			if (display.timerSelect >= TIMER_SELECT_BLOCKS_NUMBER)
				display.timerSelect = TIMER_SELECT_NONE;
			else
				// Increase if its all right
				display.timerSelect++;

		// Decrease Selection
		} else if (button.buttonLeft && button.onePingIfButtonPressed) {
			// Check for validity
			if (display.timerSelect <= 0)
				display.timerSelect = 0;
			else
				// Decrease if its all right
				display.timerSelect--;
		}
		// Do chosen action
		if (!button.buttonMenuEnter || !button.onePingIfButtonPressed)
			break;

		switch(display.timerSelect){
		case TIMER_SELECT_NONE:
			nextDisplayPage(DISPLAY_MENU);
			break;
		case TIMER_SELECT_OPENING_ACTIVATE:
			switch(timer.timerState){
			case TIMER_ONLY_OPEN ... TIMER_ACTIVE:
				// Stay like before
				break;
			case TIMER_ONLY_CLOSE:
				nextTimerState(TIMER_ACTIVE);
				break;
			case TIMER_DEACTIVE:
				nextTimerState(TIMER_ONLY_OPEN);
				break;
			default:
				// Do nothing
				break;
			}
			break;
		case TIMER_SELECT_OPENING_DEACTIVATE:
			switch(timer.timerState){
			case TIMER_DEACTIVE ... TIMER_ONLY_CLOSE:
				// Stay like before
				break;
			case TIMER_ONLY_OPEN:
				nextTimerState(TIMER_DEACTIVE);
				break;
			case TIMER_ACTIVE:
				nextTimerState(TIMER_ONLY_CLOSE);
				break;
			default:
				// Do nothing
				break;
			}
			break;
		case TIMER_SELECT_OPENING_SET_TIME:
			nextDisplayPage(DISPLAY_TIMER_OPENING);
			break;
		case TIMER_SELECT_CLOSING_ACTIVATE:
			switch(timer.timerState){
			case TIMER_ONLY_CLOSE:
				// Stay like before
				break;
			case TIMER_ACTIVE:
				// Stay like before
				break;
			case TIMER_ONLY_OPEN:
				nextTimerState(TIMER_ACTIVE);
				break;
			case TIMER_DEACTIVE:
				nextTimerState(TIMER_ONLY_CLOSE);
				break;
			default:
				// Do nothing
				break;
			}
			break;
		case TIMER_SELECT_CLOSING_DEACTIVATE:
			switch(timer.timerState){
			case TIMER_DEACTIVE:
				// Stay like before
				break;
			case TIMER_ONLY_OPEN:
				// Stay like before
				break;
			case TIMER_ONLY_CLOSE:
				nextTimerState(TIMER_DEACTIVE);
				break;
			case TIMER_ACTIVE:
				nextTimerState(TIMER_ONLY_OPEN);
				break;
			default:
				// Do nothing
				break;
			}
			break;
		case TIMER_SELECT_CLOSING_SET_TIME:
			nextDisplayPage(DISPLAY_TIMER_CLOSING);
			break;
		default:
			// Do nothing
			break;
		}
		break;
	case DISPLAY_TIMER_CLOSING:
		if(button.buttonMenuEnter && button.onePingIfButtonPressed){

			// With Enter set timer digits, start with hour digits
			if (display.timerTimeSelect == TIMER_TIME_SELECT_NONE)
				display.timerTimeSelect = TIMER_TIME_SELECT_HOUR;
			// Switch to minutes if hour is selected and Enter is pushed
			else if (display.timerTimeSelect == TIMER_TIME_SELECT_HOUR)
				display.timerTimeSelect = TIMER_TIME_SELECT_MINUTE;
			// End set timer closing digits if minutes is selected and Enter is pushed
			else if (display.timerTimeSelect == TIMER_TIME_SELECT_MINUTE)
				display.timerTimeSelect = TIMER_TIME_SELECT_NONE;
		}

		// Set timer closing digits
		// Set digits with the right and left button
		else if(display.timerTimeSelect > TIMER_TIME_SELECT_NONE && (button.buttonRight || button.buttonLeft)){

			// Hour digits
			if (display.timerTimeSelect == TIMER_TIME_SELECT_HOUR){

				// Fast rising/sinking of digits after x pressed seconds
				if (millis() > button.firstTimeButtonPressed + PRESSED_BUTTON_TIME_FOR_FAST_NUMBER_RISE) {
					if (millis() % 400 > 200) {

						// Increase if right button is pressed
						if (button.buttonRight)
							timer.closeFlapTime_One_H++;

						// Decrease if left button is pressed
						else if(timer.closeFlapTime_One_H != 0)
							timer.closeFlapTime_One_H--;
						else if (timer.closeFlapTime_Dec_H > 0){
							timer.closeFlapTime_Dec_H--;
							timer.closeFlapTime_One_H = 9;
						}
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else {

					// Increase if right button is pressed
					if (button.buttonRight)
						timer.closeFlapTime_One_H++;

					// Decrease if left button is pressed
					else if(timer.closeFlapTime_One_H != 0)
						timer.closeFlapTime_One_H--;
					else if (timer.closeFlapTime_Dec_H > 0){
						timer.closeFlapTime_Dec_H--;
						timer.closeFlapTime_One_H = 9;
					}
				}
				// Rise Dec digit if 10 ones are reached and set ones back to 0
				if (timer.closeFlapTime_One_H >= 10) {
					timer.closeFlapTime_Dec_H++;
					timer.closeFlapTime_One_H = 0;
				}

				// Reset if dec: 2 and one: 4 is reached -->24:XX --> 00:XX
				if (timer.closeFlapTime_Dec_H >= 2 && timer.closeFlapTime_One_H >= 4) {
					timer.closeFlapTime_Dec_H = 0;
					timer.closeFlapTime_One_H = 0;
				}

			// Minute digits
			} else if (display.timerTimeSelect == TIMER_TIME_SELECT_MINUTE){
				// Fast rising/sinking of digits after x pressed seconds
				if (millis() > button.firstTimeButtonPressed + PRESSED_BUTTON_TIME_FOR_FAST_NUMBER_RISE) {
					if (millis() % 100 > 50) {

						// Increase if right button is pressed
						if (button.buttonRight)
							timer.closeFlapTime_One_M++;

						// Decrease if left button is pressed
						else if(timer.closeFlapTime_One_M != 0)
							timer.closeFlapTime_One_M--;
						else if (timer.closeFlapTime_Dec_M > 0){
							timer.closeFlapTime_Dec_M--;
							timer.closeFlapTime_One_M = 9;
						}
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else if (millis() % 1000 > 500) {

					// Increase if right button is pressed
					if (button.buttonRight)
						timer.closeFlapTime_One_M++;

					// Decrease if left button is pressed
					else if(timer.closeFlapTime_One_M != 0)
						timer.closeFlapTime_One_M--;
					else if (timer.closeFlapTime_Dec_M > 0){
						timer.closeFlapTime_Dec_M--;
						timer.closeFlapTime_One_M = 9;
					}
				}

				// Rise Dec digit if 10 ones are reached and set ones back to 0
				if (timer.closeFlapTime_One_M >= 10) {
					timer.closeFlapTime_Dec_M++;
					timer.closeFlapTime_One_M = 0;
				}

				// Reset if dec: 2 and one: 4 is reached -->XX:60 --> XX:00
				if (timer.closeFlapTime_Dec_M >= 6 && timer.closeFlapTime_One_M >= 0) {
					timer.closeFlapTime_Dec_M = 0;
					timer.closeFlapTime_One_M = 0;
				}
			}
		}
		break;
	case DISPLAY_TIMER_OPENING:
		if(button.buttonMenuEnter && button.onePingIfButtonPressed){

			// With Enter set timer digits, start with hour digits
			if (display.timerTimeSelect == TIMER_TIME_SELECT_NONE)
				display.timerTimeSelect = TIMER_TIME_SELECT_HOUR;
			// Switch to minutes if hour is selected and Enter is pushed
			else if (display.timerTimeSelect == TIMER_TIME_SELECT_HOUR)
				display.timerTimeSelect = TIMER_TIME_SELECT_MINUTE;
			// End set timer closing digits if minutes is selected and Enter is pushed
			else if (display.timerTimeSelect == TIMER_TIME_SELECT_MINUTE)
				display.timerTimeSelect = TIMER_TIME_SELECT_NONE;
		}

		// Set timer closing digits
		// Set digits with the right and left button
		else if(display.timerTimeSelect > TIMER_TIME_SELECT_NONE && (button.buttonRight || button.buttonLeft)){

			// Hour digits
			if (display.timerTimeSelect == TIMER_TIME_SELECT_HOUR){

				// Fast rising/sinking of digits after x pressed seconds
				if (millis() > button.firstTimeButtonPressed + PRESSED_BUTTON_TIME_FOR_FAST_NUMBER_RISE) {
					if (millis() % 400 > 200) {

						// Increase if right button is pressed
						if (button.buttonRight)
							timer.openFlapTime_One_H++;

						// Decrease if left button is pressed
						else if(timer.openFlapTime_One_H != 0)
							timer.openFlapTime_One_H--;
						else if (timer.openFlapTime_Dec_H > 0){
							timer.openFlapTime_Dec_H--;
							timer.openFlapTime_One_H = 9;
						}
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else {

					// Increase if right button is pressed
					if (button.buttonRight)
						timer.openFlapTime_One_H++;

					// Decrease if left button is pressed
					else if(timer.openFlapTime_One_H != 0)
						timer.openFlapTime_One_H--;
					else if (timer.openFlapTime_Dec_H > 0){
						timer.openFlapTime_Dec_H--;
						timer.openFlapTime_One_H = 9;
					}
				}
				// Rise Dec digit if 10 ones are reached and set ones back to 0
				if (timer.openFlapTime_One_H >= 10) {
					timer.openFlapTime_Dec_H++;
					timer.openFlapTime_One_H = 0;
				}

				// Reset if dec: 2 and one: 4 is reached -->24:XX --> 00:XX
				if (timer.openFlapTime_Dec_H >= 2 && timer.openFlapTime_One_H >= 4) {
					timer.openFlapTime_Dec_H = 0;
					timer.openFlapTime_One_H = 0;
				}

			// Minute digits
			} else if (display.timerTimeSelect == TIMER_TIME_SELECT_MINUTE){
				// Fast rising/sinking of digits after x pressed seconds
				if (millis() > button.firstTimeButtonPressed + PRESSED_BUTTON_TIME_FOR_FAST_NUMBER_RISE) {
					if (millis() % 100 > 50) {

						// Increase if right button is pressed
						if (button.buttonRight)
							timer.openFlapTime_One_M++;

						// Decrease if left button is pressed
						else if(timer.openFlapTime_One_M != 0)
							timer.openFlapTime_One_M--;
						else if (timer.openFlapTime_Dec_M > 0){
							timer.openFlapTime_Dec_M--;
							timer.openFlapTime_One_M = 9;
						}
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else {

					// Increase if right button is pressed
					if (button.buttonRight)
						timer.openFlapTime_One_M++;

					// Decrease if left button is pressed
					else if(timer.openFlapTime_One_M != 0)
						timer.openFlapTime_One_M--;
					else if (timer.openFlapTime_Dec_M > 0){
						timer.openFlapTime_Dec_M--;
						timer.openFlapTime_One_M = 9;
					}
				}

				// Rise Dec digit if 10 ones are reached and set ones back to 0
				if (timer.openFlapTime_One_M >= 10) {
					timer.openFlapTime_Dec_M++;
					timer.openFlapTime_One_M = 0;
				}

				// Reset if dec: 2 and one: 4 is reached -->XX:60 --> XX:00
				if (timer.openFlapTime_Dec_M >= 6 && timer.openFlapTime_One_M >= 0) {
					timer.openFlapTime_Dec_M = 0;
					timer.openFlapTime_One_M = 0;
				}
			}
		}
		break;
	case DISPLAY_CONFIG:
		// Increase Selection
		if (button.buttonRight && button.onePingIfButtonPressed) {

			// Check for validity
			if (display.configSelect >= CONFIG_SELECT_BLOCKS_NUMBER)
				display.configSelect = CONFIG_SELECT_BLOCKS_NUMBER;
			else
				// Increase if its all right
				display.configSelect++;

		// Decrease Selection
		} else if (button.buttonLeft && button.onePingIfButtonPressed) {
			// Check for validity
			if (display.configSelect <= 0)
				display.configSelect = 0;
			else
				// Decrease if its all right
				display.configSelect--;

		// Switch to chosen Display Page with Enter
		} else if (button.buttonMenuEnter && button.onePingIfButtonPressed){

			if (display.configSelect == CONFIG_SELECT_IN_USE){
				nextDisplayPage(DISPLAY_CONFIG_IN_USE);
				flap.motorOperationTimeSetted = false;
			} else if (display.configSelect == CONFIG_SELECT_ADJUST_TIME && button.buttonMenuEnter && button.onePingIfButtonPressed)
				nextDisplayPage(DISPLAY_CONFIG_ADJUST_TIME);
		}
		break;
	case DISPLAY_CONFIG_IN_USE:
		// Nothing to do
		break;
	case DISPLAY_CONFIG_ADJUST_TIME:
		if(button.buttonMenuEnter && button.onePingIfButtonPressed){

			// With Enter set active motor duration time
			if (display.adjustTimeSelect == ADJUST_TIME_SELECT_NONE)
				display.adjustTimeSelect = ADJUST_TIME_SELECT_SECONDS;
			// End setting with pressing Enter again
			else if (display.adjustTimeSelect == ADJUST_TIME_SELECT_SECONDS)
				display.adjustTimeSelect = ADJUST_TIME_SELECT_NONE;
		}

		// Set time with the right and left button
		if(display.adjustTimeSelect == ADJUST_TIME_SELECT_SECONDS && (button.buttonRight || button.buttonLeft)){

			// Fast rising/sinking of digits after x pressed seconds
			if (millis() > button.firstTimeButtonPressed + PRESSED_BUTTON_TIME_FOR_FAST_NUMBER_RISE) {
				if (millis() % 100 > 50) {

					// Increase if right button is pressed
					if (button.buttonRight) {
						util_time_t tmpMotorTime = flap.motorOperationTime / 100;
						tmpMotorTime++;
						flap.motorOperationTime = tmpMotorTime*100;
					}
					// Decrease if left button is pressed
					else {
						util_time_t tmpMotorTime = flap.motorOperationTime / 100;
						tmpMotorTime--;
						flap.motorOperationTime = tmpMotorTime*100;
					}
				}

			// Normal rising/sinking of digits before x pressed seconds
			} else {

				// Increase if right button is pressed
				if (button.buttonRight){
					util_time_t tmpMotorTime = flap.motorOperationTime / 100;
					tmpMotorTime++;
					flap.motorOperationTime = tmpMotorTime*100;
				}
				// Decrease if left button is pressed
				else {
					util_time_t tmpMotorTime = flap.motorOperationTime / 100;
					tmpMotorTime--;
					flap.motorOperationTime = tmpMotorTime*100;
				}
			}
		}
		break;
	default:
		// Do nothing
		break;
	}
}

void nextDisplayPage(displayPage_t nextDisplayPage){
	if (display.displayPage == nextDisplayPage)
		return;

	display.displayPage = nextDisplayPage;
	display.menuSelect = 0;
	display.timerSelect = 0;
	display.configSelect = 0;
	display.adjustTimeSelect = 0;
}

void displayEnable(){
	if(ssd1306_GetDisplayOn() == DISPLAY_ON)
		return;

	ssd1306_SetDisplayOn(DISPLAY_ON);
}

void displayDisable(){
	if(ssd1306_GetDisplayOn() == DISPLAY_OFF)
		return;
	ssd1306_SetDisplayOn(DISPLAY_OFF);
}

void addBlockDownRight(char* string){
	ssd1306_DrawRectangle(95, 53, 127, 63, White);
	ssd1306_SetCursor(97,55);
	ssd1306_WriteString(string, Font_6x8, White);
}

void addBlockDownLeft(char* string){
	ssd1306_DrawRectangle(0, 53, 27, 63, White);
	ssd1306_SetCursor(3,55);
	ssd1306_WriteString(string, Font_6x8, White);
}

void closeNotificationWindow(){
	// Stop if no button is pressed
	if(!anyButtonPushed())
		return;

	setNotificationShown();
	display.notificationWindowActive = false;
}
