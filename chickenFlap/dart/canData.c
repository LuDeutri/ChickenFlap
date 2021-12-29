#include "../../chickenFlap/dart/canData.h"

int receiveHandleCount = 0;
CANData_ReceiveHandle_t receiveHandles[CANDATA_RECEIVE_COUNT];

int transmitHandleCount = 0;
CANData_TransmitHandle_t transmitHandles[CANDATA_TRANSMIT_COUNT];

#ifdef CANDATA_ENABLE_FAST_PATH
bool canData_fastPath_filled[CAN_MAX_CHANNEL_COUNT][CANDATA_FASTPATH_MAP_SIZE];
CANData_ReceiveHandle_t* canData_fastPath_map[CAN_MAX_CHANNEL_COUNT][CANDATA_FASTPATH_MAP_SIZE][CANDATA_FASTPATH_BUCKET_SIZE];
#endif

void dartCANData_init() {
#ifdef CANDATA_ENABLE_FAST_PATH
	memset(canData_fastPath_filled, false, sizeof(canData_fastPath_filled));
	memset(canData_fastPath_map, 0, sizeof(canData_fastPath_map));
#endif
}

void dartCANData_update() {
	for (int i = 0; i < transmitHandleCount; i++) {
		// Find handles where the timer has elapsed
		CANData_TransmitHandle_t* handle = &transmitHandles[i];
		if (!hasTimerElapsed(&handle->timer))
			continue;

		// Is transmit handles activated?
		if (!handle->enabled)
		    continue;

		// Send the message
		can_send(handle->message, handle->data, handle->size);
	}
}

void _canData_receiver(const CANMessage_t message, const uint8_t* data, const size_t size, const void* arg) {
	DART_NOT_NULL(data, DART_ERROR_INTERNAL);
	DART_NOT_NULL(arg, DART_ERROR_INTERNAL);


	CANData_ReceiveHandle_t* handle = (CANData_ReceiveHandle_t*)arg;
	DART_ASSERT(handle->message.can == message.can && handle->message.id == message.id, DART_ERROR_INTERNAL);

	#ifdef CANDATA_USE_LAX_DLC_CHECK
		DART_ASSERT(size >= handle->size, DART_ERROR_INTERNAL);
	#else
		DART_ASSERT(size == handle->size, DART_ERROR_INTERNAL);
	#endif

	// Copy data over
	if (unlikely(handle->itemSize > 0)) { // handle multiplexed item
		uint8_t signal = data[handle->multiplexerByte];
		int offset = signal * handle->itemSize;

		DART_ASSERT((offset + handle->itemSize) >= handle->size, DART_ERROR_BUFFER_OVERFLOW);
		memcpy(handle->data + offset, data, size);
	}
	else { // handle simple message

		memcpy(handle->data, data, handle->size);
	}

	// Update the dirty handle when any is provided
	if (handle->dirtyHandle != NULL) {
		handle->dirtyHandle->dirty = true;
		resetHeartbeat(&handle->dirtyHandle->heartbeat);
	}
}

bool canData_fast_path(const int can, const uint16_t id, const uint8_t* data, const size_t size) {
#ifndef CANDATA_ENABLE_FAST_PATH
	DART_NOT_USED(can);
	DART_NOT_USED(id);
	DART_NOT_USED(data);
	DART_NOT_USED(size);
	return false;
#else
	// Find the receive handle in the map
	int index = hash_uint16(id) % CANDATA_FASTPATH_MAP_SIZE;
	if (!canData_fastPath_filled[can][index])
		return false;

	CANData_ReceiveHandle_t** bucket = canData_fastPath_map[can][index];
	for (int i = 0; i < CANDATA_FASTPATH_BUCKET_SIZE; i++) {
		CANData_ReceiveHandle_t* item = bucket[i];
		if (item != NULL && item->message.id == id) {
			// Call the normal handler
			CANMessage_t message;
			message.can = can;
			message.id = id;
			message.dataLength = size;
			_canData_receiver(message, data, size, item);

			// Message was handled
			return true;
		}
	}
	// Nothing found -> do it the slow way
	return false;
#endif
}

