#ifndef DART_COMMON_XI2021_FUSEBOX_H_
#define DART_COMMON_XI2021_FUSEBOX_H_

// CAN protocol not changed to 2020
#include "../../../../chickenFlap/dart/common/xi2020/fusebox.h"

// Only temperature data added
#pragma pack(push, 1)
typedef struct {
	int16_t temperature;
	bool error : 1;
	uint8_t : 7;
} Fuse_temperature_data_t;
#pragma pack(pop)

#define CAN_ID_FUSE_TEMPERATURE 0x345

#endif
