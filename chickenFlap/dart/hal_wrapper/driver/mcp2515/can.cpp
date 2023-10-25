#include "../../../../../chickenFlap/dart/hal_wrapper/driver/mcp2515/can.h"

#if defined(HAL_ENABLE_CAN_MCP2515)
int mcp2515_spi[CAN_MAX_CHANNEL_COUNT] = MCP2515_SPI_MAP;
int mcp2515_cs[CAN_MAX_CHANNEL_COUNT] = MCP2515_PIN_CS_MAP;
CAN_Baudrate_t mcp2515_baudrate[CAN_MAX_CHANNEL_COUNT] = MCP2515_CAN_BAUDRATE;
MCP2515* mcp2515_can[CAN_MAX_CHANNEL_COUNT];

#ifdef HAL_ENABLE_MCP2515_THREAD
QueueHandle_t transmitQueue[CAN_MAX_CHANNEL_COUNT];
QueueHandle_t receiveQueue[CAN_MAX_CHANNEL_COUNT];
#endif

void hal_can_init() {
	dartSPI_init();

	// Reset any CAN objects
	memset(mcp2515_can, 0, sizeof(mcp2515_can));

	// Create every MCP2515 object
	for (int i = 0; i < CAN_MAX_CHANNEL_COUNT; i++) {
		int spi = mcp2515_spi[i];
		int cs = mcp2515_cs[i];
		if (spi < 0 || cs < 0)
			continue;

		MCP2515* mcp = new MCP2515(spi, cs);
		DART_NOT_NULL(mcp, DART_ERROR_HAL);

		mcp->reset();
		mcp->setBitrate((CAN_SPEED)mcp2515_baudrate[i], MCP_8MHZ);
		mcp->setNormalMode();

		mcp2515_can[i] = mcp;
		delayMillis(20);

		#ifdef HAL_ENABLE_MCP2515_THREAD
			transmitQueue[i] = xQueueCreate(10, sizeof(CANBufferItem_t));
			receiveQueue[i] = xQueueCreate(10, sizeof(CANBufferItem_t));
		#endif
	}

	// Start task
#ifdef HAL_ENABLE_MCP2515_THREAD
	TaskHandle_t handle;
	xTaskCreate(vTaskCAN, "task_can", 512, NULL, 3, &handle);
#endif
}

void hal_can_update() {
	// Not needed
}

void hal_can_set_baudrate(const int can, const CAN_Baudrate_t baudrate) {
	DART_NOT_IMPLEMENTED();
}

void hal_can_createTransmitMessage(CANMessage_t* message) {
	// Not needed
}

void hal_can_flushTransmitBuffer(int can, ringbuffer_t* ringBuffer, CANBufferItem_t* buffer) {
	MCP2515* mcp = mcp2515_can[can];
	if (mcp == NULL)
		return;

	for (int i = 0; i < 3; i++) {
		if (rb_isempty(ringBuffer))
			return;

		int index = rb_peek(ringBuffer);
		CANBufferItem_t* item = &buffer[index];

#ifdef HAL_ENABLE_MCP2515_THREAD
		if (xQueueSend(transmitQueue[can], item, 0))
			return;

		rb_get(ringBuffer); // remove it from buffer
#else
		// Try to send it
		if (!hal_can_mcp2515_send(mcp, item))
			return;

		rb_get(ringBuffer); // remove it from buffer
#endif
	}
}

void hal_can_fillReceiveBuffer() {
	for (int can = 0; can < CAN_MAX_CHANNEL_COUNT; can++) {
		MCP2515* mcp = mcp2515_can[can];
		if (mcp == NULL)
			continue;

#ifdef HAL_ENABLE_MCP2515_THREAD
		CANBufferItem_t item;
		while (xQueueReceive(receiveQueue[can], &item, 0))
			_dartCAN_addReceiveItem(item);
#else
		CANBufferItem_t item;
		if (hal_can_mcp2515_receive(can, mcp, &item))
			_dartCAN_addReceiveItem(item);
#endif
	}
}

bool hal_can_mcp2515_send(MCP2515* mcp, CANBufferItem_t* item) {
	DART_NOT_NULL_RETURN(mcp, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(item, DART_ERROR_INVALID_VALUE, false);

	struct can_frame frame;
	frame.can_id = item->id;
	frame.can_dlc = item->dlc;
	memcpy(frame.data, item->data, sizeof(frame.data));

	// Try to send it
	return mcp->sendMessage(&frame) == MCP2515::ERROR_OK;
}
bool hal_can_mcp2515_receive(int can, MCP2515* mcp, CANBufferItem_t* item) {
	DART_NOT_NULL_RETURN(mcp, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(item, DART_ERROR_INVALID_VALUE, false);
	struct can_frame frame;
	if (mcp->readMessage(&frame) != MCP2515::ERROR_OK)
		return false;

	// Copy message to item
	item->can = can;
	item->id = frame.can_id;
	item->dlc = frame.can_dlc;
	memcpy(item->data, frame.data, sizeof(item->data));
	return true;
}

#ifdef HAL_ENABLE_MCP2515_THREAD
void vTaskCAN(void* pvParameters) {
	while (1) {
		for (int i = 0; i < 1; i++) {
			MCP2515* mcp = mcp2515_can[i];
			if (mcp == NULL)
				continue;

			CANBufferItem item;
			if (xQueueReceive(transmitQueue[i], &item, 0))
				hal_can_mcp2515_send(mcp, &item);

			if (hal_can_mcp2515_receive(i, mcp, &item))
				xQueueSend(receiveQueue[i], &item, 0);
		}
	}
}
#endif
#endif
