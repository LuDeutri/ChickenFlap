#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/can.h"

#if defined(USE_ATMEGA32M1) && defined(HAL_ENABLE_CAN)
void hal_can_init() {
	CAN_filterType filter;
	filter.id = 0;
	filter.mask = 0xFFFF;
	filter.rtr = 0;
	can_set_filter(2, &filter);
}

void hal_can_update() {
	// Not needed
}

void hal_can_yield() {
	// Do nothing
}

void hal_can_set_baudrate(const int can, const CAN_Baudrate_t baudrate) {
	DART_NOT_IMPLEMENTED();
}

void hal_can_createTransmitMessage(CANMessage_t* message) {
	// Not needed
}

void hal_can_flushTransmitBuffer(int can, ringbuffer_t* ringBuffer, CANBufferItem_t* buffer) {
	DART_NOT_IMPLEMENTED();
}

void hal_can_fillReceiveBuffer() {
	DART_NOT_IMPLEMENTED();
}

bool can_has_error(int can) {
	DART_NOT_IMPLEMENTED();
}

uint32_t can_get_error_code(int can) {
	DART_NOT_IMPLEMENTED();
}

void can_reset_error(int can) {
	DART_NOT_IMPLEMENTED();
}
#endif
