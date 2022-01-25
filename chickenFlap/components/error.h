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

typedef struct{
	bool stateMachine;
	bool motorMaxRunningTime;
	bool safetyTest;
	bool timerTimes;
	bool displayInit;
	bool emptyBattery;
}error_t;
error_t error;

typedef struct{
	bool lowBattery;
	bool noBatteryData;
	bool startAnimation;
} warning_t;
warning_t warning;

err_delay_t errTimerTimes;
err_delay_t errDisplayInit;
err_delay_t warnNoBatteryData;

void error_init();
void error_update();

/*
 * Checks all errors in the error_t struct. If one of them is true, the statemachine switchs to state STATE_ERROR
 */
void checkAllErrors();

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

#endif
