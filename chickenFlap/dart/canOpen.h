/**
 * Implementation for the CANopen protocol.
 */

#ifndef DART_CANOPEN_H_
#define DART_CANOPEN_H_

#include "../../chickenFlap/dart/can.h"
#include "../../chickenFlap/dart/canData.h"
#include "../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../chickenFlap/dart/util/error.h"
#include "../../chickenFlap/dart/util/math.h"
#include "../../chickenFlap/dart/util/ringbuffer.h"
#include "../../chickenFlap/dart/util/time.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * CAN ID for the NMT message.
 */
#define CANOPEN_NMT 0x00
/**
 * CAN ID for the Sync message.
 */
#define CANOPEN_SYNC 0x80

/**
 * Base CAN ID for the SDO transmit.
 */
#define CANOPEN_SDO_TX_BASE 0x580

/**
 * Base CAN ID for the SDO receive.
 */
#define CANOPEN_SDO_RX_BASE 0x600

/**
 * Node ID for broadcast.
 */
#define CANOPEN_NODE_BROADCAST 0

/**
 * Maximum CANopen node id.
 */
#define CANOPEN_MAX_NODE_ID 127

/**
 * Maximum number of nodes the CANopen module will support.
 */
#define CANOPEN_MAX_NODE_COUNT 8

/**
 * Constant for an invalid node ID.
 */
#define CANOPEN_INVALID_NODE 255

/**
 * Specificies the maximum number of SDO receive items for one CANopen node.
 */
#define CANOPEN_MAX_SDO_RECEIVE_ITEMS 16

/**
 * Defines function ReceiveCallback that is called when a CAN message is received.
 * @param message The CANMessage the callback is called for.
 * @param data Pointer to the data that was received.
 * @param size Length of data in bytes.
 */
typedef void (*SDOReceiveCallback)(CANMessage_t message, uint8_t* data, size_t size);

/**
 * CAN message data structure for the Service Data Object CAN message.
 */
#pragma pack(push, 1)
typedef struct {
	uint8_t ccs : 3;
	bool : 1;
	uint8_t n : 2;
	bool e : 1;
	bool s : 1;
	uint16_t index;
	uint8_t subIndex;
	uint8_t data[4];
} canOpen_sdo_t;
#pragma pack(pop)

/**
 * Internal structure for one SDO object which is being received.
 */
typedef struct {
	canOpen_sdo_t sdo;
	uint8_t* data;
	size_t size;
} canOpen_sdo_receive_item_t;

/**
 * Internal structure for one CANopen node.
 */
typedef struct {
	uint8_t id;
	CANMessage_t sdo_writeDictionary;
	CANMessage_t sdo_receive;
	canOpen_sdo_receive_item_t sdo_receive_items[CANOPEN_MAX_SDO_RECEIVE_ITEMS];
} canOpen_node_t;



/**
 * Internal structure to save data for each CAN bus.
 */
typedef struct {
	CANMessage_t nmt;
	bool enableSync;
	CANMessage_t sync;
	util_timer_t syncInterval;
	uint8_t syncCounter;
	canOpen_node_t nodes[CANOPEN_MAX_NODE_COUNT];
} canOpen_handle_t;


/**
 * Initializes the CANopen module.
 * Needs to be called before any other CANopen function.
 */
void canOpen_init(int can);

/**
 * Updates the CANopen module.
 */
void canOpen_update(int can);

/**
 * Sends the message to wake up every CANopen node on the specified CAN network.
 */
void canOpen_wakeupAll(int can);
/**
 * Sends the message to wake up one specified CANopen node with the given node ID on the specified CAN network.
 */
void canOpen_wakeup(int can, uint8_t nodeID);
/**
 * Creates a period sending for the sync message to all nodes on the specified CAN network. This will trigger their sending of their PDOs if they are configured so.
 */
void canOpen_createSyncInterval(int can, util_time_t interval);

/**
 * Calculates the CAN message ID for the PDO1 transmit.
 */
int canOpen_pdo1TxID(uint8_t nodeID);
/**
 * Calculates the CAN message ID for the PDO1 receive.
 */
int canOpen_pdo1RxID(uint8_t nodeID);
/**
 * Calculates the CAN message ID for the PDO2 transmit.
 */
int canOpen_pdo2TxID(uint8_t nodeID);
/**
 * Calculates the CAN message ID for the PDO2 receive.
 */
int canOpen_pdo2RxID(uint8_t nodeID);
/**
 * Calculates the CAN message ID for the PDO3 transmit.
 */
int canOpen_pdo3TxID(uint8_t nodeID);
/**
 * Calculates the CAN message ID for the PDO3 receive.
 */
int canOpen_pdo3RxID(uint8_t nodeID);
/**
 * Calculates the CAN message ID for the PDO4 transmit.
 */
int canOpen_pdo4TxID(uint8_t nodeID);
/**
 * Calculates the CAN message ID for the PDO4 receive.
 */
int canOpen_pdo4RxID(uint8_t nodeID);


/**
 * Writes to any CANopen node using the SDO message.
 */
void canOpen_writeDictionary(int can, uint8_t nodeID, uint16_t index, uint8_t subIndex, uint8_t* data, size_t size);

/**
 * Reads from any CANopen node using the SDO message using a periodic task. The data is copied from the CAN message
 * into the specified buffer at data with the given size.
 */
void canOpen_readDictionaryInterval(int can, uint8_t nodeID, util_time_t intervalMilliseconds, uint16_t index, uint8_t subIndex, uint8_t* data, size_t size);

/**
 * Finds or creates the canOpen_node_t for the specified nodeID on the specified CAN.
 */
canOpen_node_t* _canOpen_findNode(int can, uint8_t nodeID);

/**
 * Internal callback for any SDO message was received.
 */
void _canOpen_readCallback(const CANMessage_t message, const uint8_t* data, const size_t size, const void* arg);

#ifdef __cplusplus
}
#endif

#endif
