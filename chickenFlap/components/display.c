#include "display.h"

void display_init(){
	display.displayEnable = true;
	digitalWrite(DISPLAY_ENABLE, HIGH);
	display.displayPage = DISPLAY_HOME;
	display.menuSelect = MENU_SELECT_WATCH;
	display.timerSelect = TIMER_SELECT_NONE;
	display.timerTimeSelect = TIMER_TIME_SELECT_NONE;
	display.watchSelect = WATCH_SELECT_NONE;
	ssd1306_Init();
}

void display_update(){
	// Stop if the display is disabled or not ready yet
	if (!display.displayEnable || /*TODO SSD1306_Init()<=0*/ 0)
		return;

	displayNavigation();
	displayStateMachine();
}

// TODO Sämtliche Puts auf koordinaten untersuchen und anpassen damits angezeigt wird wies soll

void displayStateMachine() {
	// Display enable
	digitalWrite(DISPLAY_ENABLE, display.displayEnable);

	// Reset diplay
	ssd1306_Fill(Black);

	//---------------- Information line at the top START----------------
	// Timer status ( ON / OFF / ONLY OPENING / ONLY CLOSING)

	char strTimer[20] = "Timer:";

	switch(timer.timerState){
	case TIMER_ACTIVE:
		strcat(strTimer, "ON");
		break;
	case TIMER_DEACTIVE:
		strcat(strTimer, "OFF");
		break;
	case TIMER_ONLY_OPEN:
		strcat(strTimer, "ONLY OPENING");
		break;
	case TIMER_ONLY_CLOSE:
		strcat(strTimer, "ONLY CLOSING");
		break;
	default:
		strcat(strTimer, "---");
		break;
	}
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString(strTimer, Font_6x8, White);

	// Battery capacity
	char strBatValue[10] = "";
	bms.batteryCapapcityPercentage = 100; // TODO
	sprintf(strBatValue,"%d", bms.batteryCapapcityPercentage);
	strcat(strBatValue, "%");

	ssd1306_SetCursor(104,0);
	ssd1306_WriteString(strBatValue, Font_6x8, White);

	char strTimerTime[50] = "";

	// If only opening function of the timer is active
	if(timer.timerState == TIMER_ONLY_OPEN){
		strcat(strTimerTime, "(");
		getTimerOpeningTimeInString(strTimerTime);
		strcat(strTimerTime, ")");
	}

	// If opening and closing functions are active
	if(timer.timerState == TIMER_ACTIVE){
		strcat(strTimerTime, "(");
		getTimerOpeningTimeInString(strTimerTime);
		strcat(strTimerTime, "|");
		getTimerClosingTimeInString(strTimerTime);
		strcat(strTimerTime, ")");
	}

	// If only closing function of the timer is active
	if(timer.timerState == TIMER_ONLY_CLOSE){
		strcat(strTimerTime,"(");
		getTimerClosingTimeInString(strTimerTime);
		strcat(strTimerTime, ")");
	}

	ssd1306_SetCursor(0,9);
	ssd1306_WriteString(strTimerTime, Font_6x8, White);

	// Flap status (OPENED / CLOSED / OPENING / CLOSING)
	char strFlap[15] = "Flap :";

	switch(stateMachine.state){
	case STATE_FLAP_OPEN:
		if(flap.motorIsRuning)
			strcat(strFlap, "OPENING");
		else
			strcat(strFlap, "OPENED");
		break;
	case STATE_FLAP_CLOSE:
		if(flap.motorIsRuning)
			strcat(strFlap, "CLOSING");
		else
			strcat(strFlap, "CLOSED");
		break;
	default:
		strcat(strFlap, "---");
		break;
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

/*
	// Variables used in the switch block

	char tmpStrDisplayTimer[20];

	char strWatchTime[10] ="";
	sprintf(strWatchTime,"%d%d:%d%d",
		watch.watchDecHour,
		watch.watchOneHour,
		watch.watchDecMinute,
		watch.watchOneMinute
	);

	char strTimerOpenTime[5] = "";
	getTimerOpeningTimeInString(strTimerOpenTime);

	char strTimerClosingTime[5];
	getTimerClosingTimeInString(strTimerClosingTime);

	// Display navigation blocks
	char tmpStrMenuWatch[5] = "Watch";
	char tmpStrMenuTimer[5] = "Timer";
	char tmpStrMenuEnter[5] = "Enter";
	char tmpStrMenuBack[4] = "Back";

	switch(DISPLAY_HOME) {
	case DISPLAY_HOME:
		//------------------------- Watch START -------------------------
		ssd1306_SetCursor(30,30);
		ssd1306_WriteString(strWatchTime, Font_6x8, White);
		//------------------------- Watch END -------------------------

		//--------------------- Menu block START ---------------------
		ssd1306_DrawRectangle(101, 52, 127, 63, White);
		ssd1306_SetCursor(103,55);
		ssd1306_WriteString("Menu", Font_6x8, White);
		//---------------------- Menu block END ----------------------
		break;
	case DISPLAY_MENU:
		//---------------------- Menu view START ----------------------
		// Selection Rectangle
		if (display.menuSelect == MENU_SELECT_WATCH) {
			ssd1306_DrawRectangle(90, 50, 20, 20, White);
			ssd1306_DrawRectangle(90, 50, 20, 20, Black);
		} else if (display.menuSelect == MENU_SELECT_TIMER) {
			ssd1306_DrawRectangle(90, 50, 20, 20, White);
			ssd1306_DrawRectangle(90, 50, 20, 20, Black);
		}

		// Watch Block
		ssd1306_DrawRectangle(90, 50, 20, 20, White);
		ssd1306_DrawRectangle(90, 50, 20, 20, Black);
		ssd1306_SetCursor(30,30);
		ssd1306_WriteString(tmpStrMenuWatch, Font_6x8, 1);

		// Timer Block
		ssd1306_DrawRectangle(90, 50, 20, 20, White);
		ssd1306_DrawRectangle(90, 50, 20, 20, Black);
		ssd1306_SetCursor(80,30);
		ssd1306_WriteString(tmpStrMenuTimer, Font_6x8, 1);

		// Back Block
		ssd1306_DrawRectangle(90, 50, 20, 20, White);
		ssd1306_DrawRectangle(90, 50, 20, 20, Black);
		ssd1306_SetCursor(30,55);
		ssd1306_WriteString(tmpStrMenuBack, Font_6x8, 1);

		// Enter Block
		ssd1306_DrawRectangle(90, 50, 20, 20, White);
		ssd1306_DrawRectangle(90, 50, 20, 20, Black);
		ssd1306_SetCursor(80,55);
		ssd1306_WriteString(tmpStrMenuEnter, Font_6x8, 1);
		//----------------------- Menu view END -----------------------
		break;
	case DISPLAY_WATCH:
		if(millis() % 2000 > 1000 && display.watchSelect > WATCH_SELECT_NONE) // Blink if digits are setting
			break;

		ssd1306_SetCursor(30,40);
		ssd1306_WriteString(strWatchTime, Font_6x8, 1);

		// Enter Block
		ssd1306_DrawRectangle(90, 50, 20, 20, White);
		ssd1306_DrawRectangle(90, 50, 20, 20, Black);
		ssd1306_SetCursor(80,55);
		ssd1306_WriteString(tmpStrMenuEnter, Font_6x8, 1);

		break;
	case DISPLAY_TIMER:

		// Mark actually chosen action
		switch(display.timerSelect){
		case TIMER_SELECT_NONE:
			// Nothing marked
			break;
		case TIMER_SELECT_OPENING_ACTIVATE:
			ssd1306_DrawRectangle(8, 30, 15, 31, White);
			ssd1306_DrawRectangle(9, 29, 14, 30, Black);
			break;
		case TIMER_SELECT_OPENING_DEACTIVATE:
			ssd1306_DrawRectangle(48, 30, 15, 32, White);
			ssd1306_DrawRectangle(49, 29, 14, 31, Black);
			break;
		case TIMER_SELECT_OPENING_SET_TIME:
			ssd1306_DrawRectangle(88, 30, 15, 32, White);
			ssd1306_DrawRectangle(89, 29, 14, 31, Black);
			break;
		case TIMER_SELECT_CLOSING_ACTIVATE:
			ssd1306_DrawRectangle(8, 40, 15, 31, White);
			ssd1306_DrawRectangle(9, 39, 14, 30, Black);
			break;
		case TIMER_SELECT_CLOSING_DEACTIVATE:
			ssd1306_DrawRectangle(48, 40, 15, 32, White);
			ssd1306_DrawRectangle(49, 39, 14, 31, Black);
			break;
		case TIMER_SELECT_CLOSING_SET_TIME:
			ssd1306_DrawRectangle(88, 40, 15, 32, White);
			ssd1306_DrawRectangle(89, 39, 14, 31, Black);
			break;
		default:
			// Do nothing
			break;
		}
	}
/*
		//--------------------- OPENING Configurations ---------------------
		strcpy(tmpStrDisplayTimer, "Opening:");
		ssd1306_SetCursor(5,30);
		ssd1306_WriteString(tmpStrDisplayTimer, Font_6x8, 1);

		// Activate Block
		ssd1306_DrawRectangle(10, 30, 15, 30, White);
		ssd1306_DrawRectangle(11, 29, 14, 29, Black);
		strcpy(tmpStrDisplayTimer, "Activate");
		ssd1306_SetCursor(13,35);
		ssd1306_WriteString(tmpStrDisplayTimer, Font_6x8, 1);

		// Dectivate Block
		ssd1306_DrawRectangle(50, 30, 15, 30, White);
		ssd1306_DrawRectangle(51, 29, 14, 29, Black);
		strcpy(tmpStrDisplayTimer, "Deactivate");
		ssd1306_SetCursor(53,35);
		ssd1306_WriteString(tmpStrDisplayTimer, Font_6x8, 1);

		// Time set Block
		ssd1306_DrawRectangle(90, 30, 15, 30, White);
		ssd1306_DrawRectangle(91, 29, 14, 29, Black);
		strcpy(tmpStrDisplayTimer, "Set Time");
		ssd1306_SetCursor(93,35);
		ssd1306_WriteString(tmpStrDisplayTimer, Font_6x8, 1);


		//--------------------- CLOSING Configurations ---------------------
		strcpy(tmpStrDisplayTimer, "Closing:");
		ssd1306_SetCursor(5,40);
		ssd1306_WriteString(tmpStrDisplayTimer, Font_6x8, 1);

		// Activate Block
		ssd1306_DrawRectangle(10, 40, 15, 30, White);
		ssd1306_DrawRectangle(11, 39, 14, 29, Black);
		strcpy(tmpStrDisplayTimer, "Activate");
		ssd1306_SetCursor(13,35);
		ssd1306_WriteString(tmpStrDisplayTimer, Font_6x8, 1);

		// Dectivate Block
		ssd1306_DrawRectangle(50, 40, 15, 30, White);
		ssd1306_DrawRectangle(51, 39, 14, 29, Black);
		strcpy(tmpStrDisplayTimer, "Deactivate");
		ssd1306_SetCursor(53,35);
		ssd1306_WriteString(tmpStrDisplayTimer, Font_6x8, 1);

		// Time set Block
		ssd1306_DrawRectangle(90, 40, 15, 30, White);
		ssd1306_DrawRectangle(91, 39, 14, 29, Black);
		strcpy(tmpStrDisplayTimer, "Set Time");
		ssd1306_SetCursor(93,35);
		ssd1306_WriteString(tmpStrDisplayTimer, Font_6x8, 1);


		// Enter Block
		ssd1306_DrawRectangle(110, 55, 20, 20, White);
		ssd1306_DrawRectangle(111, 56, 19, 19, Black);
		ssd1306_SetCursor(115,60);
		ssd1306_WriteString(tmpStrMenuEnter, Font_6x8, 1);

		// Back Block
		ssd1306_DrawRectangle(90, 50, 20, 20, White);
		ssd1306_DrawRectangle(90, 50, 20, 20, Black);
		ssd1306_SetCursor(80,55);
		ssd1306_WriteString(tmpStrMenuBack, Font_6x8, 1);
		break;
	case DISPLAY_TIMER_OPENING:
		ssd1306_SetCursor(70,40);
		ssd1306_WriteString(strTimerOpenTime, Font_6x8, 1);
		break;
	case DISPLAY_TIMER_CLOSING:
		ssd1306_SetCursor(30,40);
		ssd1306_WriteString(strTimerClosingTime, Font_6x8, 1);
		break;
	default:
		// Do nothing
		break;
	}
*/
	// Sends new data to the display
	ssd1306_UpdateScreen();
}

void displayNavigation(){
	// Stop if no button is pressed
	if (!button.buttonRight && !button.buttonLeft && !button.buttonMenuEnter && !button.buttonMenuBack){
		button.firstTimeButtonPressed = 0;
		return;
	}

	switch(display.displayPage){
	case DISPLAY_HOME:
		if (button.buttonMenuEnter && button.onePingIfButtonPressed)
			nextDisplayPage(DISPLAY_MENU);
		break;
	case DISPLAY_MENU:
		// With Back back to DISPLAY_HOME
		if (button.buttonMenuBack && button.onePingIfButtonPressed){
			nextDisplayPage(DISPLAY_HOME);

		// Increase Selection
		} else if (button.buttonRight && button.onePingIfButtonPressed) {

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
		break;
	case DISPLAY_WATCH:
		// With Back back to DISPLAY_MENU
		if (button.buttonMenuBack && button.onePingIfButtonPressed) {
			nextDisplayPage(DISPLAY_MENU);

		// With Enter set watch digits, start with hour digits
		} else if (button.buttonMenuEnter && button.onePingIfButtonPressed && display.watchSelect == WATCH_SELECT_NONE){
			display.watchSelect = WATCH_SELECT_HOUR;
		// Switch to minutes if hour is selected and Enter is pushed
		} else if (button.buttonMenuEnter && button.onePingIfButtonPressed && display.watchSelect == WATCH_SELECT_HOUR)
			display.watchSelect = WATCH_SELECT_MINUTE;
		// End set watch digits if minutes is selected and Enter is pushed
		else if (button.buttonMenuEnter && button.onePingIfButtonPressed && display.watchSelect == WATCH_SELECT_MINUTE){
			display.watchSelect = WATCH_SELECT_NONE;
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
						else
							watch.watchOneHour--;
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else if (millis() % 1000 > 500) {

					// Increase if right button is pressed
					if (button.buttonRight)
						watch.watchOneHour++;

					// Decrease if left button is pressed
					else
						watch.watchOneHour--;
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
					if (millis() % 400 > 200) {

						// Increase if right button is pressed
						if (button.buttonRight)
							watch.watchOneMinute++;

						// Decrease if left button is pressed
						else
							watch.watchOneMinute--;
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else if (millis() % 1000 > 500) {

					// Increase if right button is pressed
					if (button.buttonRight)
						watch.watchOneMinute++;

					// Decrease if left button is pressed
					else
						watch.watchOneMinute--;
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
		// With Back or enter if no action is selected back to DISPLAY_MENU
		if (button.buttonMenuBack && button.onePingIfButtonPressed) {
			nextDisplayPage(DISPLAY_MENU);

		// Increase Selection
		} else if (button.buttonRight && button.onePingIfButtonPressed) {

			// Check for validity
			if (display.timerSelect >= TIMER_SELECT_BLOCKS_NUMBER)
				display.timerSelect = TIMER_SELECT_BLOCKS_NUMBER;
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
				display.menuSelect--;

		// Do chosen action
		} else if (display.timerSelect == TIMER_SELECT_NONE || !button.buttonMenuEnter || !button.onePingIfButtonPressed)
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
		// With Back back to DISPLAY_TIMER
		if (button.buttonMenuBack && button.onePingIfButtonPressed) {
			nextDisplayPage(DISPLAY_TIMER);

		// With Enter set timer digits, start with hour digits
		} else if (button.buttonMenuEnter && button.onePingIfButtonPressed && display.timerTimeSelect == TIMER_TIME_SELECT_NONE){
			display.timerTimeSelect = TIMER_TIME_SELECT_HOUR;
		// Switch to minutes if hour is selected and Enter is pushed
		} else if (button.buttonMenuEnter && button.onePingIfButtonPressed && display.timerTimeSelect == TIMER_TIME_SELECT_HOUR)
			display.timerTimeSelect = TIMER_TIME_SELECT_MINUTE;
		// End set timer closing digits if minutes is selected and Enter is pushed
		else if (button.buttonMenuEnter && button.onePingIfButtonPressed && display.timerTimeSelect == TIMER_TIME_SELECT_MINUTE){
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
						else
							timer.closeFlapTime_One_H--;
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else if (millis() % 1000 > 500) {

					// Increase if right button is pressed
					if (button.buttonRight)
						timer.closeFlapTime_One_H++;

					// Decrease if left button is pressed
					else
						timer.closeFlapTime_One_H--;
				}
				// Rise Dec digit if 10 ones are reached and set ones back to 0
				if (timer.closeFlapTime_One_H >= 10) {
					timer.openFlapTime_Dec_H++;
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
					if (millis() % 400 > 200) {

						// Increase if right button is pressed
						if (button.buttonRight)
							timer.closeFlapTime_One_M++;

						// Decrease if left button is pressed
						else
							timer.closeFlapTime_One_M--;
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else if (millis() % 1000 > 500) {

					// Increase if right button is pressed
					if (button.buttonRight)
						timer.closeFlapTime_One_M++;

					// Decrease if left button is pressed
					else
						timer.closeFlapTime_One_M--;
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
		// With Back back to DISPLAY_TIMER
		if (button.buttonMenuBack && button.onePingIfButtonPressed) {
			nextDisplayPage(DISPLAY_TIMER);

		// With Enter set timer digits, start with hour digits
		} else if (button.buttonMenuEnter && button.onePingIfButtonPressed && display.timerTimeSelect == TIMER_TIME_SELECT_NONE){
			display.timerTimeSelect = TIMER_TIME_SELECT_HOUR;
		// Switch to minutes if hour is selected and Enter is pushed
		} else if (button.buttonMenuEnter && button.onePingIfButtonPressed && display.timerTimeSelect == TIMER_TIME_SELECT_HOUR)
			display.timerTimeSelect = TIMER_TIME_SELECT_MINUTE;
		// End set timer closing digits if minutes is selected and Enter is pushed
		else if (button.buttonMenuEnter && button.onePingIfButtonPressed && display.timerTimeSelect == TIMER_TIME_SELECT_MINUTE){
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
						else
							timer.openFlapTime_One_H--;
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else if (millis() % 1000 > 500) {

					// Increase if right button is pressed
					if (button.buttonRight)
						timer.openFlapTime_One_H++;

					// Decrease if left button is pressed
					else
						timer.openFlapTime_One_H--;
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
					if (millis() % 400 > 200) {

						// Increase if right button is pressed
						if (button.buttonRight)
							timer.openFlapTime_One_M++;

						// Decrease if left button is pressed
						else
							timer.openFlapTime_One_M--;
					}

				// Normal rising/sinking of digits before x pressed seconds
				} else if (millis() % 1000 > 500) {

					// Increase if right button is pressed
					if (button.buttonRight)
						timer.openFlapTime_One_M++;

					// Decrease if left button is pressed
					else
						timer.openFlapTime_One_M--;
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
}

void displayEnable(){
	digitalWrite(DISPLAY_ENABLE, HIGH);
}

void displayDisable(){
	digitalWrite(DISPLAY_ENABLE, LOW);
}

void getTimerOpeningTimeInString(char* dest){
	char tmp[6];
	sprintf(tmp, "%d%d:%d%d",
		timer.openFlapTime_Dec_H,
		timer.openFlapTime_One_H,
		timer.openFlapTime_Dec_M,
		timer.openFlapTime_One_M
	);
	strcat(dest,tmp);
}

void getTimerClosingTimeInString(char* dest){
	char tmp[6];
	sprintf(tmp, "%d%d:%d%d",
		timer.closeFlapTime_Dec_H,
		timer.closeFlapTime_One_H,
		timer.closeFlapTime_Dec_M,
		timer.closeFlapTime_One_M
	);
	strcat(dest,tmp);
}
