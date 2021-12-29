#ifndef DART_COMMON_XI2020_COMPAT2019_H_
#define DART_COMMON_XI2020_COMPAT2019_H_

#include "../../../../chickenFlap/dart/standard_include.h"

#pragma pack(push, 1)
struct Compat_SN_Front_Analog_Raw_Data {
	uint16_t apps1Raw;
	uint16_t apps2Raw;
	uint16_t steeringRaw;
	uint16_t brakePedalRaw;
};

struct Compat_SN_Front_Brake_Raw_Data {
	uint16_t brakeFrontRaw;
	uint16_t brakeRearRaw;
};

struct Compat_SN_Front_R2DS_Data {
	uint8_t enable;
};

struct Compat_SN_Front_EBS_Sound_Data {
	uint8_t enable;
};

struct Compat_SN_Front_SpringTravel {
	uint16_t springTravel_FL;
	uint16_t springTravel_FR;
};

struct Compat_SN_Rear_SpringTravel {
	uint16_t springTravel_RL;
	uint16_t springTravel_RR;
};

struct Compat_VN300_AccelGyro {
	int16_t accelX;
	int16_t accelY;
	int16_t gyroZ;
	int16_t accelZ;
};

struct Compat_VN300_YPR {
	int16_t yaw;
	int16_t pitch;
	int16_t roll;
};

struct Compat_VN300_Position {
	int32_t latitude;
	int32_t longitude;
};

struct Compat_VN300_Velocity {
	int16_t bodyX;
	int16_t bodyY;
};

struct Compat_VN300_Status {
	uint16_t timeStart;
	uint8_t numGPSSatellites;
	uint8_t : 1;
	bool gpsHeading : 1;
	bool gpsCompass : 1;
	bool gpsFix : 1;
	uint8_t insMode : 2;
	uint8_t gpsMode : 2;
	uint8_t crcErrorCount;
};

struct Compat_VCU_State_Data {
	uint8_t mode;
	uint8_t state;
	uint8_t : 7;
	bool ebsFault : 1;
	uint8_t ebs_checkup;
	uint8_t ros_status;
	uint16_t ros_ping;
};

struct Compat_SN_Front_Gyro {
	int16_t gyroX;
	int16_t gyroY;
	int16_t gyroZ;
};

struct Compat_SN_Front_Accel {
	int16_t accelX;
	int16_t accelY;
	int16_t accelZ;
};

struct Compat_VCU_Mission_Data {
	uint8_t mission;
};

struct Compat_VCU_State_Request_Data {
	uint8_t requestMode;
	uint8_t requestState;
	uint64_t : 48;
};

struct Compat_Dashboard_Param_Request_Data {
	uint8_t parameterID;
};
struct Compat_Dashboard_Param_Transmit_Data {
	uint8_t parameterID;
	uint32_t : 24;
	int32_t value;
};
#pragma pack(pop)

#define COMPAT2019_CANID_PARAM_REQ 0x550
#define COMPAT2019_CANID_PARAM_TRANSMIT 0x551
#define COMPAT2019_PARAM_POWERMODE 4

#define COMPAT2019_VN300_GPS_FIX_NONE 0
#define COMPAT2019_VN300_GPS_FIX_TIME 1
#define COMPAT2019_VN300_GPS_FIX_2D 2
#define COMPAT2019_VN300_GPS_FIX_3D 3

#endif
