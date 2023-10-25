#pragma once

#include "../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"
#include "../../../chickenFlap/dart/util/motorola.h"

#ifndef DEVICE_HAS_FAKE_INT8

#define BINARY_NEED_SWAPPING IS_INTEL_DEVICE

class BinaryHelper
{
public:
	static void changeByteOrder(uint8_t* buffer, int32_t offset, size_t length);

	static void writeInt16(uint8_t* buf, int32_t offset, int16_t val);
	static int16_t readInt16(uint8_t* buf, int32_t offset);

	static void writeInt32(uint8_t* buf, int32_t offset, int32_t val);
	static int32_t readInt32(uint8_t* buf, int32_t offset);

	static void writeInt64(uint8_t* buf, int32_t offset, int64_t val);
	static int64_t readInt64(uint8_t* buf, int32_t offset);


	static void writeUint16(uint8_t* buf, int32_t offset, uint16_t val);
	static uint16_t readUint16(uint8_t* buf, int32_t offset);

	static void writeUint32(uint8_t* buf, int32_t offset, uint32_t val);
	static uint32_t readUint32(uint8_t* buf, int32_t offset);

	static void writeUint64(uint8_t* buf, int32_t offset, uint64_t val);
	static uint64_t readUint64(uint8_t* buf, int32_t offset);

	static void writeFloat(uint8_t* buf, int32_t offset, float val);
	static float readFloat(uint8_t* buf, int32_t offset);

	static void writeDouble(uint8_t* buf, int32_t offset, double val);
	static double readDouble(uint8_t* buf, int32_t offset);
};
#endif
