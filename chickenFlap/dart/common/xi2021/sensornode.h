#ifndef DART_COMMON_XI2021_SENSORNODE_H_
#define DART_COMMON_XI2021_SENSORNODE_H_


#include "../../../../chickenFlap/dart/common/xi2020/sensornode.h"

#pragma pack(push, 1)

typedef struct {
	bool pumpEnable : 1;
	uint16_t speedCmd;
	uint16_t speedActual;
	uint8_t aliveRollingCount : 2;
	uint16_t motorACCurrent;
	bool internalOperationErr : 1;
	bool comunicationErr : 1;
	bool powerErr : 1;
	bool overTemp : 1;
	bool motorDryRun : 1;
	bool motorStall : 1;
} Sensornode_pumpStatus_t;

typedef struct {
	bool ok : 1;
	bool directionXUnknown : 1;
	bool directionYUnknown : 1;
	bool error_noData : 1;
	bool error_dataFormat : 1;
	uint8_t : 3;
	int16_t velocityX;
	int16_t velocityY;
} Sensornode_speedWedge_t;
#pragma pack(pop)

#define CAN_ID_SENSORNODE_PUMP_STATUS 0x288
#define CAN_ID_SENSORNODE_SPEEDWEDGE 0x289

#endif
