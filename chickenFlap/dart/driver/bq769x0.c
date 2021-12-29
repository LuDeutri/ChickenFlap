#include "../../../chickenFlap/dart/driver/bq769x0.h"

uint8_t bqBuffer[128];

const unsigned int OVPThreshold = 4370;
const unsigned int UVPThreshold = 3000;
const unsigned char SCDDelay = SCD_DELAY_200us;
const unsigned char SCDThresh = SCD_THRESH_178mV_89mV;
const unsigned char OCDDelay = OCD_DELAY_640ms;
const unsigned char OCDThresh = OCD_THRESH_78mV_39mV;
const unsigned char OVDelay = OV_DELAY_1s;
const unsigned char UVDelay = UV_DELAY_1s;

bool bq_I2CSendBytes(bq_device_t* device, unsigned char* buffer, size_t length)
{
	i2c_begin(device->i2c);
	if (!i2c_write(device->i2c, device->address, buffer, length))
		return false;
	i2c_end(device->i2c);
	return true;
}

bool bq_I2CWriteBlockWithCRC(bq_device_t* device, uint8_t registerAddress, uint8_t* data, size_t length)
{
	DART_ASSERT_RETURN(2 * length + 1 <= sizeof(bqBuffer), DART_ERROR_BUFFER_OVERFLOW, false);

	// Used for first  CRC checksum
	uint8_t tempBuffer[3];
	tempBuffer[0] = device->address << 1;
	tempBuffer[1] = registerAddress;
	tempBuffer[2] = data[0];

	bqBuffer[0] = registerAddress;
	bqBuffer[1] = data[0];
	bqBuffer[2] = crc8(tempBuffer, 3, CRC_KEY);
	for (size_t i = 1; i < length; i++) {
		bqBuffer[1 + i * 2] = data[i];
		bqBuffer[2 + i * 2] = crc8(data + i, 1, CRC_KEY);
	}

	return bq_I2CSendBytes(device, bqBuffer, 2 * length + 1);
}


bool bq_I2CWriteBlock(bq_device_t* device, uint8_t registerAddress, uint8_t* data, size_t length)
{
	if (device->useCRC)
		return bq_I2CWriteBlockWithCRC(device, registerAddress, data, length);
	else {
		DART_ASSERT_RETURN(length + 1 <= sizeof(bqBuffer), DART_ERROR_BUFFER_OVERFLOW, false);
		bqBuffer[0] = registerAddress;
		memcpy(bqBuffer + 1, data, length);

		return bq_I2CSendBytes(device, bqBuffer, length + 1);
	}
}

bool bq_I2CReadBytes(bq_device_t* device, unsigned char* data, size_t length)
{
	i2c_begin(device->i2c);
	if (!i2c_read(device->i2c, device->address, data, length))
		return false;
	i2c_end(device->i2c);
	return true;
}

bool bq_I2CReadBlockWithCRC(bq_device_t* device, uint8_t registerAddress, uint8_t* data, size_t length)
{
	if (!bq_I2CSendBytes(device, &registerAddress, 1))
		return false;

	if (!bq_I2CReadBytes(device, bqBuffer, length * 2))
		return false;

	uint8_t buf[2];
	buf[0] = (device->address << 1) + 1;
	buf[1] = bqBuffer[0];
	if (crc8(buf, 2, CRC_KEY) != bqBuffer[1])
		return false;

	data[0] = bqBuffer[0];
	for (size_t i = 1; i < length; i++) {
		data[i] = bqBuffer[i * 2];
		if (crc8(data + i, 1, CRC_KEY) != bqBuffer[i * 2 + 1])
			return false;
	}

	return true;
}

bool bq_I2CReadBlock(bq_device_t* device, uint8_t registerAddress, uint8_t* data, size_t length)
{
	if (device->useCRC)
		return bq_I2CReadBlockWithCRC(device, registerAddress, data, length);
	else {
		if (!bq_I2CSendBytes(device, &registerAddress, 1))
			return false;
		return bq_I2CReadBytes(device, data, length);
	}
}

bool bq_initialise(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);

	device->registers.Protect1.Protect1Bit.RSNS = true;
	device->registers.Protect1.Protect1Bit.SCD_DELAY = SCDDelay;
	device->registers.Protect1.Protect1Bit.SCD_THRESH = SCDThresh;
	device->registers.Protect2.Protect2Bit.OCD_DELAY = OCDDelay;
	device->registers.Protect2.Protect2Bit.OCD_THRESH = OCDThresh;
	device->registers.Protect3.Protect3Bit.OV_DELAY = OVDelay;
	device->registers.Protect3.Protect3Bit.UV_DELAY = UVDelay;

	if (!bq_readADCGainOffset(device))
		return false;

	device->gain = ADCGAIN_BASE + ((device->registers.ADCGain1.ADCGain1Bit.ADCGAIN_4_3 << 2) | device->registers.ADCGain2.ADCGain2Bit.ADCGAIN_2_0);

	device->registers.OVTrip = (((int)((OVPThreshold - device->registers.ADCOffset) / (device->gain / 1000.0)) >> 4) & 0xFF);
	device->registers.UVTrip = (((int)((UVPThreshold - device->registers.ADCOffset) / (device->gain / 1000.0)) >> 4) & 0xFF);
	device->registers.CCCfg = 0x19;

	return bq_updateProtect(device);
}

