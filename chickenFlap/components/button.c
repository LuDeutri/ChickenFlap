#include "button.h"

button_t button;

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
	buttonPingReset();
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
	button.buttonJoker = !digitalRead(BUTTON_JOKER);
	if(button.buttonJoker)
		return;
}

void setButtonTime(){
	// Set starting time if one button is pressed
	if (button.firstTimeButtonPressed == 0 && (
			   button.buttonMenuEnter
			|| button.buttonMenuBack
			|| button.buttonLeft
			|| button.buttonRight
			|| button.buttonFlapCtrl))
	{
		button.firstTimeButtonPressed = button.lastTimeButtonPressed = millis();
		// If display off, dont set a button ping
		if(ssd1306_GetDisplayOn() == 1 && !display.notificationWindowActive)
			button.onePingIfButtonPressed = true;
	}
}

void buttonFlapCtrl(){
	// Return if button is not pressed
	if(button.buttonFlapCtrl == LOW || !button.onePingIfButtonPressed)
		return;

	// Stop if timer activate the motor actually
	if(checkIfTimeToOpen() || checkIfTimeToClose())
		return;


	// Ignore the stop button if the flap is in reverse direction after an middle break up
	if (flap.motorButtonCtrlTime > 0 && flap.motorIsRuning)
		return;

	// Stopping the flap when the motor is already running
	if(flap.motorButtonCtrlTime == 0 && flap.motorIsRuning){
		if(flap.motorOperationTimeSetted){
			// Safe actual flap motion state
			flap.motorWaitForButton = true;
			// Safe motor active duration for the reserve movement
			flap.motorButtonCtrlTime = flap.motorRunningTime;
			// Stop motor
			stopMotor();
			// Reset motorCtrl state
			flap.motorIsRuning = false;
		} else {
			// Motor action time is set
			flap.motorOperationTimeSetted = true;
			// Stop motor
			stopMotor();
		}
	}
	// Start the motor if the button is pressed
	else {
		// Enable motor movement
		flap.motorWaitForButton = false;

		switch(flap.actuallyStateFlap){
		case FLAP_OPENED:
			nextState(STATE_FLAP_CLOSE);
			break;
		case FLAP_OPENING:
			nextState(STATE_FLAP_CLOSE);
			break;
		case FLAP_CLOSING:
			nextState(STATE_FLAP_OPEN);
			break;
		case FLAP_CLOSED:
			nextState(STATE_FLAP_OPEN);
			break;
		default:
			break;
		}
	}
}

void buttonPingReset(){
	button.onePingIfButtonPressed = false;
}

bool anyButtonPushed(){
	if(button.buttonFlapCtrl || button.buttonLeft || button.buttonMenuBack || button.buttonMenuEnter || button.buttonRight)
		return true;
	return false;
}
