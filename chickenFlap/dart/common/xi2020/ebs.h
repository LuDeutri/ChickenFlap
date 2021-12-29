#ifndef DART_COMMON_XI2020_EBS_H_
#define DART_COMMON_XI2020_EBS_H_

#include "../../../../chickenFlap/dart/standard_include.h"

typedef enum  {
	EBSInit = 0,
	EBSUnavailable = 1,
	EBSArmed = 2
} EBS_state_t;

#pragma pack(push, 1)
typedef struct {
	 uint8_t State : 8;

	 bool ASMS_State : 1;
	 bool TSActivation_State : 1;
	 bool SDC_Ready : 1;
	 bool Ack_24V_RES2 : 1;

	 bool Ack_SupplyValve : 1;
	 bool Ack_ControlValveLeft: 1;
	 bool Ack_ControlValveRight : 1;
	 bool Ack_SteeringRelay : 1;

	 bool Error_Internal : 1;
	 bool Error_Heartbeat : 1;
	 bool Error_RelayFault : 1;
	 bool Error_Supply_Pressure : 1;
	 bool Error_EBSLeft_Pressure : 1;
	 bool Error_EBSRight_Pressure : 1;
	 bool Error_VPPX_Pressure : 1;
	 bool Driver_Status : 1;

	 bool Logic_AS_CloseSDC : 1;
	 bool Logic_DrivingMode : 1;
	 bool Logic_Watchdog : 1;
	 bool Logic_IsTSLatched: 1;
	 bool Logic_IsTriggered : 1;
} EBS_Status_Data_t;

typedef struct {
	uint16_t ebsLeft;
	uint16_t ebsRight;
	uint16_t supply;
	uint16_t vppx;
} EBS_Pressures_Data_t;

typedef struct {
	bool asMode : 1;
	bool closeSDC : 1;
	bool enableWatchdog : 1;
	bool setSupplyValve : 1;
	bool setControlValveLeft : 1;
	bool setControlValveRight : 1;
	bool setSteeringRelay : 1;
} EBS_Control_Data_t;
#pragma pack(pop)

#define CAN_ID_EBS_STATUS 0x561
#define CAN_ID_EBS_PRESSURES 0x562
#define CAN_ID_EBS_CONTROL 0x563

#endif
