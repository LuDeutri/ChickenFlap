#include "timer.h"

flapTimer_t timer;

void timer_init(){
	// Set default clap opening
	timer.openFlapTime_hour = DEFAULT_OPEN_FLAP_TIME_HOUR;
	timer.openFlapTime_minute = DEFAULT_OPEN_FLAP_TIME_MINUTE;
	// Set default clap closing
	timer.closeFlapTime_hour = DEFAULT_CLOSE_FLAP_TIME_HOUR;
	timer.closeFlapTime_minute = DEFAULT_CLOSE_FLAP_TIME_MINUTE;

	timer.timerState = TIMER_ACTIVE;
	timer.timerActionRunning = false;
}

bool checkIfTimeToClose(){
	// Do nothing / stay opened, if timer is deactivated or only used to open the flap
	if(timer.timerState != TIMER_ONLY_CLOSE && timer.timerState != TIMER_ACTIVE)
		return false;

	timer.timerActionRunning = (timer.closeFlapTime_hour == watch.hour
								&& timer.closeFlapTime_minute == watch.minute);

	if(timer.timerActionRunning)
		button.lastTimeButtonPressed = millis();

	return  timer.timerActionRunning;
}

bool checkIfTimeToOpen(){
	// Do nothing / stay closed, if timer is deactivated or only used to close the flap
	if(timer.timerState < TIMER_ONLY_CLOSE)
		return false;

	timer.timerActionRunning = (timer.openFlapTime_hour == watch.hour
								&& timer.openFlapTime_minute == watch.minute);

	if(timer.timerActionRunning)
			button.lastTimeButtonPressed = millis();

	return  timer.timerActionRunning;
}

void nextTimerState(timerState_t nextTimerState){
	if (timer.timerState == nextTimerState)
		return;

	timer.timerState = nextTimerState;
}
