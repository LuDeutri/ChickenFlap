#include "timer.h"

void timer_init(){
	// Set default clap opening / closing time
	timer.openFlapTime_Dec_H = DEFAULT_OPEN_FLAP_TIME_DEC_H;
	timer.openFlapTime_One_H = DEFAULT_OPEN_FLAP_TIME_ONE_H;
	timer.openFlapTime_Dec_M = DEFAULT_OPEN_FLAP_TIME_DEC_MIN;
	timer.openFlapTime_One_M = DEFAULT_OPEN_FLAP_TIME_ONE_MIN;
	timer.closeFlapTime_Dec_H = DEFAULT_CLOSE_FLAP_TIME_DEC_H;
	timer.closeFlapTime_One_H = DEFAULT_CLOSE_FLAP_TIME_ONE_H;
	timer.closeFlapTime_Dec_M = DEFAULT_CLOSE_FLAP_TIME_DEC_MIN;
	timer.closeFlapTime_One_M = DEFAULT_CLOSE_FLAP_TIME_ONE_MIN;

	timer.timerState = TIMER_ACTIVE;
}

bool checkIfTimeToClose(){
	// Do nothing / stay opened, if timer is deactivated or only used to open the flap
	if(timer.timerState != TIMER_ONLY_CLOSE || timer.timerState != TIMER_ACTIVE)
		return false;

	return  (timer.closeFlapTime_Dec_H == watch.watchDecHour
			&& timer.closeFlapTime_One_H == watch.watchOneHour
			&& timer.closeFlapTime_Dec_M == watch.watchDecMinute
			&& timer.closeFlapTime_One_M == watch.watchOneMinute);
}

bool checkIfTimeToOpen(){
	// Do nothing / stay closed, if timer is deactivated or only used to close the flap
	if(timer.timerState < TIMER_ONLY_CLOSE)
		return false;

	return  (timer.openFlapTime_Dec_H == watch.watchDecHour
			&& timer.openFlapTime_One_H == watch.watchOneHour
			&& timer.openFlapTime_Dec_M == watch.watchDecMinute
			&& timer.openFlapTime_One_M == watch.watchOneMinute);
}

void nextTimerState(timerState_t nextTimerState){
	if (timer.timerState == nextTimerState)
		return;

	timer.timerState = nextTimerState;
}
