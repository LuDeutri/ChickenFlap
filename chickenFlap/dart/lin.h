#ifndef DART_LIN_H_
#define DART_LIN_H_

/** LIN PACKET:
 * 	LIN 1.x --> Classic Checksum
 *  It consist of:
 *  ___________ __________ _______ ____________ _________
 * |           |          |       |            |         |
 * |Sync Break |Sync Byte |ID byte| Data Bytes |Checksum |
 * |___________|__________|_______|____________|_________|
 *
 * Every byte have start bit and stop bit and it is send LSB first.
 * Synch Break - 13 bits of dominant state ("0"), followed by 1 bit recessive state ("1")
 * Synch Byte - Byte for Bound rate synchronization, always 0x55
 * ID Byte - consist of parity, length and address; parity is determined by LIN standard and depends from address and message length
 * Data Bytes - user defined; depend on devices on LIN bus
 * Checksum - inverted 256 checksum; data bytes are summed up and then inverted
 */

#include "../../chickenFlap/dart/uart.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIN_SYNC_DATA 		0x55
#define LIN_MAX_DATA 		8
#define LIN_RX_TIMEOUT_TIME	1

// LIN bus
typedef struct {
	int uart;
	int nslpPin;
} LIN_bus_t;

LIN_bus_t dartLIN_init(int uart, int nslpPin);
void dartLIN_transceiverWakeup(LIN_bus_t* bus);
void dartLIN_busWakeup(LIN_bus_t* bus);

uint8_t dartLIN_calculateFrameID(uint8_t frameIdWithoutParity);

bool dartLIN_sendData(LIN_bus_t* bus, uint8_t frameIdWithParity, uint8_t* data, size_t dataSize, bool enhancedChecksum);
bool dartLIN_receiveData(LIN_bus_t* bus, uint8_t frameIdWithParity, uint8_t* data, size_t dataSize, bool enhancedChecksum);

uint8_t dartLIN_calculateChecksumClassic(uint8_t* data, size_t dataSize);
uint8_t dartLIN_calculateChecksumEnhanced(int8_t frameIdWithParity, uint8_t* data, size_t dataSize);

#ifdef __cplusplus
}
#endif

#endif
