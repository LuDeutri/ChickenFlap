#include "../../../../chickenFlap/dart/hal_wrapper/tms320/can.h"

#if defined(USE_TMS320) && defined(HAL_ENABLE_CAN)


void hal_can_init() {
    DART_NOT_IMPLEMENTED();
}

void hal_can_update() {
    //Do nothing
}

void hal_can_createTransmitMessage(CANMessage_t* message) {
    DART_NOT_IMPLEMENTED();
}

void hal_can_flushTransmitBuffer(int can, ringbuffer_t* ringBuffer, CANBufferItem_t* buffer) {
    hal_can_update();

    DART_NOT_IMPLEMENTED();
}

void hal_can_fillReceiveBuffer() {
    // Do nothing
}
#endif
