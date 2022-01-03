#include "display.h"

void display_init(){
	display.displayEnable = true;
	display.displayPage = DISPLAY_HOME;
	display.menuSelect = MENU_SELECT_WATCH;
	display.timerSelect = TIMER_SELECT_NONE;
	display.timerTimeSelect = TIMER_TIME_SELECT_NONE;
	display.watchSelect = WATCH_SELECT_NONE;
}

void display_update(){
	// Stop if the display is disabled or not ready yet
	if (!display.displayEnable || SSD1306_Init()<=0)
		return;

	displayNavigation();
	displayStateMachine();
}

// TODO Sämtliche Puts auf koordinaten untersuchen und anpassen damits angezeigt wird wies soll

void displayStateMachine() {
	// Clear the hole display
	SSD1306_Clear();

	//---------------- Information line at the top START----------------
	// Timer status ( ON / OFF / ONLY OPENING / ONLY CLOSING)
	char* tmpStrTimState = "Timer: ";

	switch(timer.timerState){
	case TIMER_ACTIVE:
		strcat(tmpStrTimState, "ON");
		break;
	case TIMER_DEACTIVE:
		strcat(tmpStrTimState, "OFF");
		break;
	case TIMER_ONLY_OPEN:
		strcat(tmpStrTimState, "ONLY OPENING");
		break;
	case TIMER_ONLY_CLOSE:
		strcat(tmpStrTimState, "ONLY CLOSING");
		break;
	default:
		strcat(tmpStrTimState, "---");
		break;
	}
	SSD1306_GotoXY(5,5);
	SSD1306_Puts(tmpStrTimState, &Font_7x10, 1);

	// Flap status (OPENED / CLOSED / OPENING / CLOSING)
	char* tmpStrFlapState = "Flap: ";

	switch(stateMachine.state){
	case STATE_FLAP_OPEN:
		if(flap.motorIsRuning)
			strcat(tmpStrFlapState, "OPENING");
		else
			strcat(tmpStrFlapState, "OPENED");
		break;
	case STATE_FLAP_CLOSE:
		if(flap.motorIsRuning)
			strcat(tmpStrFlapState, "CLOSING");
		else
			strcat(tmpStrFlapState, "CLOSED");
		break;
	default:
		strcat(tmpStrFlapState, "---");
		break;
	}
	if(flap.motorIsRuning && millis() % 1000 > 500){ // Blinks if motor is running
		SSD1306_GotoXY(5,10);
		SSD1306_Puts(tmpStrFlapState, &Font_7x10, 1);
	}

	// Battery capacity
	char* tmpStrBatCapacity = "";
	sprintf(tmpStrBatCapacity," %d", bms.batteryCapapcityPercentage);
	char* tmpStrPercent = " %";
	strcat(tmpStrBatCapacity, tmpStrPercent);

	SSD1306_GotoXY(100,5);
	SSD1306_Puts(tmpStrBatCapacity, &Font_7x10, 1);

	// Show cut line
	SSD1306_DrawLine(0,20,127,22,SSD1306_COLOR_WHITE);

	//---------------- Information line at the top END----------------

	//------------------- Timer Information START -------------------

	char* tmpStrFlapTime = "";

	// If opening function of the timer is active, show opening time
	if(timer.timerState == TIMER_ONLY_OPEN){
		char tmpStrFlapOpenTime[20];
		sprintf(tmpStrFlapOpenTime, "open: %d %d : %d %d",
			timer.openFlapTime_Dec_H,
			timer.openFlapTime_One_H,
			timer.openFlapTime_Dec_M,
			timer.openFlapTime_One_M
		);
		strcat(tmpStrFlapTime, tmpStrPercent);
	}

	// If opening and closing function are active, show cut line between them
	if(timer.timerState == TIMER_ACTIVE){
		char* tmpStrTimFlapActive = "|";
		strcat(tmpStrFlapTime, tmpStrTimFlapActive);
	}

	// If closing function of the timer is active, show closing time
	if(timer.timerState == TIMER_ONLY_CLOSE){
		char tmpStrTimFlapClose[20];
		sprintf(tmpStrTimFlapClose, "close: %d %d : %d %d",
			timer.closeFlapTime_Dec_H,
			timer.closeFlapTime_One_H,
			timer.closeFlapTime_Dec_M,
			timer.closeFlapTime_One_M
		);
		strcat(tmpStrFlapTime, tmpStrTimFlapClose);
	}

	SSD1306_GotoXY(5,10);
	SSD1306_Puts(tmpStrFlapTime, &Font_7x10, 1);

	//------------------- Timer Information END -------------------

	char tmpStrDisplayTimer[20];

	char tmpStrWatch[15];
	sprintf(tmpStrWatch,"%d %d  :  %d %d",
		watch.watchDecHour,
		watch.watchOneHour,
		watch.watchDecMinute,
		watch.watchOneMinute
	);

	char tmpStrOpeningTime[20];
	sprintf(tmpStrOpeningTime,"%d %d  :  %d %d",
		timer.openFlapTime_Dec_H,
		timer.openFlapTime_One_H,
		timer.openFlapTime_Dec_M,
		timer.openFlapTime_One_M
	);

	char tmpStrClosingTime[20];
	sprintf(tmpStrClosingTime,"%d %d  :  %d %d",
		timer.closeFlapTime_Dec_H,
		timer.closeFlapTime_One_H,
		timer.closeFlapTime_Dec_M,
		timer.closeFlapTime_One_M
	);

	// Display navigation blocks
	char* tmpStrMenuWatch = "Watch";
	char* tmpStrMenuTimer = "Timer";
	char* tmpStrMenuEnter = "Enter";
	char* tmpStrMenuBack = "Back";

	switch(DISPLAY_HOME) {
	case DISPLAY_HOME:
		//------------------------- Watch START -------------------------
		SSD1306_GotoXY(30,40);
		SSD1306_Puts(tmpStrWatch, &Font_7x10, 1);
		//------------------------- Watch END -------------------------

		//--------------------- Menu block START ---------------------
		SSD1306_GotoXY(100,52);
		SSD1306_Puts("Menu", &Font_7x10, 1);
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
		//---------------------- Menu block END ----------------------
		break;
	case DISPLAY_MENU:
		//---------------------- Menu view START ----------------------
		// Selection Rectangle
		if (display.menuSelect == MENU_SELECT_WATCH) {
			SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
			SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_BLACK);
		} else if (display.menuSelect == MENU_SELECT_TIMER) {
			SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
			SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_BLACK);
		}

		// Watch Block
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(30,30);
		SSD1306_Puts(tmpStrMenuWatch, &Font_7x10, 1);

		// Timer Block
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(80,30);
		SSD1306_Puts(tmpStrMenuTimer, &Font_7x10, 1);

		// Back Block
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(30,55);
		SSD1306_Puts(tmpStrMenuBack, &Font_7x10, 1);

		// Enter Block
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(80,55);
		SSD1306_Puts(tmpStrMenuEnter, &Font_7x10, 1);
		//----------------------- Menu view END -----------------------
		break;
	case DISPLAY_WATCH:
		if(millis() % 2000 > 1000 && display.watchSelect > WATCH_SELECT_NONE) // Blink if digits are setting
			break;

		SSD1306_GotoXY(30,40);
		SSD1306_Puts(tmpStrWatch, &Font_7x10, 1);

		// Enter Block
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(80,55);
		SSD1306_Puts(tmpStrMenuEnter, &Font_7x10, 1);

		break;
	case DISPLAY_TIMER:

		// Mark actually chosen action
		switch(display.timerSelect){
		case TIMER_SELECT_NONE:
			// Nothing marked
			break;
		case TIMER_SELECT_OPENING_ACTIVATE:
			SSD1306_DrawRectangle(8, 30, 15, 31, SSD1306_COLOR_WHITE);
			SSD1306_DrawRectangle(9, 29, 14, 30, SSD1306_COLOR_BLACK);
			break;
		case TIMER_SELECT_OPENING_DEACTIVATE:
			SSD1306_DrawRectangle(48, 30, 15, 32, SSD1306_COLOR_WHITE);
			SSD1306_DrawRectangle(49, 29, 14, 31, SSD1306_COLOR_BLACK);
			break;
		case TIMER_SELECT_OPENING_SET_TIME:
			SSD1306_DrawRectangle(88, 30, 15, 32, SSD1306_COLOR_WHITE);
			SSD1306_DrawRectangle(89, 29, 14, 31, SSD1306_COLOR_BLACK);
			break;
		case TIMER_SELECT_CLOSING_ACTIVATE:
			SSD1306_DrawRectangle(8, 40, 15, 31, SSD1306_COLOR_WHITE);
			SSD1306_DrawRectangle(9, 39, 14, 30, SSD1306_COLOR_BLACK);
			break;
		case TIMER_SELECT_CLOSING_DEACTIVATE:
			SSD1306_DrawRectangle(48, 40, 15, 32, SSD1306_COLOR_WHITE);
			SSD1306_DrawRectangle(49, 39, 14, 31, SSD1306_COLOR_BLACK);
			break;
		case TIMER_SELECT_CLOSING_SET_TIME:
			SSD1306_DrawRectangle(88, 40, 15, 32, SSD1306_COLOR_WHITE);
			SSD1306_DrawRectangle(89, 39, 14, 31, SSD1306_COLOR_BLACK);
			break;
		default:
			// Do nothing
			break;
		}

		//--------------------- OPENING Configurations ---------------------
		strcpy(tmpStrDisplayTimer, "Opening:");
		SSD1306_GotoXY(5,30);
		SSD1306_Puts(tmpStrDisplayTimer, &Font_7x10, 1);

		// Activate Block
		SSD1306_DrawRectangle(10, 30, 15, 30, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(11, 29, 14, 29, SSD1306_COLOR_BLACK);
		strcpy(tmpStrDisplayTimer, "Activate");
		SSD1306_GotoXY(13,35);
		SSD1306_Puts(tmpStrDisplayTimer, &Font_7x10, 1);

		// Dectivate Block
		SSD1306_DrawRectangle(50, 30, 15, 30, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(51, 29, 14, 29, SSD1306_COLOR_BLACK);
		strcpy(tmpStrDisplayTimer, "Deactivate");
		SSD1306_GotoXY(53,35);
		SSD1306_Puts(tmpStrDisplayTimer, &Font_7x10, 1);

		// Time set Block
		SSD1306_DrawRectangle(90, 30, 15, 30, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(91, 29, 14, 29, SSD1306_COLOR_BLACK);
		strcpy(tmpStrDisplayTimer, "Set Time");
		SSD1306_GotoXY(93,35);
		SSD1306_Puts(tmpStrDisplayTimer, &Font_7x10, 1);


		//--------------------- CLOSING Configurations ---------------------
		strcpy(tmpStrDisplayTimer, "Closing:");
		SSD1306_GotoXY(5,40);
		SSD1306_Puts(tmpStrDisplayTimer, &Font_7x10, 1);

		// Activate Block
		SSD1306_DrawRectangle(10, 40, 15, 30, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(11, 39, 14, 29, SSD1306_COLOR_BLACK);
		strcpy(tmpStrDisplayTimer, "Activate");
		SSD1306_GotoXY(13,35);
		SSD1306_Puts(tmpStrDisplayTimer, &Font_7x10, 1);

		// Dectivate Block
		SSD1306_DrawRectangle(50, 40, 15, 30, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(51, 39, 14, 29, SSD1306_COLOR_BLACK);
		strcpy(tmpStrDisplayTimer, "Deactivate");
		SSD1306_GotoXY(53,35);
		SSD1306_Puts(tmpStrDisplayTimer, &Font_7x10, 1);

		// Time set Block
		SSD1306_DrawRectangle(90, 40, 15, 30, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(91, 39, 14, 29, SSD1306_COLOR_BLACK);
		strcpy(tmpStrDisplayTimer, "Set Time");
		SSD1306_GotoXY(93,35);
		SSD1306_Puts(tmpStrDisplayTimer, &Font_7x10, 1);


		// Enter Block
		SSD1306_DrawRectangle(110, 55, 20, 20, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(111, 56, 19, 19, SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(115,60);
		SSD1306_Puts(tmpStrMenuEnter, &Font_7x10, 1);

		// Back Block
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(80,55);
		SSD1306_Puts(tmpStrMenuBack, &Font_7x10, 1);
		break;
	case DISPLAY_TIMER_OPENING:
		SSD1306_GotoXY(70,40);
		SSD1306_Puts(tmpStrOpeningTime, &Font_7x10, 1);
		break;
	case DISPLAY_TIMER_CLOSING:
		SSD1306_GotoXY(30,40);
		SSD1306_Puts(tmpStrClosingTime, &Font_7x10, 1);
		break;
	default:
		// Do nothing
		break;
	}
	// Sends new data to the display
	SSD1306_UpdateScreen();
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
