#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "bms.h"
#include "config.h"
#include "display.h"
#include "flap.h"
#include "timer.h"
#include "../dart/hal_wrapper/hal_wrapper.h"
#include "../dart/util/time.h"


// States State machine
typedef enum{
	STATE_INIT,
	STATE_FLAP_CLOSE,
	STATE_FLAP_OPEN,
	STATE_SLEEP,
	STATE_ERROR
} state_t;

typedef struct{
	uint8_t state;
	uint8_t lastState;
	util_time_t firstTimeInState;
	bool errLED; // LOW: errLED is on, HIGH: errLED is off
}stateMachine_t;
extern stateMachine_t stateMachine;

void stateMachine_init();
void stateMachine_update();

/*
 * Switch state in the statemachine to the given next one
 * @param next State - use values from enum typ state_t
 */
void nextState(state_t nextState);

/*
 * Check if there was an button activity in the configured time (config.h)
 * If not it switch into sleep mode.
 * With an button activity it returns to the last state.
 */
void checkActivity();



#endif