void canData_receive(int can, uint16_t id, void* data, size_t size, CANData_Dirty_t* dirtyHandle) {
	DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(can >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(size > 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(size <= CAN_MAX_DATA_SIZE, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(receiveHandleCount < CANDATA_RECEIVE_COUNT, DART_ERROR_BUFFER_OVERFLOW);

	CANData_ReceiveHandle_t* handle = &receiveHandles[receiveHandleCount++];
	handle->data = (uint8_t*)data;
	handle->size = size;
	handle->multiplexerByte = 0;
	handle->itemSize = 0;
	handle->message = can_createReceiveMessage(can, id, (uint8_t)size, &_canData_receiver, handle);
	handle->dirtyHandle = dirtyHandle;

	if (dirtyHandle != NULL)
		dirtyHandle->dirty = false;

	memset(data, 0, size);

#ifdef CANDATA_ENABLE_FAST_PATH
	// Prepare fastpath
	int index = hash_uint16(id) % CANDATA_FASTPATH_MAP_SIZE;
	canData_fastPath_filled[can][index] = true;

	CANData_ReceiveHandle_t** bucket = canData_fastPath_map[can][index];
	for (int i = 0; i < CANDATA_FASTPATH_BUCKET_SIZE; i++) {
		CANData_ReceiveHandle_t* item = bucket[i];
		if (item == NULL) {
			bucket[i] = handle;
			break;
		}
	}
#endif
}

void canData_receiveMultiplexed(int can, uint16_t id, uint8_t multiplexerByte, size_t itemSize, void* data, size_t bufferSize, CANData_Dirty_t* dirtyHandle) {
	DART_ASSERT(itemSize > 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(itemSize <= CAN_MAX_DATA_SIZE, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(multiplexerByte < itemSize, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(bufferSize > 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(receiveHandleCount < CANDATA_RECEIVE_COUNT, DART_ERROR_BUFFER_OVERFLOW);

	CANData_ReceiveHandle_t* handle = &receiveHandles[receiveHandleCount++];
	handle->data = (uint8_t*)data;
	handle->size = bufferSize;
	handle->multiplexerByte = multiplexerByte;
	handle->itemSize = itemSize;
	handle->message = can_createReceiveMessage(can, id, (uint8_t)itemSize, &_canData_receiver, handle);
	handle->dirtyHandle = dirtyHandle;

	memset(data, 0, bufferSize);
}

void canData_transmit(int can, uint16_t id, util_time_t intervalMilliseconds, void* data, size_t size) {
	DART_ASSERT(intervalMilliseconds > 0, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(size > 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(size <= CAN_MAX_DATA_SIZE, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(transmitHandleCount < CANDATA_TRANSMIT_COUNT, DART_ERROR_INVALID_VALUE);

	// Check if already a handle was added with the same CAN and ID
	for (int i = 0; i < transmitHandleCount; i++)
		DART_ASSERT(transmitHandles[i].message.can != can || transmitHandles[i].message.id != id, DART_ERROR_ID_ALREADY_IN_USE);

	// Create handle
	CANData_TransmitHandle_t handle;
	handle.enabled = true;
	handle.timer = createTimer(intervalMilliseconds, rand() % 1000);
	handle.data = data;
	handle.size = size;
	handle.message = can_createTransmitMessage(can, id, (uint8_t)size);

	transmitHandles[transmitHandleCount++] = handle;

	// Clear data
	memset(data, 0, size);
}

void canData_stop(int can, uint16_t id) {
	for (int i = 0; i < transmitHandleCount; i++) {
		CANData_TransmitHandle_t* handle = &transmitHandles[i];
		if (handle->message.can == can && handle->message.id == id)
			handle->enabled = false;
	}
}

void canData_set_enable_transmit(int can, uint16_t id, bool enabled) {
    for (int i = 0; i < transmitHandleCount; i++) {
        CANData_TransmitHandle_t* handle = &transmitHandles[i];
        if (handle->message.can == can && handle->message.id == id)
            handle->enabled = enabled;
    }
}
