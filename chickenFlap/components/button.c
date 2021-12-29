#include "button.h"

void button_init(){
	button.onePingIfButtonPressed = false;
	button.buttonMenuBack = false;
	button.buttonMenuEnter = false;
	button.buttonLeft = false;
	button.buttonRight = false;
	button.buttonFlapCtrl = false;
	button.firstTimeButtonPressed = 0;
	button.lastTimeButtonPressed = 0;
}

void button_update(){
	buttonPing();
	readButtonState();
	setButtonTime();
	buttonFlapCtrl();
}

void readButtonState(){
	// Ignore the other buttons if one is already pressed
	button.buttonMenuEnter = !digitalRead(BUTTON_MENU_ENTER);
	if(button.buttonMenuEnter)
		return;
	button.buttonMenuBack = !digitalRead(BUTTON_MENU_BACK);
	if(button.buttonMenuBack)
		return;
	button.buttonFlapCtrl = !digitalRead(BUTTON_FLAP_CTRL);
	if(button.buttonFlapCtrl)
		return;
	button.buttonLeft = !digitalRead(BUTTON_LEFT);
	if(button.buttonLeft)
		return;
	button.buttonRight = !digitalRead(BUTTON_RIGHT);
	if(button.buttonRight)
		return;
}

void setButtonTime(){
	// Set starting time if one button is pressed
	// Pushing one button in the sleepmode will not registry as button pressing. Its only to wakeup from sleep mode.
	if (button.firstTimeButtonPressed == 0 && stateMachine.state != STATE_SLEEP &&
			(  !button.buttonMenuEnter
			|| !button.buttonMenuBack
			|| !button.buttonLeft
			|| !button.buttonRight
			|| !button.buttonFlapCtrl)) {
		button.firstTimeButtonPressed = button.lastTimeButtonPressed = millis();
		button.onePingIfButtonPressed = true;
	} else
		button.firstTimeButtonPressed = 0;
}

void buttonFlapCtrl(){
	// Flap ctrl
	// Pushing the button in the sleepmode will not registry as button pressing. Its only to wakeup from sleep mode.
	if (button.buttonFlapCtrl == HIGH && stateMachine.state == STATE_SLEEP)
		return;

	// Stopping the flap when the motor is already running
	if (flap.motorIsRuning)
		stopMotor();

	// Open flap if closed
	else if(flap.actuallyStateFlap == FLAP_CLOSED)
		nextState(STATE_FLAP_OPEN);

	// Close flap if opened
	else if (flap.actuallyStateFlap == FLAP_OPENED)
		nextState(STATE_FLAP_CLOSE);
}

void buttonPing(){
	if (button.onePingIfButtonPressed)
		button.onePingIfButtonPressed = false;
}

bool anyButtonPushed(){
	if(button.buttonFlapCtrl || button.buttonLeft || button.buttonMenuBack || button.buttonMenuEnter || button.buttonRight)
		return true;
	return false;
}
