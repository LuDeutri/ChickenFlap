#include "../../../../chickenFlap/dart/hal_wrapper/stm32f7/can.h"

#if defined(USE_STM32F7) && defined(HAL_ENABLE_CAN)

void hal_can_set_baudrate(const int can, const CAN_Baudrate_t baudrate) {
	CAN_HandleTypeDef* handle = canHandles[can];
	if (handle == NULL)
		return;

	DART_ASSERT(handle->State != HAL_CAN_STATE_ERROR, DART_ERROR_HAL);

	if (handle->State == HAL_CAN_STATE_LISTENING)
		HAL_CAN_Stop(handle);

	// This is for 54 MHz clock
	if (baudrate == CAN_RATE_1000KBPS)
		handle->Init.Prescaler = 3;
	else if (baudrate == CAN_RATE_500KBPS)
		handle->Init.Prescaler = 6;
	else if (baudrate == CAN_RATE_250KBPS)
		handle->Init.Prescaler = 12;
	else
		DART_NOT_IMPLEMENTED();
	handle->Init.Mode = CAN_MODE_NORMAL;
	handle->Init.SyncJumpWidth = CAN_SJW_1TQ;
	handle->Init.TimeSeg1 = CAN_BS1_15TQ;
	handle->Init.TimeSeg2 = CAN_BS2_2TQ;
	handle->Init.TimeTriggeredMode = DISABLE;
	handle->Init.AutoBusOff = ENABLE;
	handle->Init.AutoWakeUp = ENABLE;
	handle->Init.AutoRetransmission = DISABLE;
	handle->Init.ReceiveFifoLocked = DISABLE;
	handle->Init.TransmitFifoPriority = DISABLE;
	DART_ASSERT(HAL_CAN_Init(handle) == HAL_OK, DART_ERROR_HAL);
	DART_ASSERT(HAL_CAN_Start(handle) == HAL_OK, DART_ERROR_HAL);
}

#endif
