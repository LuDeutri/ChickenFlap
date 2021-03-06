/*
 * This class implements the error handling
 *
 * Author: Luca Deutrich
 * Date: 20.01.2022
 */

#ifndef COMPONENTS_ERROR_H_
#define COMPONENTS_ERROR_H_

#include "../dart/hal_wrapper/hal_wrapper.h"
#include "../dart/util/error_delay.h"
#include "flap.h"
#include "../ssd1306/ssd1306_startAnimation.h"

/*
 * With new errors/warnings you have to update:
 * 	notificationList_t
 * 	error_t / warning_t
 * 	array size of notifications in error_t struct
 * 	numberNotifications
 * 	setErrorDescription() / setWarningDescription()
 * 	error_init()
 * 	checkAllErrors() / chekAllWarnings()
 * 	showNotificationWindow()
 */

typedef enum{
	ERROR_STATE_MACHINE,
	ERROR_MOTOR_MAX_RUNNING_TIME,
	ERROR_SAFETY_TEST,
	ERROR_TIMER_TIMES,
	ERROR_DISPLAY_INIT,
	ERROR_EMPTY_BATTERY,
	WARNING_LOW_BATTERY,
	WARNING_NO_BATTERY_DATA,
	WARNING_START_ANIMATION,
	WARNING_TIMER_TIMES
}notificationList_t;

typedef struct{
	bool stateMachine;
	bool motorMaxRunningTime;
	bool safetyTest;
	bool timerTimes;
	bool displayInit;
	bool emptyBattery;
	char errorDescription[255];
	bool notifications[10]; // Value is true if the error with the index, of the enum, was shown already
	util_time_t timeNotificationShown;
}error_t;
error_t error;

typedef struct{
	bool overallWarning;
	bool lowBattery;
	bool noBatteryData;
	bool startAnimation;
	bool timerTimes;
} warning_t;
warning_t warning;

err_delay_t errTimerTimes;
err_delay_t errDisplayInit;
err_delay_t warnNoBatteryData;
err_delay_t errBatteryEmpty;
err_delay_t warnBatteryLow;

void error_init();
void error_update();

/*
 * Checks all errors in the error_t struct. If one of them is true, the statemachine switchs to state STATE_ERROR
 */
void checkAllErrors();

/*
 * Check if an warning occured
 */
void checkForWarnings();

/*
 * Checks if there is an error which is motor critical. If yes the motor will be disabled, so that the flap does not close anymore in an error case
 */
void setMotorEnable();

/*
 * Check if the opening and closing time of the timer are the same. After a delay, error.timerTimes will set to true.
 */
void checkTimerTimes();

/*
 * Checks the battery capacity level
 */
void checkBatteryCapacity();

/*
 * If an error in the start animation occurs, stop the animation and go to normal operations.
 */
void checkStartAnimation();

/*
 * Check if the display is initialized before timeout is reached
 */
void checkDisplayInit();

/*
 * Create an error / a warning description string to show on the display
 */
void setErrorDescription();

/*
 * Show an error or warning window on the display if ones occured
 */
void showNotificationWindow();

/*
 * Set notification as shown, that it will not shown again, until the array gets reseted
 */
void setNotificationShown();

/*
 * Reset array list afer NOTIFICATION_SHOWN_TIMEOUT
 */
void resetNotificationShown();

/*
 * Used to test the error handling. In the normal operation, this method should not be called.
 */
void errorTest();

#endif
