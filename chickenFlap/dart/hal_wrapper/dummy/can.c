#include "../../../../chickenFlap/dart/hal_wrapper/dummy/can.h"

#ifdef HAL_DUMMY_CAN
void hal_can_init() {
	// Do nothing
}

void hal_can_update() {
	// Do nothing
}

void hal_can_yield() {
	// Do nothing
}


void hal_can_set_baudrate(const int can, const CAN_Baudrate_t baudrate) {
	DART_NOT_USED(can);
	DART_NOT_USED(baudrate);
	// Do nothing
}

void hal_can_createTransmitMessage(CANMessage_t* message) {
	DART_NOT_USED(message);
	// Do nothing
}

void hal_can_flushTransmitBuffer(int can, ringbuffer_t* ringBuffer, CANBufferItem_t* buffer) {
	DART_NOT_USED(can);
	DART_NOT_USED(buffer);
	// Empty buffer
	while (!rb_isempty(ringBuffer))
		rb_get(ringBuffer);
}

void hal_can_fillReceiveBuffer() {
	// Do nothing
}

bool can_has_error(int can) {
	DART_NOT_USED(can);
	return false;
}

uint32_t can_get_error_code(int can) {
	DART_NOT_USED(can);
	return 0;
}

void can_reset_error(int can) {
	DART_NOT_USED(can);
	// Do nothing
}
#endif
