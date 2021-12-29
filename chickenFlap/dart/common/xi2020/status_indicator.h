#ifndef DART_COMMON_XI2020_STATUS_INDICATOR_H_
#define DART_COMMON_XI2020_STATUS_INDICATOR_H_

#include "../../../../chickenFlap/dart/standard_include.h"

// CAN messages
#pragma pack(push, 1)
typedef struct {
	uint8_t version : 4;
	uint8_t timer : 4;
	bool sdc_fuse : 1;
	bool sdc_sdbf : 1;
	bool sdc_vcu : 1;
	bool sdc_bots : 1;
	bool sdc_inertia : 1;
	bool sdc_sdbl : 1;
	bool sdc_sdbr : 1;
	bool sdc_interlock : 1;
	bool sdc_hvd : 1;
	bool sdc_bspd : 1;
	bool sdc_imd : 1;
	bool sdc_ams : 1;
	bool sdc_res : 1;
	bool sdc_ebs : 1;
	bool sdc_tsms : 1;
	bool sdc_bat : 1;
	uint8_t first_open;
	uint16_t sdc_fuse_voltage;
	bool error_i2c : 1;
	bool error_led_driver : 1;
} Status_info_data_t;

typedef struct {
	uint8_t brightness;
	bool showAll : 1;
	uint8_t : 7;
} Status_brightness_data_t;
#pragma pack(pop)

#define CAN_ID_STATUS_INFO 0x201
#define CAN_ID_STATUS_BRIGHTNESS 0x202

#endif
