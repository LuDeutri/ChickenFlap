/*
 * Here are functions to control the flap.
 *
 * The motor is controlled via low side switches. Addionally an 2-way-relay is used to swicht the positiv power supply (added in pcb revision 2). Therefor it is impossible
 * to short circuit your batterie with both low side switches running at the same time.
 *
 * Each low side swiche has one enable signal and one PWM timer is used for both switches. With this enable signal LOW, the leackage
 * current of the swichtes is on minimum.
 *
 * The 2-way-relay is controlled via an GPIO (MOTOR_VCC_CTRL).
 * LOW: Open is enabled
 * HIGH: Closing is enabled
 *
 * Author: Luca Deutrich
 * Date: 25.10.2023
 */

#ifndef COMPONENTS_FLAP_H_
#define COMPONENTS_FLAP_H_

#include "config.h"
#include "stateMachine.h"
#include "error.h"
#include "../dart/hal_wrapper/hal_wrapper.h"

typedef enum{
	FLAP_OPENED,
	FLAP_OPENING,
	FLAP_CLOSING,
	FLAP_CLOSED
} flapState_t;

typedef enum{ // Relay ctrl pin high or low:
	VCC_OPEN, // MOTOR_VCC_CLOSE is connected
	VCC_CLOSE // MOTOR_VCC_OPEN is connected
} vccCtrl_t;

typedef struct{
	uint8_t actuallymotorDirection;
	uint8_t actuallyDuty;
} lowSideDriverConfig_t;

typedef struct{
	uint8_t actuallyStateFlap; // Use the enum flapState_t
	uint8_t targetStateFlap;	// Use the enum flapState_t
	bool motorEnable;			// False in error case
	bool motorIsRuning;
	util_time_t motorRunningTime;	// Time which the motor is active
	util_time_t motorOperationTime; // Safed duration to open / close the flap
	bool motorOperationTimeSetted;
	util_time_t motorButtonCtrlTime; // If the flap motion stoppped, the running time is safed here, to ensure that the same running time is used in the other direction
	bool motorWaitForButton; 		// Used if the motor is stoped by the button Flap_CTRL
	util_time_t lastTimeMotorRuns;
	lowSideDriverConfig_t mtrDriverConfig;
	uint8_t motorSpeed; 		// In Percentage 0-100% adjusted via potentiometer and only adjustable in DISPLAY_CONFIG_MOTORSPEED
} flap_t;
extern flap_t flap;

extern util_time_t motorStartingTime;

void flap_init();

/*
 * Open the chicken flap
 */
void openFlap();

/*
 * Close the chicken flap
 */
void closeFlap();

/*
 * Controls the motor.
 * Depending on the value given, the motor turns in one direction or the other,
 * which corresponds to the opening or closing of the flap.
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * Note: It must be ensured that both directions are never switched at the same time,
 * as this would correspond to a short circuit of the battery.
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * @param direction for the motor --> 0: closing, 1: opening
 */
void motorCtrl(uint8_t direction);

/*
 * Stops the motor
 */
void stopMotor();

/*
 * Measure the motor operation time if its setting about the IN_USE function. The function will start if motorOperationTimeSetted is set to false.
 * This happens only if the display page switch from DISPLAY_CONFIG to DISPLAY_CONFIG_IN_USE
 * With the FLAP_CTRL_BTTN starts the timer and with pushing this button again it stops and safe the time for later opening or closing the flap.
 */
void measureMotorOperationTime();

/*
 * The speed of the motor (dutycyle of the pwm switches) can be adjusted via an potentiometer on the pcb. This method reads it out
 * and safes the speed in percentage in flap.motorSpeed struct
 */
void setMotorSpeed();

#endif
