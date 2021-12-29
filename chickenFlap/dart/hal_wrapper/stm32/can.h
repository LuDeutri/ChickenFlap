#ifndef DART_HAL_WRAPPER_STM32_CAN_H_
#define DART_HAL_WRAPPER_STM32_CAN_H_

#include "../../../../chickenFlap/dart/can.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_CAN)

extern CAN_HandleTypeDef* canHandles[CAN_MAX_CHANNEL_COUNT];

void hal_can_fillReceiveBuffer_fifo(int can, CAN_HandleTypeDef* handle, uint32_t fifo);

#endif

#endif
