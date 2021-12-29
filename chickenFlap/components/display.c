#include "display.h"

void display_init(){
	display.displayEnable = true;
	display.displayPage = DISPLAY_HOME;
	display.menuSelect = MENU_SELECT_WATCH;
	display.timerSelect = TIMER_SELECT_CLOSING;
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

void displayStateMachine() {
	// Clear the hole display
	SSD1306_Fill(SSD1306_COLOR_BLACK);

	//---------------- Information line at the top START----------------
	// Timer status ( ON / OFF)
	char* tmpStrTimState = "Timer: ";

	if (timer.timerOn)
		strcat(tmpStrTimState, "ON");
	else
		strcat(tmpStrTimState, "OFF");

	SSD1306_GotoXY(5,5);
	SSD1306_Puts(tmpStrTimState, &Font_7x10, 1);

	// Battery capacity
	char* tmpStrBatCapacity = "";
	sprintf(tmpStrBatCapacity," %d", bms.batteryCapapcityPercentage);
	char* tmpStrPercent = " %";
	strcat(tmpStrBatCapacity, tmpStrPercent);

	SSD1306_GotoXY(100,5);
	SSD1306_Puts(tmpStrBatCapacity, &Font_7x10, 1);

	// Show cut line
	SSD1306_DrawLine(0,20,127,22,SSD1306_COLOR_WHITE);


	// TODO darstellen ob Klappe gerade offen/geschlossen ist bzw. gerade öffnet/schließt
	// TODO Sämtliche Puts auf koordinaten untersuchen und anpassen damits angezeigt wird wies soll


	//---------------- Information line at the top END----------------

	//------------------- Timer Information START -------------------
	if(timer.timerOn){ // Show only if the timer is activated
		char tmpStrTimFlapOpen[40];
		sprintf(tmpStrTimFlapOpen, "open: %d %d : %d %d | close: %d %d : %d %d",
			timer.openFlapTime_Dec_H,
			timer.openFlapTime_One_H,
			timer.openFlapTime_Dec_M,
			timer.openFlapTime_One_M,
			timer.closeFlapTime_Dec_H,
			timer.closeFlapTime_One_H,
			timer.closeFlapTime_Dec_M,
			timer.closeFlapTime_One_M
		);
		SSD1306_GotoXY(5,25);
		SSD1306_Puts(tmpStrBatCapacity, &Font_7x10, 1);
	}
	//------------------- Timer Information END -------------------

	char tmpStrHomeWatch[15];
	char* tmpStrMenuWatch = "Watch";
	char* tmpStrMenuTimer = "Timer";
	char* tmpStrMenuEnter = "Enter";
	char* tmpStrMenuBack = "Back";
	char tmpStrWatchTime[15];
	char tmpStrTimerCloseTime[15];
	char tmpStrTimerOpenTime[15];
	switch(DISPLAY_HOME) {
	case DISPLAY_HOME:
		//------------------------- Watch START -------------------------
		sprintf(tmpStrHomeWatch,"%d %d  :  %d %d",
			watch.watchDecHour,
			watch.watchOneHour,
			watch.watchDecMinute,
			watch.watchOneMinute);

		SSD1306_GotoXY(30,40);
		SSD1306_Puts(tmpStrHomeWatch, &Font_7x10, 1);
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

		sprintf(tmpStrWatchTime,"%d %d  :  %d %d",
			watch.watchDecHour,
			watch.watchOneHour,
			watch.watchDecMinute,
			watch.watchOneMinute);

		SSD1306_GotoXY(30,40);
		SSD1306_Puts(tmpStrHomeWatch, &Font_7x10, 1);

		// Enter Block
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(80,55);
		SSD1306_Puts(tmpStrMenuEnter, &Font_7x10, 1);

		break;
	case DISPLAY_TIMER:
		if(millis() % 2000 > 1000) // Blinks
			// Show nothing
			break;

		// Close Time
		sprintf(tmpStrTimerCloseTime,"%d %d  :  %d %d",
			timer.closeFlapTime_Dec_H,
			timer.closeFlapTime_One_H,
			timer.closeFlapTime_Dec_M,
			timer.closeFlapTime_One_M
		);
		SSD1306_GotoXY(30,40);
		SSD1306_Puts(tmpStrHomeWatch, &Font_7x10, 1);

		// Open Time
		sprintf(tmpStrTimerOpenTime,"%d %d  :  %d %d",
			timer.openFlapTime_Dec_H,
			timer.openFlapTime_One_H,
			timer.openFlapTime_Dec_M,
			timer.openFlapTime_One_M
		);
		SSD1306_GotoXY(70,40);
		SSD1306_Puts(tmpStrHomeWatch, &Font_7x10, 1);

		// Enter Block
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_WHITE);
		SSD1306_DrawRectangle(90, 50, 20, 20, SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(80,55);
		SSD1306_Puts(tmpStrMenuEnter, &Font_7x10, 1);
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
		// With Back back to DISPLAY_MENU
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

		// Switch to chosen Display Page with Enter
		} else if (display.timerSelect == TIMER_SELECT_CLOSING && button.buttonMenuEnter && button.onePingIfButtonPressed)
			nextDisplayPage(DISPLAY_TIMER_CLOSING);
		else if (display.timerSelect == TIMER_SELECT_OPENING && button.buttonMenuEnter && button.onePingIfButtonPressed)
			nextDisplayPage(DISPLAY_TIMER_OPENING);
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
