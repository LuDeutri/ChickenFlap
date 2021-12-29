#ifndef DART_COMMON_XI2020_SENSORNODE_H_
#define DART_COMMON_XI2020_SENSORNODE_H_

#include "../../../../chickenFlap/dart/standard_include.h"

#pragma pack(push, 1)

//Data for CAN messages (to be transmitted and received) is stored in these structures
typedef struct {
	uint16_t springTravelRearLeft;
	uint16_t springTravelRearRight;
} Sensornode_springTravelData_t;

typedef struct {
	uint8_t magic;
	uint8_t topVersion;
	uint8_t bottomVersion;
} Sensornode_heartbeatData_t;

typedef struct {
	uint8_t drainPercentage[4];
	bool drainStatus1 : 1;
	bool drainStatus2 : 1;
	bool drainStatus3 : 1;
	bool drainStatus4 : 1;
} Sensornode_drainData_t;

typedef struct {
	uint8_t temperature[4];
} Sensornode_temperatureData_t;

typedef struct {
	uint16_t analogIn[4];
} Sensornode_analog1Data_t;

typedef struct {
	uint16_t analogIn[4];
} Sensornode_analog2Data_t;

typedef struct {
	bool digitalIn1 : 1;
	bool digitalIn2 : 1;
	bool digitalIn3 : 1;
	bool digitalOut : 1;
	bool versionCompatible : 1;
} Sensornode_digitalData_t;

#define CAN_ID_SENSORNODE_SPRING_TRAVEL 0x280
#define CAN_ID_SENSORNODE_HEARTBEAT 0x281
#define CAN_ID_SENSORNODE_DRAINS 0x282
#define CAN_ID_SENSORNODE_TEMPERATURE 0x283
#define CAN_ID_SENSORNODE_ANALOG_1 0x285
#define CAN_ID_SENSORNODE_ANALOG_2 0x286
#define CAN_ID_SENSORNODE_DIGITAL 0x287

#pragma pack(pop)


#endif
