#include "stateMachine.h"

stateMachine_t stateMachine;

void stateMachine_init(){
	stateMachine.state = STATE_INIT;
	stateMachine.lastState = STATE_INIT;
	stateMachine.firstTimeInState = 0;
	stateMachine.errLED = LOW; 		// Error LED goes on for a functionality test at startup
	digitalWrite(DEBUG_LED_GREEN, HIGH);	// Debug LED off, if flag is not set, debug LED stays off
}

void stateMachine_update(){
	// Check for activity to switch into sleep mode
	if(stateMachine.state > STATE_INIT)
		checkActivity();

	switch(stateMachine.state){
	case STATE_INIT:
		//Wait until the OLED display is ready
		if(!SSD1306.Initialized){
			ssd1306_Init();
			break;
		}
		// Runtime must be higher than 1000 ms  to continue to the next state. This is to ensure everything is ready to work.
		if(millis()>1000)
			nextState(STATE_FLAP_CLOSE);
		break;
	case STATE_FLAP_CLOSE:
		flap.targetStateFlap = STATE_FLAP_CLOSE;
		closeFlap();

		// Timer opening
		if (checkIfTimeToOpen())
			nextState(STATE_FLAP_OPEN);
		break;
	case STATE_FLAP_OPEN:
		flap.targetStateFlap = STATE_FLAP_OPEN;
		openFlap();

		// Timer closing
		if (checkIfTimeToClose())
			nextState(STATE_FLAP_CLOSE);
		break;
	case STATE_SLEEP:
		displayDisable();

		// Switch back to the last state if any button is pushed
		if(anyButtonPushed()){
			displayEnable();
			nextState(stateMachine.lastState);
		}

		// Timer opening
		if (checkIfTimeToOpen()){
			displayEnable();
			nextState(STATE_FLAP_OPEN);
		}

		// Timer closing
		if (checkIfTimeToClose()){
			displayEnable();
			nextState(STATE_FLAP_CLOSE);
		}
		break;
	case STATE_ERROR:
		// Close the flap
		flap.targetStateFlap = STATE_FLAP_CLOSE;
		if (flap.actuallyStateFlap < FLAP_CLOSED)
			closeFlap();
		// Stay in the error state
		break;
	default:
		error.stateMachine = true;
		nextState(STATE_ERROR);
		break;
	}
}

void nextState(state_t nextState){
	if (stateMachine.state == nextState)
		return;

	dartUART_formatLine(3, "[%d ms] state change: %d -> %d", millis(), stateMachine.state, (int)nextState);
	if (stateMachine.state != STATE_SLEEP)
		stateMachine.lastState = stateMachine.state;
	stateMachine.state = nextState;
	stateMachine.firstTimeInState = millis();
}

void checkActivity(){
	if (stateMachine.state == STATE_SLEEP)
		return;
	if(anyButtonPushed() || timer.timerActionRunning)
		return;

	if (millis() > button.lastTimeButtonPressed + TIMEOUT_SLEEP_MODE)
		nextState(STATE_SLEEP);
}
