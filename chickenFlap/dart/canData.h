#ifndef DART_CANDATA_H_
#define DART_CANDATA_H_

#include "../../chickenFlap/dart/can.h"
#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/hash.h"
#include "../../chickenFlap/dart/util/heartbeat.h"
#include "../../chickenFlap/dart/util/timer.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CANDATA_FASTPATH_MAP_SIZE
#define CANDATA_FASTPATH_MAP_SIZE 64
#endif

#ifndef CANDATA_FASTPATH_BUCKET_SIZE
#define CANDATA_FASTPATH_BUCKET_SIZE 4
#endif

//#define CANDATA_USE_LAX_DLC_CHECK

/**
 * Dirty handle, can be used to check if new data has received on any data field and a attached heartbeat to check if the data is up-to-date.
 */
typedef struct {
	/**
	 * New data has arrived.
	 */
	bool dirty;
	/**
	 * Heartbeat which is reset when new data is received.
	 */
	heartbeat_t heartbeat;
} CANData_Dirty_t;

typedef struct {
	CANMessage_t message;

	uint8_t multiplexerByte;
	size_t itemSize;

	uint8_t* data;
	size_t size;

	CANData_Dirty_t* dirtyHandle;
} CANData_ReceiveHandle_t;

/**
 * Handle for a CAN message which is sent in an interval.
 */
typedef struct {
    /**
     * If this transmit handle is activated.
     */
    bool enabled;

	/**
	 * The timer for the interval.
	 */
	util_timer_t timer;
	/**
	 * The CAN message which is used to sent the data.
	 */
	CANMessage_t message;
	/**
	 * The pointer to the data which is sent inside the CAN message.
	 */
	uint8_t* data;
	/**
	 * The size of the data in bytes.
	 */
	size_t size;
} CANData_TransmitHandle_t;

/**
 * Maximum number of receive handles that can be created by canData_receive(). See CANData_ReceiveHandle_t.
 */
#ifndef CANDATA_RECEIVE_COUNT
#define CANDATA_RECEIVE_COUNT 16
#endif

/**
 * Maximum number of transmit handles that can be created by canData_transmit(). See CANData_TransmitHandle_t.
 */
#ifndef CANDATA_TRANSMIT_COUNT
#define CANDATA_TRANSMIT_COUNT 16
#endif

/**
 * Initializes the CAN data module.
 * MUST be called before any other CAN data function.
 */
void dartCANData_init();

/**
 * Updates the CAN data module.
 * The CAN data module MUST be initialized see dartCANData_init().
 */
void dartCANData_update();

/**
 * Setups a receive callback that copies the received data into data.
 * @param can The CAN channel where the message should be received.
 * @param id The ID of the CAN message.
 * @param data The pointer to the buffer where the data should be copied to.
 * @param size The size of the buffer where the data is copied to.
 * @param dirtyHandle Pointer to an dirty handle that may be updated when a message is received. Set this to NULL when no such handle should be used.
 * The CAN data module MUST be initialized see dartCANData_init().
 */
void canData_receive(int can, uint16_t id, void* data, size_t size, CANData_Dirty_t* dirtyHandle);

/**
 * Setups a receive callback that copies the received data into data. The data is multiplexed, that means, the data is copied
 * at an offset to the data buffer. The offset is calculated by multiplying the byte saved in the received CAN message at the index
 * multiplexerByte by the itemSize:
 * data + receivedData[multiplexerByte] * itemSize.
 * It is ensured that data is not copied over the buffer boundaries specified by bufferSize.
 *
 * @param can The CAN channel where the message should be received.
 * @param id The ID of the CAN message.
 * @param multiplexerByte The byte of the CAN message the data is multiplexed to.
 * @param itemSize The size of one item.
 * @param data The pointer to the buffer where the data should be copied to.
 * @param size The size of the buffer where the data is copied to.
 * @param dirtyHandle Pointer to an dirty handle that may be updated when a message is received. Set this to NULL when no such handle should be used.
 * The CAN data module MUST be initialized see dartCANData_init().
 */
void canData_receiveMultiplexed(int can, uint16_t id, uint8_t multiplexerByte, size_t itemSize, void* data, size_t bufferSize, CANData_Dirty_t* dirtyHandle);

/**
 * Setups a transmit timers that copies the data and sends it to the CAN.
 * @param can The CAN channel where the message will be transmitted.
 * @param id The ID of the CAN message.
 * @param intervalMilliseconds The interval in milliseconds how often the data should be transmitted.
 * @param data The pointer to the buffer where the data should be copied from.
 * @param size The size of the buffer where the data is copied from.
 * The CAN data module MUST be initialized see dartCANData_init().
 */
void canData_transmit(int can, uint16_t id, util_time_t intervalMilliseconds, void* data, size_t size);

/**
 * Clears all callbacks either receiving or transmiting on the given CAN channel and the given message ID.
 */
void canData_stop(int can, uint16_t id);

/**
 * Enables or disables the sending of the CAN message with the given ID on the given CAN bus.
 */
void canData_set_enable_transmit(int can, uint16_t id, bool enabled);

#ifdef __cplusplus
}
#endif

#endif
