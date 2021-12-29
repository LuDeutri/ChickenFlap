#include "../../chickenFlap/dart/canOpen.h"

canOpen_handle_t canOpen_handles[CAN_MAX_CHANNEL_COUNT];
uint8_t canOpen_syncData[1];

void canOpen_init(int can) {
	DART_ASSERT(can >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);

	// Create new CANopen handle for this CAN
	canOpen_handle_t* handle = &canOpen_handles[can];
	handle->nmt = can_createTransmitMessage(can, CANOPEN_NMT, 2);
	handle->sync = can_createTransmitMessage(can, CANOPEN_SYNC, 1);
	handle->enableSync = false;
	handle->syncInterval = createTimer(2, 0);
	handle->syncCounter = 0;
	memset(handle->nodes, 0, sizeof(handle->nodes));
	for (int i = 0; i < CANOPEN_MAX_NODE_COUNT; i++)
		handle->nodes[i].id = CANOPEN_INVALID_NODE;
}

void canOpen_update(int can) {
	canOpen_handle_t* handle = &canOpen_handles[can];
	if (handle->enableSync && hasTimerElapsed(&handle->syncInterval)) {
		handle->syncCounter++;
		can_send(handle->sync, &handle->syncCounter, sizeof(handle->syncCounter));
	}
}

void canOpen_wakeupAll(int can) {
	canOpen_wakeup(can, CANOPEN_NODE_BROADCAST);
}

void canOpen_wakeup(int can, uint8_t nodeID) {
	DART_ASSERT(can >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);

	// Send CAN message for the NMT
	uint8_t data[2];
	data[0] = 0x01; // go to operational
	data[1] = nodeID;
	can_send(canOpen_handles[can].nmt, data, sizeof(data));
}

void canOpen_createSyncInterval(int can, util_time_t interval) {
	canOpen_handle_t* handle = &canOpen_handles[can];
	handle->enableSync = true;
	handle->syncInterval = createTimer(interval, 0);
}

int canOpen_pdo1TxID(uint8_t nodeID) {
	return 0x181 + nodeID;
}
int canOpen_pdo1RxID(uint8_t nodeID) {
	return 0x201 + nodeID;
}
int canOpen_pdo2TxID(uint8_t nodeID) {
	return 0x281 + nodeID;
}
int canOpen_pdo2RxID(uint8_t nodeID) {
	return 0x301 + nodeID;
}
int canOpen_pdo3TxID(uint8_t nodeID) {
	return 0x381 + nodeID;
}
int canOpen_pdo3RxID(uint8_t nodeID) {
	return 0x401 + nodeID;
}
int canOpen_pdo4TxID(uint8_t nodeID) {
	return 0x481 + nodeID;
}
int canOpen_pdo4RxID(uint8_t nodeID) {
	return 0x501 + nodeID;
}


void canOpen_writeDictionary(int can, uint8_t nodeID, uint16_t index, uint8_t subIndex, uint8_t* data, size_t size) {
	DART_ASSERT(can >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(size <= 3, DART_ERROR_INVALID_VALUE);

	canOpen_node_t* node = _canOpen_findNode(can, nodeID);
	DART_NOT_NULL(node, DART_ERROR_INTERNAL);

	// Create SDO
	canOpen_sdo_t sdo;
	sdo.ccs = 1;
	sdo.n = (uint8_t)size;
	sdo.e = 1;
	sdo.s = 1;
	sdo.index = index;
	sdo.subIndex = subIndex;
	memcpy(sdo.data, data, size);

	// Send it over CAN
	can_send(node->sdo_writeDictionary, (uint8_t*)&sdo, sizeof(sdo));
}

void canOpen_readDictionaryInterval(int can, uint8_t nodeID, util_time_t intervalMilliseconds, uint16_t index, uint8_t subIndex, uint8_t* data, size_t size) {
	DART_ASSERT(can >= 0, DART_ERROR_INVALID_VALUE);
	DART_ASSERT(can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE);
	DART_NOT_NULL(data, DART_ERROR_INVALID_VALUE);

	canOpen_node_t* node = _canOpen_findNode(can, nodeID);
	DART_NOT_NULL(node, DART_ERROR_INTERNAL);

	for (int i = 0; i < CANOPEN_MAX_SDO_RECEIVE_ITEMS; i++) {
		canOpen_sdo_receive_item_t* item = &node->sdo_receive_items[i];
		if (item->sdo.index == 0 && item->sdo.subIndex == 0) {
			// Create SDO
			canOpen_sdo_t sdo;
			sdo.ccs = 2;
			sdo.n = 0;
			sdo.e = 0;
			sdo.s = 0;
			sdo.index = index;
			sdo.subIndex = subIndex;
			memset(sdo.data, 0, sizeof(sdo.data));

			// Create item
			item->sdo = sdo;
			item->data = data;
			item->size = size;

			// Create interval for transmit
			canData_transmit(can, CANOPEN_SDO_TX_BASE + node->id, intervalMilliseconds, (uint8_t*)&item->sdo, sizeof(item->sdo));
		}
	}
}

canOpen_node_t* _canOpen_findNode(int can, uint8_t nodeID) {
	DART_ASSERT_RETURN(can >= 0, DART_ERROR_INVALID_VALUE, NULL);
	DART_ASSERT_RETURN(can < CAN_MAX_CHANNEL_COUNT, DART_ERROR_INVALID_VALUE, NULL);
	canOpen_handle_t* handle = &canOpen_handles[can];

	// Find existing node
	for (int i = 0; i < CANOPEN_MAX_NODE_COUNT; i++)
		if (handle->nodes[i].id == nodeID)
			return &handle->nodes[i];

	// Create new node
	for (int i = 0; i < CANOPEN_MAX_NODE_COUNT; i++) {
		if (handle->nodes[i].id == CANOPEN_INVALID_NODE) {
			canOpen_node_t* node = &handle->nodes[i];
			node->id = nodeID;
			node->sdo_receive = can_createReceiveMessage(can, CANOPEN_SDO_RX_BASE + nodeID, sizeof(canOpen_sdo_t), _canOpen_readCallback, node);
			return node;
		}
	}

	return NULL;
}

void _canOpen_readCallback(const CANMessage_t message, const uint8_t* data, const size_t size, const void* arg) {
	DART_NOT_NULL(data, DART_ERROR_INTERNAL);
	DART_NOT_NULL(arg, DART_ERROR_INTERNAL);

	canOpen_sdo_t* sdo = (canOpen_sdo_t*)data;
	DART_ASSERT(size == sizeof(canOpen_sdo_t), DART_ERROR_INVALID_VALUE);

	// Find the correct node and check if the ID matches the received message
	canOpen_node_t* node = (canOpen_node_t*)arg;
	DART_ASSERT(CANOPEN_SDO_RX_BASE + node->id == message.id, DART_ERROR_INTERNAL);

	for (int i = 0; i < CANOPEN_MAX_SDO_RECEIVE_ITEMS; i++) {
		canOpen_sdo_receive_item_t* item = &node->sdo_receive_items[i];
		// Correct item?
		if (item->sdo.index != sdo->index || item->sdo.subIndex != sdo->subIndex)
			continue;

		// Check that data fits
		DART_NOT_NULL(item->data, DART_ERROR_INVALID_VALUE);
		DART_ASSERT(item->size == sdo->n, DART_ERROR_INVALID_VALUE);

		// Copy data
		memcpy(item->data, sdo->data, item->size);
	}
}
