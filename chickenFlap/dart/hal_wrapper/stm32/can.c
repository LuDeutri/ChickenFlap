#include "../../../../chickenFlap/dart/hal_wrapper/stm32/can.h"

#if defined(USE_STM32) && defined(HAL_ENABLE_CAN)

#ifndef CAN_MAP
#error "CAN_MAP is not defined"
#endif


CAN_HandleTypeDef* canHandles[CAN_MAX_CHANNEL_COUNT] = CAN_MAP;

bool canInit = true;
bool canDirtyTX = false;

void hal_can_init() {
	// Create every receive filter
	for (int i = 0; i < CAN_MAX_CHANNEL_COUNT; i++) {
		CAN_HandleTypeDef* handle = canHandles[i];
		if (handle == NULL)
			continue;

		CAN_FilterTypeDef filter;
		filter.FilterBank = 0;
		filter.FilterIdHigh = 0x0000;
		filter.FilterIdLow = 0xFFFF;
		filter.FilterMaskIdLow = 0x0000;
		filter.FilterMaskIdHigh = 0x0000;
		filter.FilterFIFOAssignment = CAN_RX_FIFO0;
		filter.FilterMode = CAN_FILTERMODE_IDMASK;
		filter.FilterScale = CAN_FILTERSCALE_32BIT;
		filter.FilterActivation = CAN_FILTER_ENABLE;
		filter.SlaveStartFilterBank = 0;

		DART_ASSERT(HAL_CAN_ConfigFilter(handle, &filter) == HAL_OK, DART_ERROR_HAL);

		CAN_FilterTypeDef filter1;
		filter1.FilterBank = 1;
		filter1.FilterIdHigh = 0x0000;
		filter1.FilterIdLow = 0xFFFF;
		filter1.FilterMaskIdLow = 0x0000;
		filter1.FilterMaskIdHigh = 0x0000;
		filter1.FilterFIFOAssignment = CAN_RX_FIFO1;
		filter1.FilterMode = CAN_FILTERMODE_IDMASK;
		filter1.FilterScale = CAN_FILTERSCALE_32BIT;
		filter1.FilterActivation = CAN_FILTER_ENABLE;
		filter1.SlaveStartFilterBank = 0;

		DART_ASSERT(HAL_CAN_ConfigFilter(handle, &filter1) == HAL_OK, DART_ERROR_HAL);
		DART_ASSERT(HAL_CAN_ActivateNotification(handle, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING) == HAL_OK, DART_ERROR_HAL);
	}
}

void hal_can_update() {
	if (canInit) {
		for (int i = 0; i < CAN_MAX_CHANNEL_COUNT; i++) {
			CAN_HandleTypeDef* handle = canHandles[i];
			if (handle == NULL || handle->State != HAL_CAN_STATE_READY)
				continue;
			DART_ASSERT(HAL_CAN_Start(handle) == HAL_OK, DART_ERROR_HAL);
		}
		canInit = false;
	}
}

void hal_can_yield() {
	if (canDirtyTX) {
		hal_can_flushTransmitBuffer_all();
		canDirtyTX = false;
	}
}

void hal_can_createTransmitMessage(CANMessage_t* message) {
	DART_NOT_USED(message);
	// Not needed
}

void hal_can_flushTransmitBuffer(int can, ringbuffer_t* ringBuffer, CANBufferItem_t* buffer) {
	CAN_HandleTypeDef* handle = canHandles[can];
	if (handle == NULL)
		return;

	hal_can_update();

	int free = HAL_CAN_GetTxMailboxesFreeLevel(handle);
	for (int i = 0; i < free; i++) {
		if (rb_isempty(ringBuffer))
			break;

		// Get index from ring buffer
		int index = rb_get(ringBuffer);

		// Write buffered CAN message to the free mailbox on the hardware
		CANBufferItem_t* item = &buffer[index];
		CAN_TxHeaderTypeDef header;
		header.StdId = item->id;
		header.DLC = item->dlc;
		header.IDE = CAN_ID_STD;
		header.RTR = CAN_RTR_DATA;
		header.TransmitGlobalTime = DISABLE;

		uint32_t mailbox;
		HAL_CAN_AddTxMessage(handle, &header, (uint8_t*)item->data, &mailbox);
	}
}

void hal_can_fillReceiveBuffer() {
	hal_can_update();

	for (int can = 0; can < CAN_MAX_CHANNEL_COUNT; can++) {
		CAN_HandleTypeDef* handle = canHandles[can];
		if (handle == NULL)
			continue;

		hal_can_fillReceiveBuffer_fifo(can, handle, CAN_RX_FIFO0);
		hal_can_fillReceiveBuffer_fifo(can, handle, CAN_RX_FIFO1);
	}
}

void hal_can_fillReceiveBuffer_fifo(int can, CAN_HandleTypeDef* handle, uint32_t fifo) {
	// See how many messages we have to handle
	int level = HAL_CAN_GetRxFifoFillLevel(handle, fifo);
	for (int i = 0; i < level; i++) {
		// Receive message
		CAN_RxHeaderTypeDef header;
		uint8_t data[CAN_MAX_DATA_SIZE];
		if (HAL_CAN_GetRxMessage(handle, fifo, &header, data) != HAL_OK)
			break;

		// Copy message to buffer
		CANBufferItem_t item;
		item.can = can;
		item.id = header.StdId;
		item.dlc = header.DLC;
		memcpy(item.data, data, sizeof(item.data));
		_dartCAN_addReceiveItem(item);
	}
}

bool can_has_error(int can) {
	DART_ASSERT_RETURN(can >= 0, DART_ERROR_INVALID_VALUE, true);
	DART_ASSERT_RETURN(can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE, true);
	CAN_HandleTypeDef* handle = canHandles[can];
	if (handle == NULL)
		return false;

	return HAL_CAN_GetState(handle) == HAL_CAN_STATE_ERROR;
}

uint32_t can_get_error_code(int can) {
	DART_ASSERT_RETURN(can >= 0, DART_ERROR_INVALID_VALUE, UINT32_MAX);
	DART_ASSERT_RETURN(can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE, UINT32_MAX);
	CAN_HandleTypeDef* handle = canHandles[can];
	if (handle == NULL)
		return 0;
	return HAL_CAN_GetError(handle);
}

void can_reset_error(int can) {
	DART_ASSERT(can >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
	CAN_HandleTypeDef* handle = canHandles[can];
	if (handle == NULL)
		return;
	HAL_CAN_ResetError(handle);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	for (int can = 0; can < CAN_MAX_CHANNEL_COUNT; can++) {
		if (hcan == canHandles[can]) {
			hal_can_fillReceiveBuffer_fifo(can, hcan, CAN_RX_FIFO0);
			break;
		}
	}
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	for (int can = 0; can < CAN_MAX_CHANNEL_COUNT; can++) {
		if (hcan == canHandles[can]) {
			hal_can_fillReceiveBuffer_fifo(can, hcan, CAN_RX_FIFO1);
			break;
		}
	}
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan) {
	DART_NOT_USED(hcan);
	canDirtyTX = true;
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* hcan) {
	DART_NOT_USED(hcan);
	canDirtyTX = true;
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* hcan) {
	DART_NOT_USED(hcan);
	canDirtyTX = true;
}


#endif
