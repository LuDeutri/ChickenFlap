/*
 * Here are functions to control the flap.
 *
 * Author: Luca Deutrich
 * Date: 26.12.21
 */

#ifndef COMPONENTS_FLAP_H_
#define COMPONENTS_FLAP_H_

#include "config.h"
#include "stateMachine.h"
#include "../dart/hal_wrapper/hal_wrapper.h"

typedef struct{
	bool actuallyStateFlap;
	bool targetStateFlap;
	bool motorIsRuning;
} flap_t;
flap_t flap;

util_time_t motorStartingTime;

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
 * Checks if both MosFETs are switched on.
 * @return true if both MosFETs are switched on
 */
bool safetyTest();

/*
 *Calculate the stoping time of the motor, depending on the given distance of the flap, the battery voltage and the max. motor speed
 *@return Time on which the motor should stops
 */
uint32_t calculateMotorStopTime();

#endif
