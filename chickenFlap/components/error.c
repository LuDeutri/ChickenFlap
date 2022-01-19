#include "error.h"

void error_init(){
	error.stateMachine = false;
	error.motorMaxRunningTime = false;
	error.safetyTest = false;
}

void error_update(){
	setMotorEnable();
}

void setMotorEnable(){
	if(error.motorMaxRunningTime || error.safetyTest)
		flap.motorEnable = false;
}
