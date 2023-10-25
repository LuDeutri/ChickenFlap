/**
 * Wrapper around the CAN HAL.
 */

#ifndef DART_CAN_H_
#define DART_CAN_H_

/**
 * Maximum of CAN channels the CAN module supports.
 */
#ifndef CAN_MAX_CHANNEL_COUNT
#define CAN_MAX_CHANNEL_COUNT 3
#endif

/**
 * Defines the constant for the max data length in bytes.
 */
#define CAN_MAX_DATA_SIZE 8

/**
 * Defines the maximum CAN ID that can by used.
 * Includes also the CAN_INVALID_ID.
 */
#define CAN_MAX_ID 0x7FF

/**
 * Defines an CAN ID that is used by the firmware to indicate an invalid id. MUST NOT be used by any CANMessage.
 */
#define CAN_INVALID_ID 0x7FF

#include "../../chickenFlap/dart/standard_include.h"
#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

/**
 * Defines structure for a message sent or received by CAN.
 */
typedef struct CANMessage {
	int can;		    	// CAN Index
    uint16_t id;        	// ID for the CAN message
    uint8_t dataLength; 	// Length of the data in bytes that is sent or excepted to get received.
    uint32_t mailbox;		// Mailbox for this CAN message, may not be used
} CANMessage_t;

#include "../../chickenFlap/dart/util/error.h"
#include "../../chickenFlap/dart/util/math.h"
#include "../../chickenFlap/dart/util/ringbuffer.h"
#include "../../chickenFlap/dart/can_baudrate.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CAN_MAX_RECEIVE_CALLBACKS
/**
 * Defines the maximum amount of receive callbacks that can be created by createReceiveMessage.
 */
#define CAN_MAX_RECEIVE_CALLBACKS 64
#endif

#ifndef CAN_TRANSMIT_BUFFER_SIZE
/**
 * Maximum number of transmitted CAN message that are buffered.
 */
#define CAN_TRANSMIT_BUFFER_SIZE 32
#endif


#ifndef CAN_RECEIVE_BUFFER_SIZE
/**
 * Maximum number of received CAN message that are buffered.
 */
#define CAN_RECEIVE_BUFFER_SIZE 64
#endif

#ifndef CAN_TIMEOUT
/**
 * Constant for time in milliseconds the CAN module waits for any blocking operation. When the timeout is reached the blocking operation is stopped.
 */
#define CAN_TIMEOUT 50
#endif

//#define CAN_SEND_USE_LAX_DLC_CHECK

/**
 * Defines structure for the buffering of CAN data in any buffer structure.
 */
typedef struct CANBufferItem {
	int can;							// CAN index of the CAN message received
    uint16_t id;                        // ID of the CAN message received
    uint16_t dlc;						// Data length of the CAN message in bytes
    uint8_t data[CAN_MAX_DATA_SIZE];    // Data that was received
    uint32_t mailbox;                   // Mailbox (may not be used)
} CANBufferItem_t;

/**
 * Defines function ReceiveCallback that is called when a CAN message is received.
 * @param message The CANMessage the callback is called for.
 * @param data Pointer to the data that was received.
 * @param size Length of data in bytes.
 * @param arg Argument given by user when creating the callback.
 */
typedef void (*ReceiveCallback)(const CANMessage_t message, const uint8_t* data, const size_t size, const void* arg);

/**
 * Defines structure for saving the message and the receive callback.
 */
typedef struct CANReceiveCallback {
    CANMessage_t message;     // Message that is getting received
    ReceiveCallback function; // Callback that is called when the message is received
    void* arg;				  // User provided argument for the callback
} CANReceiveCallback_t;

/**
 * Initializes the CAN HAL.
 * MUST be called before any other CAN function.
 */
void dartCAN_init();

/**
 * Updates the CAN system, i.e. calls the callbacks for the received buffered CAN messages.
 * The CAN HAL MUST be initialized see dartCAN_init().
 */
void dartCAN_update();

/**
 * This will fetch new CAN messages or sent queued message, but will not call any callbacks.
 */
void dartCAN_yield();

/**
 * Tries to receive CAN messages.
 * The CAN HAL MUST be initialized see dartCAN_init().
 */
