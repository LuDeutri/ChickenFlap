/*
 * Here are functions to control the OLED display
 *
 * Author: Luca Deutrich
 * Date: 26.12.2021
 */

#ifndef COMPONENTS_DISPLAY_H_
#define COMPONENTS_DISPLAY_H_

#include <ssd1306.h>

#include "bms.h"
#include "button.h"
#include "config.h"
#include "timer.h"
#include "watch.h"
#include "../dart/hal_wrapper/hal_wrapper.h"

typedef enum{
	DISPLAY_HOME,
	DISPLAY_MENU,
	DISPLAY_WATCH,
	DISPLAY_TIMER,
	DISPLAY_TIMER_CLOSING,
	DISPLAY_TIMER_OPENING
} displayPage_t;

typedef enum{
	MENU_SELECT_WATCH,
	MENU_SELECT_TIMER
} menuSelect_t;

typedef enum{
	WATCH_SELECT_NONE,
	WATCH_SELECT_HOUR,
	WATCH_SELECT_MINUTE
} watchSelect_t;

typedef enum{
	TIMER_SELECT_NONE,
	TIMER_SELECT_OPENING_ACTIVATE,
	TIMER_SELECT_OPENING_DEACTIVATE,
	TIMER_SELECT_OPENING_SET_TIME,
	TIMER_SELECT_CLOSING_ACTIVATE,
	TIMER_SELECT_CLOSING_DEACTIVATE,
	TIMER_SELECT_CLOSING_SET_TIME,
} timerSelect_t;

typedef enum{
	TIMER_TIME_SELECT_NONE,
	TIMER_TIME_SELECT_HOUR,
	TIMER_TIME_SELECT_MINUTE
} timerTimeSelect_t;


typedef struct{
	bool displayEnable; // True: Display on, False: Display off
	uint8_t displayPage; // Value can use from enum displayPage_t
	uint8_t menuSelect;
	uint8_t watchSelect;
	uint8_t timerSelect;
	uint8_t timerTimeSelect;
} display_t;
display_t display;

void display_init();
void display_update();

/*
 * Update the display data and shown page
 */
void displayStateMachine();

/*
 * Update the display target state with the user navigation.
 * Not until displayStateMachine() the changes are implemented.
 */
void displayNavigation();

/*
 * Set the display page to the given one.
 * @param next display page. Values form displayPage_t enum can be used.
 */
void nextDisplayPage(displayPage_t nextDisplayPage);

/*
 * Enable the display.
 * RES pin of the display is set to Vcc
 */
void displayEnable();

/*
 * Disable the display.
 * RES pin of the display is set to GND
 */
void displayDisable();

#endif
