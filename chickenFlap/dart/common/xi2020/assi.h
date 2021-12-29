#ifndef DART_COMMON_XI2020_CAN_ASSI_H_
#define DART_COMMON_XI2020_CAN_ASSI_H_

#include "../../../../chickenFlap/dart/standard_include.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	ASSI_OFF = 1,
	ASSI_YELLOW = 2,
	ASSI_YELLOW_BLINK = 3,
	ASSI_BLUE = 4,
	ASSI_BLUE_BLINK = 5,
	ASSI_BOTH = 6,
	ASSI_BOTH_BLINK = 7,
	ASSI_BOTH_ALTERNATE = 8
} ASSI_state_t;

// CAN messages
#pragma pack(push, 1)
typedef struct {
	uint8_t state;
} ASSI_control_data_t;

typedef struct {
	uint8_t magic;
	uint8_t version;
	uint8_t state;
	uint8_t timer;
	bool startup : 1;
	bool error_controlMessageLost : 1;
	bool error_unknownState : 1;
} ASSI_heartbeat_data_t;
#pragma pack(pop)

#define CAN_ID_ASSI_CONTROL 0x621
#define CAN_ID_ASSI_HEARTBEAT 0x622

const char* assi_state_string(const ASSI_state_t state);


#ifdef __cplusplus
}
#endif

#endif