void dartCAN_readReceived();

/**
 * Tries to flush the transmit buffer.
 * The CAN HAL MUST be initialized see dartCAN_init().
 */
void dartCAN_flushTransmit();

/**
 * Flushes the transmit buffer.
 */
void hal_can_flushTransmitBuffer_all();

/**
 * Sets the baudrate for the given CAN channel.
 */
void can_set_baudrate(const int can, const CAN_Baudrate_t baudrate);

/**
 * Internal function that creates a CANMessage with the given parameters.
 * MUST NOT be called by any other function than can_createReceiveMessage or can_createTransmitMessage.
 */
CANMessage_t _dartCAN_createMessage(const int can, const uint16_t id, const uint8_t dataLength);

/**
 * Adds a newly received CAN message to the internal buffer.
 */
void _dartCAN_addReceiveItem(const CANBufferItem_t item);

/**
 * Returns the pointer to the transmit buffer for the given can.
 */
CANBufferItem_t* _dartCAN_getTransmitBuffer(const int can);

/**
 * Returns true when any message is sitting in the received buffer.
 */
bool can_anyMessageInReceivedBuffer();

/**
 * Returns the last received message.
 */
CANBufferItem_t can_popReceivedMessage();

/**
 * Creates new message that is excepted to get received.
 * @param can The index of the CAN channel the message is received.
 * @param id The CAN ID of the message.
 * @param dataLength Length of the message to excepted to be received in bytes. MUST be smaller or equal to CAN_DATA_SIZE.
 * @param callbackFunction Callback that is called when the message is received.
 * @param arg Argument given to the callback function.
 */
CANMessage_t can_createReceiveMessage(const int can, const uint16_t id, const uint8_t dataLength, const ReceiveCallback callbackFunction, void* arg);

/**
 * Creates new message that is sent by the firmware.
 * @param id The CAN ID of the message.
 * @param dataLength Length of the message to be sent in bytes. MUST be smaller or equal to CAN_DATA_SIZE.
 */
CANMessage_t can_createTransmitMessage(const int can, const uint16_t id, const uint8_t dataLength);

/**
 * Sends the CAN message.
 * @param can The index of the CAN channel the message is sent.
 * @param message The CAN message that is sent.
 * @param data Data that is sent with the message.
 * @param size Length of data in bytes.
 * The CAN HAL MUST be initialized see dartCAN_init().
 */
void can_send(const CANMessage_t message, const uint8_t* data, size_t size);

/**
 * Returns true when the CAN as any error.
 */
bool can_has_error(int can);

/**
 * Returns the last CAN error code. (HAL specific)
 */
uint32_t can_get_error_code(int can);

/**
 * Resets the CAN.
 */
void can_reset_error(int can);

/**
 * Initializes the HAL behind the CAN module. Implemented by the HAL wrapper.
 */
void hal_can_init();

/**
 * Updates the HAL behind the CAN module. Implemented by the HAL wrapper.
 */
void hal_can_update();

/**
 * Receive or sent CAN messages. Should not call the callbacks. Implemented by the HAL wrapper.
 */
void hal_can_yield();

/**
 * Sets the baudrate for given CAN channel. Implemented by the HAL wrapper.
 */
void hal_can_set_baudrate(const int can, const CAN_Baudrate_t baudrate);

/**
 * Creates a CAN message inside the HAL. Implemented by the HAL wrapper.
 */
void hal_can_createTransmitMessage(CANMessage_t* message);

/**
 * Tries to send every CAN message in the transmit buffer. Implemented by the HAL wrapper.
 */
void hal_can_flushTransmitBuffer(int can, ringbuffer_t* ringBuffer, CANBufferItem_t* buffer);

/**
 * Pools every received CAN message from the HAL and saves it into the receive buffer. Implemented by the HAL wrapper.
 */
void hal_can_fillReceiveBuffer();

/**
 * This function is called by the CAN submodule to handle CAN messages directly when they are received, so they do not
 * get added to the receive buffer, thus saving some computation time.
 */
bool canData_fast_path(const int can, const uint16_t id, const uint8_t* data, const size_t size);

#ifdef __cplusplus
}
#endif

#endif
