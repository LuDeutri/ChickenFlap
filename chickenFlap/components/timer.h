/*
 * Here are functions for the timer which controls the automatic open and close of the flap to the given time
 *
 * Author: Luca Deutrich
 * Date: 26.12.21
 */

#ifndef COMPONENTS_TIMER_H_
#define COMPONENTS_TIMER_H_

#include "config.h"
#include "watch.h"
#include "../dart/hal_wrapper/hal_wrapper.h"

typedef enum {
	TIMER_DEACTIVE,
	TIMER_ONLY_CLOSE,
	TIMER_ONLY_OPEN,
	TIMER_ACTIVE
} timerState_t;

typedef struct{
	timerState_t timerState;

	// Time when the flap should close
	uint8_t openFlapTime_Dec_H;
	uint8_t openFlapTime_One_H;
	uint8_t openFlapTime_Dec_M;
	uint8_t openFlapTime_One_M;

	// Time when the flap should open
	uint8_t closeFlapTime_Dec_H;
	uint8_t closeFlapTime_One_H;
	uint8_t closeFlapTime_Dec_M;
	uint8_t closeFlapTime_One_M;
} flapTimer_t;
flapTimer_t timer;

void timer_init();

/*
 * Returns true if the watch have the same time like the saved timer time to close the chicken flap
 * @param true --> timer-closing can start
 */
bool checkIfTimeToClose();

/*
 * Returns true if the watch have the same time like the saved timer time to open the chicken flap
 * @param true --> timer-opening can start
 */
bool checkIfTimeToOpen();

void nextTimerState(timerState_t nextTimerState);


#endif
