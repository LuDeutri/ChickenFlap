/*
 * Here are functions to control the OLED display
 *
 * Author: Luca Deutrich
 * Date: 26.12.2021
 */

#ifndef COMPONENTS_DISPLAY_H_
#define COMPONENTS_DISPLAY_H_

#include <string.h>
#include <stdio.h>

#include "../ssd1306/ssd1306.h"
#include "../ssd1306/ssd1306_startAnimation.h"

#include "bms.h"
#include "button.h"
#include "config.h"
#include "timer.h"
#include "watch.h"
#include "../dart/hal_wrapper/hal_wrapper.h"

#define DISPLAY_OFF 0
#define DISPLAY_ON 1

#define CONFIG_SELECT_BLOCKS_NUMBER 2
#define MENU_SELECT_BLOCKS_NUMBER 2
#define TIMER_SELECT_BLOCKS_NUMBER 6

typedef enum{
	DISPLAY_HOME,
	DISPLAY_MENU,
	DISPLAY_WATCH,
	DISPLAY_TIMER,
	DISPLAY_TIMER_CLOSING,
	DISPLAY_TIMER_OPENING,
	DISPLAY_CONFIG,
	DISPLAY_CONFIG_PAGE_2,
	DISPLAY_CONFIG_IN_USE,
	DISPLAY_CONFIG_ADJUST_TIME,
	DISPLAY_CONFIG_MOTORSPEED
} displayPage_t;

typedef enum{
	MENU_SELECT_WATCH,
	MENU_SELECT_TIMER,
	MENU_SELECT_CONFIG
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

typedef enum{
	TIMER_TIME_OPENING,
	TIMER_TIME_CLOSING
} timerTime_t;

typedef enum{
	CONFIG_SELECT_IN_USE,
	CONFIG_SELECT_ADJUST_TIME,
	CONFIG_SELECT_MOTORSPEED
} configSelect_t;

typedef enum{
	ADJUST_TIME_SELECT_NONE,
	ADJUST_TIME_SELECT_SECONDS
} adjustTimeSelect_t;

typedef struct{
	uint8_t displayPage; 	// Enum: displayPage_t
	uint8_t menuSelect;		// Enum: menuSelect_t
	uint8_t watchSelect;	// Enum: watchSelect_t
	uint8_t timerSelect;	// Enum: timerSelect_t
	uint8_t timerTimeSelect;	// Enum: timerTimeSelect_t
	uint8_t configSelect;		// Enum: configSelect_t
	uint8_t adjustTimeSelect;	// Enum: adjustTimeSelect_t
	bool notificationWindowActive; // True if an error or warn window is shown on the display
} display_t;
extern display_t display;

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

/*
 * Add string with an rectangle in the down right edge
 * @param string to be shown
 */
void addBlockDownRight(char* string);

/*
 * Add string with an rectangle in the down left edge
 * @param string to be shown
 */
void addBlockDownLeft(char* string);

void closeNotificationWindow();

#endif
