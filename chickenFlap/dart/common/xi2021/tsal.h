#ifndef DART_COMMON_XI2021_TSAL_H_
#define DART_COMMON_XI2021_TSAL_H_

#include "../../../../chickenFlap/dart/standard_include.h"

#pragma pack(push, 1)
typedef struct {
	bool TSON : 1;
	bool TSOFF : 1;
} TSAL_TSStatus_t;
#pragma pack(pop)

#define CAN_ID_TSAL_TS_STATUS 0x284

#endif