bool bq_shutdown(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);

	device->registers.SysCtrl1.SysCtrl1Bit.SHUT_A = 0;
	device->registers.SysCtrl1.SysCtrl1Bit.SHUT_B = 0;
	if (!bq_updateControl(device))
		return false;

	device->registers.SysCtrl1.SysCtrl1Bit.SHUT_A = 0;
	device->registers.SysCtrl1.SysCtrl1Bit.SHUT_B = 1;
	if (!bq_updateControl(device))
		return false;

	device->registers.SysCtrl1.SysCtrl1Bit.SHUT_A = 1;
	device->registers.SysCtrl1.SysCtrl1Bit.SHUT_B = 0;
	return bq_updateControl(device);
}

bool bq_clearStatus(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);

	device->registers.SysStatus.StatusByte = 0xFF;
	if (!bq_updateStatus(device))
		return false;
	return bq_readStatus(device);
}

bool bq_clearCCReady(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);

	device->registers.SysStatus.StatusByte = 0;
	device->registers.SysStatus.Status.CC_READY = true;
	if (!bq_updateStatus(device))
		return false;
	return bq_readStatus(device);
}

bool bq_updateStatus(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
	return bq_I2CWriteBlock(device, SYS_STAT, &device->registers.SysStatus.StatusByte, 1);
}

bool bq_readStatus(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
	return bq_I2CReadBlock(device, SYS_STAT, &device->registers.SysStatus.StatusByte, 1);
}

bool bq_updateBalancing(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
	return bq_I2CWriteBlock(device, CELLBAL1, &device->registers.CellBal1.CellBal1Byte, 3);
}

bool bq_readControl(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
	return bq_I2CReadBlock(device, SYS_CTRL1, &device->registers.SysCtrl1.SysCtrl1Byte, 2);
}

bool bq_updateControl(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
	return bq_I2CWriteBlock(device, SYS_CTRL1, &device->registers.SysCtrl1.SysCtrl1Byte, 2);
}

bool bq_updateProtect(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);

	unsigned char bqMaximoProtectionConfig[6];

	if (!bq_I2CWriteBlock(device, PROTECT1, &device->registers.Protect1.Protect1Byte, sizeof(bqMaximoProtectionConfig)))
		return false;
	if (!bq_I2CReadBlock(device, PROTECT1, bqMaximoProtectionConfig, sizeof(bqMaximoProtectionConfig)))
		return false;

	return bqMaximoProtectionConfig[0] == device->registers.Protect1.Protect1Byte
		&& bqMaximoProtectionConfig[1] == device->registers.Protect2.Protect2Byte
		&& bqMaximoProtectionConfig[2] == device->registers.Protect3.Protect3Byte
		&& bqMaximoProtectionConfig[3] == device->registers.OVTrip
		&& bqMaximoProtectionConfig[4] == device->registers.UVTrip;
}

bool bq_readADCGainOffset(bq_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
	if (!bq_I2CReadBlock(device, ADCGAIN1, &device->registers.ADCGain1.ADCGain1Byte, 2))
		return false;
	return bq_I2CReadBlock(device, ADCGAIN2, &device->registers.ADCGain2.ADCGain2Byte, 1);
}


bool bq_readVoltages(bq_device_t* device, float* cellVoltages, size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
	DART_ASSERT_RETURN(size >= sizeof(float) * 15, DART_ERROR_INVALID_VALUE, false);

	if (!bq_I2CReadBlock(device, VC1_HI_BYTE, &device->registers.VCell1.VCell1Byte.VC1_HI, 30))
		return false;

	unsigned long lTemp = 0;
	unsigned char* pRawADCData = &device->registers.VCell1.VCell1Byte.VC1_HI;
	for (int i = 0; i < 15; i++)
	{
		unsigned int iTemp = (unsigned int)(*pRawADCData << 8) + *(pRawADCData + 1);
		lTemp = ((unsigned long)iTemp * device->gain)/1000;
		lTemp += device->registers.ADCOffset;
		cellVoltages[i] = lTemp / 1000.0f; // V
		pRawADCData += 2;
	}

	return true;
}

bool bq_readTemperatures(bq_device_t* device, float* temperatures, size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);
	DART_ASSERT_RETURN(size >= sizeof(float) * 3, DART_ERROR_INVALID_VALUE, false);

	if (!bq_I2CReadBlock(device, TS1_HI_BYTE, &device->registers.TS1.TS1Byte.TS1_HI, 6))
		return false;

	unsigned long lTemp;
	unsigned char *pRawADCData = &device->registers.TS1.TS1Byte.TS1_HI;
	for (int i = 0; i < 3; i++)
	{
		unsigned int iTemp = (unsigned int)(*pRawADCData << 8) + *(pRawADCData + 1);
		lTemp = ((unsigned long)iTemp * 382)/1000;
		lTemp -= 1200;
		temperatures[i] = 25.0f - (lTemp / 1000.0f) * 0.0042f; // °C
		pRawADCData += 2;
	}

	return true;
}

bool bq_readCharge(bq_device_t* device, float* charge) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_VALUE_IS_NULL, false);

	if (!bq_I2CReadBlock(device, REG_CC_HI, (unsigned char*)(&device->registers.CC.CCWord), 2))
		return false;
	*charge = ((device->registers.CC.CCByte.CC_HI << 8) | device->registers.CC.CCByte.CC_LO) * 8.44f;
	return true;
}
