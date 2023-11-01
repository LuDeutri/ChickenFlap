/*
 * Here are functions to overwatch and work with the buttons
 *
 * In this project are 5 Buttons included.
 * 4 of them to control diffrent functions in the display.
 * The 5th is to open/close the flap without influencing the timer.
 *
 * Author: Luca Deutrich
 * Date: 26.12.21
 */

#ifndef COMPONENTS_BUTTON_H_
#define COMPONENTS_BUTTON_H_

#include "config.h"
#include "flap.h"
#include "stateMachine.h"
#include "../dart/hal_wrapper/hal_wrapper.h"

typedef struct{
	bool onePingIfButtonPressed; // For one code run true if a button is pressed. Can only come true again with a new push action.
	util_time_t firstTimeButtonPressed;
	util_time_t lastTimeButtonPressed;
	bool buttonMenuBack;
	bool buttonMenuEnter;
	bool buttonLeft; // not the position, its the function (its acutally on the left side of the display)
	bool buttonRight; // not the position, its the function (its acutally on the rigth side of the display)
	bool buttonFlapCtrl;
	bool buttonJoker; // not used
} button_t;
extern button_t button;

void button_init();
void button_update();

/*
 * Read out digital inputs of buttons.
 * If one is high the others are ignored.
 */
void readButtonState();

/*
 * Save the actually system time in the struct if one button is pressed
 */
void setButtonTime();

/*
 * Button 5, controls the Flap without influencing the timer.
 */
void buttonFlapCtrl();

/*
 * Button is used to start the start animation again. Only possible if the system clk is not used
 */
void buttonJoker();

/*
 * If ping is true set it false.
 *
 * This method must called before readButtonState().
 * On this way a pressed Button will be valid for one code runtime.
 * Only until the buttons were unpressed, die ping can go again true for one code runtime.
 */
void buttonPingReset();

/*
 * True if any button is pushed. Otherwise false
 * @return boolean operator
 */
bool anyButtonPushed();


#endif
