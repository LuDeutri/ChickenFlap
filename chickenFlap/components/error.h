/*
 * This class implements the error handling
 *
 * Author: Luca Deutrich
 * Date: 26.12.2021
 */

#ifndef COMPONENTS_ERROR_H_
#define COMPONENTS_ERROR_H_

#include "../dart/hal_wrapper/hal_wrapper.h"
#include "flap.h"

typedef struct{
	bool stateMachine;
	bool motorMaxRunningTime;
	bool safetyTest;
}error_t;
error_t error;

void error_init();
void error_update();

void setMotorEnable();

#endif
