#include "../../chickenFlap/dart/lin.h"


LIN_bus_t dartLIN_init(int uart, int nslpPin) {
	LIN_bus_t linBus;
	linBus.uart = uart;
	linBus.nslpPin = nslpPin;
	delayMillis(10); // wait till transceiver is enabled

	digitalWrite(nslpPin, HIGH);
	delayMillis(1);
	dartLIN_transceiverWakeup(&linBus);
	return linBus;
}

void dartLIN_transceiverWakeup(LIN_bus_t* bus) {
	DART_NOT_NULL(bus, DART_ERROR_INVALID_VALUE);

	// Turn on transceiver and enable NSLP to force into normal slope mode
	digitalWrite(bus->nslpPin, LOW);
	#ifdef __NOP
	    __NOP(); __NOP();
    #endif
	digitalWrite(bus->nslpPin, HIGH);
}

void dartLIN_busWakeUp(LIN_bus_t* bus) {
	// send anything and wait for reply
}

uint8_t dartLIN_calculateFrameID(uint8_t frameIdWithoutParity) {
	uint8_t frameIdBits[8];
	for (int i = 0; i < 8; i++)
		frameIdBits[i] = (frameIdWithoutParity & (1 << i)) != 0 ? 1 : 0;

	uint8_t parity0 = frameIdBits[0] ^ frameIdBits[1] ^ frameIdBits[2] ^ frameIdBits[3];
	uint8_t parity1 = frameIdBits[4] ^ frameIdBits[5] ^ frameIdBits[6] ^ frameIdBits[7];
	return (uint8_t)(frameIdWithoutParity | (parity0 << 6) | (parity1 << 7));
}

bool dartLIN_sendData(LIN_bus_t* bus, uint8_t frameIdWithParity, uint8_t* data, size_t dataSize, bool enhancedChecksum) {
	DART_NOT_NULL_RETURN(bus, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(dataSize > 0 && dataSize <= LIN_MAX_DATA, DART_ERROR_INVALID_VALUE, false);

	// Wakeup
	dartLIN_transceiverWakeup(bus);

	// Sync break
	if (!dartUART_lin_syncBreak(bus->uart))
		return false;

	// Sync data
	dartUART_transmit_byte(bus->uart, LIN_SYNC_DATA);

	// ID
	dartUART_transmit_byte(bus->uart, frameIdWithParity);
	
	// Data
	for (size_t i = 0; i < dataSize; i++)
		dartUART_transmit_byte(bus->uart, data[i]);

	// Checksum
	if (enhancedChecksum)
		dartUART_transmit_byte(bus->uart, dartLIN_calculateChecksumEnhanced(frameIdWithParity, data, dataSize));
	else
		dartUART_transmit_byte(bus->uart, dartLIN_calculateChecksumClassic(data, dataSize));
	return true;
}

bool dartLIN_receiveData(LIN_bus_t* bus, uint8_t frameIdWithParity, uint8_t* data, size_t dataSize, bool enhancedChecksum) {
	DART_NOT_NULL_RETURN(bus, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);
	DART_ASSERT_RETURN(dataSize > 0 && dataSize <= LIN_MAX_DATA, DART_ERROR_INVALID_VALUE, false);

	memset(data, 0, dataSize);

	// Wakeup
	dartLIN_transceiverWakeup(bus);

	// Sync break
	if (!dartUART_lin_syncBreak(bus->uart))
		return false;

	// Sync data
	dartUART_transmit_byte(bus->uart, LIN_SYNC_DATA);

	// ID
	dartUART_transmit_byte(bus->uart, frameIdWithParity);

	// Wait till data size and checksum byte is received
	util_time_t timeout = millis();
	while(dartUART_readBytesAvailable(bus->uart) < dataSize + 1) {
		if (millis() - timeout > LIN_RX_TIMEOUT_TIME)
			return false;
	}

	// Read data
	size_t dataReceived = dartUART_read(bus->uart, data, dataSize);
	if (dataReceived != dataSize)
		return false;

	// Read checksum
	uint8_t checksum;
	if (dartUART_read(bus->uart, &checksum, 1) != 1)
		return false;

	return true;
}

uint8_t dartLIN_calculateChecksumClassic(uint8_t* data, size_t dataSize) {
	int checksum = 0;
	for (size_t i = 0; i < dataSize; i++)
		checksum = (checksum + data[i]) % 256;
	return (uint8_t)(255 - checksum);
}

uint8_t dartLIN_calculateChecksumEnhanced(int8_t frameIdWithParity, uint8_t* data, size_t dataSize) {
	int checksum = frameIdWithParity;
	for (size_t i = 0; i < dataSize; i++)
		checksum = (checksum + data[i]) % 256;
	return (uint8_t)(255 - checksum);
}

