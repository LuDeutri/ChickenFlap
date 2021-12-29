#pragma once

#include "../../../../chickenFlap/dart/standard_include.h"

#pragma pack(push, 1)

typedef struct {
	uint8_t state;
	uint8_t mission;
	bool hvON : 1;
	bool sdcClosed : 1;
} VCUStateData_t;

typedef struct {
	bool brakeLight : 1;
	uint8_t : 7;
	uint8_t appsPercentage;
	uint8_t hydrBrakePercentage;
	uint8_t brakePedalPercentage;
	int16_t steeringAngle;
} VCUDriverSensorData_t;

typedef struct {
	uint8_t hydrBrakePressureFront;
	uint8_t hydrBrakePressureRear;
	uint16_t springTravelFrontLeft;
	uint16_t springTravelFrontRight;
} VCUUndercarriageSensorData_t;

typedef struct {
	int16_t gyroX;
	int16_t gyroY;
	int16_t gyroZ;
} VCUGyroData_t;

typedef struct {
	int16_t accelerometerX;
	int16_t accelerometerY;
	int16_t accelerometerZ;
} VCUAccelerometerData_t;

typedef struct {
	int16_t environmentTemperature;
	int16_t environmentPressure;
	uint8_t environmentHumidity;
	uint8_t environmentAirQuality;
} VCUEnvironmentData_t;

typedef struct {
	bool appsImplausibility : 1;
	bool appsBrakePedalPlausibility : 1;
	bool apps1Disconnected : 1;
	bool apps2Disconnected : 1;
	bool brakeFrontDisconnected : 1;
	bool brakeRearDisconnected : 1;
	bool steeringAngleDisconnected : 1;
} VCUSensorErrorData_t;

typedef struct {
	uint8_t fan1;
	uint8_t fan2;
	uint8_t pump1;
	uint8_t pump2;
} VCUControlCoolingData_t;

typedef struct {
	uint8_t year : 6;
	uint8_t month : 4;
	uint8_t day : 5;
	uint8_t hours : 5;
	uint8_t minutes : 6;
	uint8_t seconds : 6;
	uint8_t vbat : 8;
} VCUDateTimeData_t;

typedef struct {
	uint8_t vehicle;
} VCUDebugData_t;

typedef struct {
	uint16_t rawAPPS1;
	uint16_t rawAPPS2;
	uint16_t rawBrakeFront;
	uint16_t rawBrakeRear;
} VCUDebugAnalog1Data_t;

typedef struct {
	uint16_t rawSteeringAngle1;
	uint16_t rawSteeringAngle2;
	uint16_t rawBrakePedal1;
	uint16_t rawBrakePedal2;
} VCUDebugAnalog2Data_t;

typedef struct {
	uint16_t rawVPPX1;
	uint16_t rawVPPX2;
	uint16_t rawVPPXOutput;
} VCUDebugVPPXData_t;

typedef struct {
	bool tson : 1;
	bool ctrlRelay : 1;
	bool ctrlSDC : 1;
	bool drivem : 1;
	bool extaw : 1;
	bool r2dsEnable : 1;
	bool ebsSoundEnable : 1;
	uint8_t : 1;

	bool steeringButton1 : 1;
	bool steeringButton2 : 1;
	bool steeringButton3 : 1;
	bool steeringButton4 : 1;
	bool steeringButton5 : 1;
	bool steeringButton6 : 1;
	uint8_t : 2;

	bool menuL : 1;
	bool menuM : 1;
	bool menuR : 1;
	uint8_t : 1;
	bool errorRelay : 1;
	bool errorSDC : 1;
	bool errorExtAW : 1;
} VCUDebugDigitalData_t;

typedef struct {
	int16_t torqueFL;
	int16_t torqueFR;
	int16_t torqueRL;
	int16_t torqueRR;
} VCUDebugControllerTorqueData_t;

typedef struct {
	uint16_t timeSeconds;
	uint8_t loadPercentage;
	uint16_t loop;
	uint16_t controller;
	uint8_t loopRough;
} VCUDebugRuntimeData_t;

typedef struct {
	uint8_t cmd;
} VCUDebugCommandData_t;

typedef struct {
	uint8_t year : 6;
	uint8_t month : 4;
	uint8_t day : 5;
	uint8_t hours : 5;
	uint8_t minutes : 6;
	uint8_t seconds : 6;
} VCUSetDateTimeData_t;

typedef struct {
	VCUStateData_t state;
	VCUDriverSensorData_t driverSensor;
	VCUUndercarriageSensorData_t undercarriage;
	VCUGyroData_t gyro;
	VCUAccelerometerData_t accelerometer;
	VCUEnvironmentData_t environment;
	VCUSensorErrorData_t sensorError;
	VCUDateTimeData_t dateTime;
	VCUDebugData_t debugData;
	VCUDebugAnalog1Data_t debugAnalog1;
	VCUDebugAnalog2Data_t debugAnalog2;
	VCUDebugVPPXData_t debugVPPX;
	VCUDebugControllerTorqueData_t debugTorqueData;
	VCUDebugRuntimeData_t debugRuntimeData;
	VCUDebugDigitalData_t debugDigital;
	VCUDebugCommandData_t debugCommand;
} VCUData_t;
#pragma pack(pop)

#define CAN_ID_VCU_STATE 0x571
#define CAN_ID_VCU_DRIVER_SENSOR 0x572
#define CAN_ID_VCU_COOLING_CONTROL 0x578
