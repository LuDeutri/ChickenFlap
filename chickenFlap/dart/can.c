#include "../../chickenFlap/dart/can.h"

/**
 * Array for the callbacks.
 */
CANReceiveCallback_t receiveCallbacks[CAN_MAX_RECEIVE_CALLBACKS];

/**
 * Counter that is incremented when a new ReceiveCallback is added to receiveCallbacks.
 */
int receiveCallbackCounter = 0;

/**
 * Current index of the receiveBuffer.
 */
int receiveBufferIndex = 0;

/**
 * Current index of each transmitBuffer.
 */
ringbuffer_t transmitBufferRB[CAN_MAX_CHANNEL_COUNT];

/**
 * Buffer for the transmit message boxes.
 */
CANBufferItem_t transmitBuffer[CAN_TRANSMIT_BUFFER_SIZE * CAN_MAX_CHANNEL_COUNT];

/**
 * Buffer for the received CAN messages.
 */
CANBufferItem_t receiveBuffer[CAN_RECEIVE_BUFFER_SIZE];

void dartCAN_init() {
    // Initialize callbacks
    receiveCallbackCounter = 0;

    // Receive buffer
    for (uint8_t i = 0; i < CAN_MAX_RECEIVE_CALLBACKS; i++)
        receiveCallbacks[i].message.id = CAN_INVALID_ID;
    receiveBufferIndex = 0;

    // Transmit buffer
    memset(transmitBuffer, 0, sizeof(transmitBuffer));
    for (int i = 0; i < CAN_MAX_CHANNEL_COUNT; i++)
    	transmitBufferRB[i] = rb_create(CAN_TRANSMIT_BUFFER_SIZE);

    hal_can_init();
}

void dartCAN_update() {
	dartCAN_readReceived();
	dartCAN_flushTransmit();

	// Call callbacks
    for (uint8_t i = 0; i < receiveBufferIndex; i++) {
        CANBufferItem_t* item = &receiveBuffer[i];

        // Find the receive callback that is used for this id.
        for (uint8_t j = 0; j < receiveCallbackCounter; j++) {
            CANReceiveCallback_t callback = receiveCallbacks[j];
            if (callback.message.id == item->id)
                callback.function(callback.message, item->data, item->dlc, callback.arg);
        }
    }
    receiveBufferIndex = 0;
}

void dartCAN_yield() {
	hal_can_yield();
}

void dartCAN_readReceived() {
	hal_can_update();
	hal_can_fillReceiveBuffer();
}

void dartCAN_flushTransmit() {
	hal_can_update();
	hal_can_flushTransmitBuffer_all();
}

void hal_can_flushTransmitBuffer_all() {
	for (int i = 0; i < CAN_MAX_CHANNEL_COUNT; i++)
		hal_can_flushTransmitBuffer(i, &transmitBufferRB[i], _dartCAN_getTransmitBuffer(i));
}

void can_set_baudrate(const int can, const CAN_Baudrate_t baudrate) {
	DART_ASSERT(can >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
	hal_can_set_baudrate(can, baudrate);
}

CANMessage_t _dartCAN_createMessage(const int can, const uint16_t id, const uint8_t dataLength) {
    CANMessage_t message = {0};
    DART_ASSERT_RETURN(can >= 0, DART_ERROR_INVALID_VALUE, message);
    DART_ASSERT_RETURN(can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE, message);
    DART_ASSERT_RETURN(id <= CAN_MAX_ID, DART_ERROR_INVALID_VALUE, message);
    DART_ASSERT_RETURN(id != CAN_INVALID_ID, DART_ERROR_INVALID_VALUE, message);
    DART_ASSERT_RETURN(dataLength <= CAN_MAX_DATA_SIZE, DART_ERROR_INVALID_VALUE, message);

    // Construct message structure
    message.can = can;
    message.id = id;
    message.dataLength = dataLength;
    return message;
}

void _dartCAN_addReceiveItem(const CANBufferItem_t item) {
	// Use CAN data fast path
	if (canData_fast_path(item.can, item.id, item.data, item.dlc))
		return;

	// Check if enough space
	DART_ASSERT(receiveBufferIndex < CAN_RECEIVE_BUFFER_SIZE, DART_ERROR_BUFFER_OVERFLOW);

	// Copy message to buffer
	receiveBuffer[receiveBufferIndex++] = item;
}

bool can_anyMessageInReceivedBuffer() {
	return receiveBufferIndex > 0;
}

CANBufferItem_t can_popReceivedMessage() {
	CANBufferItem_t empty;
	memset(&empty, 0, sizeof(empty));
	DART_ASSERT_RETURN(receiveBufferIndex > 0, DART_ERROR_INVALID_VALUE, empty);

	return receiveBuffer[--receiveBufferIndex];
}

CANBufferItem_t* _dartCAN_getTransmitBuffer(const int can) {
	return transmitBuffer + can * CAN_TRANSMIT_BUFFER_SIZE;
}

CANMessage_t can_createReceiveMessage(const int can, const uint16_t id, const uint8_t dataLength, const ReceiveCallback callbackFunction, void* arg) {
    CANMessage_t message = _dartCAN_createMessage(can, id, dataLength);
    DART_NOT_NULL_RETURN(callbackFunction, DART_ERROR_INTERNAL, message);

    // Array receiveCallbacks is full
    DART_ASSERT_RETURN(receiveCallbackCounter < CAN_MAX_RECEIVE_CALLBACKS, DART_ERROR_BUFFER_OVERFLOW, message);

    // Construct structures
    CANReceiveCallback_t callback;
    callback.message = message;
    callback.arg = arg;
    callback.function = callbackFunction;

    // Add to receiveCallbacks
    receiveCallbacks[receiveCallbackCounter++] = callback;
    return message;
}

CANMessage_t can_createTransmitMessage(const int can, const uint16_t id, const uint8_t dataLength) {
    CANMessage_t msg = _dartCAN_createMessage(can, id, dataLength);
    hal_can_createTransmitMessage(&msg);
    return msg;
}


void can_send(const CANMessage_t message, const uint8_t* data, size_t size) {
	DART_ASSERT(message.can >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(message.can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
    DART_NOT_NULL(data, DART_ERROR_INTERNAL);
#ifdef CAN_SEND_USE_LAX_DLC_CHECK
    DART_ASSERT(size >= message.dataLength, DART_ERROR_INVALID_VALUE);
#else
    DART_ASSERT(size == message.dataLength, DART_ERROR_INVALID_VALUE);
#endif

	// Copy message to buffer
	ringbuffer_t* rb = &transmitBufferRB[message.can];
	if (rb_isfull(rb))
		return;

	CANBufferItem_t* item = &_dartCAN_getTransmitBuffer(message.can)[rb_put(rb)];
	item->can = message.can;
	item->id = message.id;
	item->dlc = message.dataLength;
	item->mailbox = message.mailbox;
	memcpy(item->data, data, sizeof(item->data));
}
