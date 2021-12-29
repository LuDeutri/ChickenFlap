#ifndef DART_COMMON_XI2020_FUSEBOX_H_
#define DART_COMMON_XI2020_FUSEBOX_H_

#include "../../../../chickenFlap/dart/standard_include.h"

// CAN messages
#pragma pack(push, 1)
typedef struct {
	uint8_t magic;
	uint8_t version;
	uint16_t power;
	uint8_t timer;
	bool error_i2c : 1;
	bool error_i2c_base : 1;
	bool error_i2c_fans : 1;
	bool error_i2c_pumps : 1;
	bool error_i2c_sensor : 1;
	bool error_i2c_asms : 1;
	bool error_i2c_pc : 1;
	bool error_i2c_as : 1;
	int16_t sumCurrent;
} Fuse_status_data_t;

typedef struct {
	int16_t current_base;
	int16_t current_fans;
	int16_t current_pumps;
	int16_t current_sensor;
} Fuse_currents_1_t;

typedef struct {
	int16_t current_asms;
	int16_t current_pc;
	int16_t current_as;
} Fuse_currents_2_t;

typedef struct {
	uint16_t voltage_lvms;
	uint16_t voltage_asms;
} Fuse_voltages_t;
#pragma pack(pop)

#define CAN_ID_FUSE_STATUS 0x341
#define CAN_ID_FUSE_CURRENTS_1 0x342
#define CAN_ID_FUSE_CURRENTS_2 0x343
#define CAN_ID_FUSE_VOLTAGES 0x344

#endif
