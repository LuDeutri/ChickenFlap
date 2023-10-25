#pragma once

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../../chickenFlap/dart/highlevel/BinaryHelper.h"

// When the platform does not have a real 8 bit type, the packet buffer class is highly broken
#ifndef DEVICE_HAS_FAKE_INT8

#define PACKETBUFFER_ENABLED

/**
 * Implements a buffer that can be used to construct packets sent over the network.
 */
class PacketBuffer {
private:
	/**
	 * Pointer to the internal buffer.
	 */
	uint8_t* data;
	/**
	 * Size of the internal buffer data.
	 */
	size_t bufferSize;

	/**
	 * Current position where data is written or read inside the internal buffer.
	 */
	uint32_t position;
	/**
	 * Size of the packet that is inside the buffer.
	 */
	uint32_t size;

	/**
	 * If any error occured. Is reset by getError().
	 */
	bool error;
	/**
	 * If the packet buffer is allowed to be read.
	 */
	bool allowRead;

	/**
	 * Checks if the given pointer is not NULL.
	 */
	bool assertNull(void* pointer);

	/**
	 * Checks if reading is allowed in this packet buffer.
	 */
	bool assertRead();

	/**
	 * Checks if the position is valid for the given length.
	 * @param length
	 * @return
	 */
	bool assertPosition(size_t length);

	/**
	 * Checks if the position is valid for the given length and we are allowed to read.
	 * @param length
	 * @return
	 */
	bool assertPositionRead(size_t length);
	bool assertPositionWrite(size_t length);

	/**
	 * Advances the position in the buffer.
	 * @param length
	 * @return
	 */
	uint32_t addPosition(size_t length);

	/**
	 * Called when any memory allocation failed.
	 */
	void errorMemory();

public:
	explicit PacketBuffer(size_t size);
	explicit PacketBuffer(uint8_t* data, size_t size);
	~PacketBuffer();

	/**
	 * Returns true when any error occured during the construction of any packet.
	 * Also the error is reset with this function.
	 */
	bool getError();

	/**
	 * Sets the internal array pointer to the given data.
	 */
	void setBuffer(uint8_t* data, size_t size);

	/**
	 * Returns the pointer to the buffer used.
	 */
	uint8_t* getBuffer() const;
	/**
	 * Returns the size of the buffer.
	 */
	size_t getBufferSize() const;

	/**
	 * Returns the current position inside the buffer.
	 */
	uint32_t getPosition() const;
	void seek(uint32_t offset);
	void resetPosition();

	void reset();

	/**
	 * Returns the size of the packet.
	 */
	uint32_t getSize() const;
	/**
	 * Sets the size of the packet.
	 */
	void setSize(uint32_t size);

	bool readBoolean();
	int8_t readInt8();
	uint8_t readUint8();
	int16_t readInt16();
	uint16_t readUint16();
	int32_t readInt32();
	uint32_t readUint32();
	int64_t readInt64();
	uint64_t readUint64();

	float readFloat();
	double readDouble();

	void read(uint8_t* buffer, size_t length);
	void read(uint8_t* buffer, size_t length, uint32_t offset);
	void read(char* buffer, size_t length, uint32_t offset);

	uint8_t* getBufferRegion(size_t size);

	char* readString();


	void writeBoolean(bool value);
	void write(char value);
#ifndef DEVICE_HAS_FAKE_INT8
	void write(int8_t value);
	void write(uint8_t value);
#endif
	void write(int16_t value);
	void write(uint16_t value);
	void write(int32_t value);
	void write(uint32_t value);
	void write(int64_t value);
	void write(uint64_t value);

	void write(float value);
	void write(double value);

	void write(uint8_t* buffer, size_t length);
	void write(uint8_t* buffer, size_t length, uint32_t offset, size_t bufferSize);

	void writeString(char* str);
	void writeString(const char* str);
};
#endif
