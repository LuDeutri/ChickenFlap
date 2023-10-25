#ifndef DART_DRIVER_MAX17261_H_
#define DART_DRIVER_MAX17261_H_

#include "../../../chickenFlap/dart/i2c.h"

// I2C Adresses
#define MAX17261_I2C_ADRESS_0 0x36 	// for 7 Msb adress
#define MAX17261_I2C_ADRESS_1 0x6C

//Bit position
#define AT_RATE_EN 13
#define DPEN 12

// Configuration Registers
#define MAX17261_REG_DESIGN_CAP 0x18
#define MAX17261_REG_V_EMPTY 0x3A
#define MAX17261_REG_MODEL_CFG 0xDB
#define MAX17261_REG_I_CHG_TERM 0x1E
#define MAX17261_REG_CONFIG 0x1D
#define MAX17261_REG_CONFIG2 0xBB

// Intern operation Registers
#define MAX17261_REG_FSTAT 0x3D
#define MAX17261_REG_R_COMP0 0x38
#define MAX17261_REG_TEMP_CO 0x39
#define MAX17261_REG_FULL_CAP_NOM 0x23
// Output Registers
#define MAX17261_REG_REP_CAP 0x05
#define MAX17261_REG_REP_SOC 0x06
#define MAX17261_REG_FULL_CAP_REP 0x10
#define MAX17261_REG_TTE 0x11 			// Time to empty
#define MAX17261_REG_TTF 0x20 			// Time to full
#define MAX17261_REG_CYCLES 0x17
#define MAX17261_REG_STATUS 0x00

// Voltage Registers
#define MAX17261_REG_VOLT_CELL 0x09
#define MAX17261_REG_VOLT_CELL_AVG 0x19
#define MAX17261_REG_VOLT_MAX_MIN 0x1B

// Current Registers
#define MAX17261_REG_CURRENT 0x0A
#define MAX17261_REG_CURRENT_AVG 0x0B
#define MAX17261_REG_CURRENT_MAX_MIN 0x1C

// Temperatures Registers
#define MAX17261_REG_TEMP 0x08
#define MAX17261_REG_TEMP_AVG 0x16
#define MAX17261_REG_TEMP_MAX_MIN 0x1A
#define MAX17261_REG_TEMP_DIE 0x034

#define MAX17261_POWER 0xB1
#define MAX17261_POWER_AVG 0xB1

// Alert Registers
#define MAX17261_REG_ALERT_VOLT_TH 0x01
#define MAX17261_REG_ALERT_TEMP_TH 0x02
#define MAX17261_REG_ALERT_SOC_TH 0x03
#define MAX17261_REG_ALERT_CURRENT_TH 0xB4

// Serial Number Format
// Config2.AtRateEn = 1 || Config2.DPEn = 1
#define MAX17261_POWER_PEAK_MAX 0xD4
#define MAX17261_POWER_PEAK_SUS 0xD5
#define MAX17261_CURRENT_MPP 0xD9
#define MAX17261_CURRENT_SPP 0xDA
#define MAX17261_ATQ_RESIDUAL 0xDC
#define MAX17261_AT_TTE 0xDD
#define MAX17261_AT_AV_SOC 0xDE
#define MAX17261_AT_AV_CAP 0xDF
// Config2.AtRateEn = 0 && Config2.DPEn = 0
#define MAX17261_SERIAL_NUMBER_WORD_0 0xD4
#define MAX17261_SERIAL_NUMBER_WORD_1 0xD5
#define MAX17261_SERIAL_NUMBER_WORD_2 0xD9
#define MAX17261_SERIAL_NUMBER_WORD_3 0xDA
#define MAX17261_SERIAL_NUMBER_WORD_4 0xDC
#define MAX17261_SERIAL_NUMBER_WORD_5 0xDD
#define MAX17261_SERIAL_NUMBER_WORD_6 0xDE
#define MAX17261_SERIAL_NUMBER_WORD_7 0xDF

// Config values
#define MAX17261_SCALE_CAPACITY 0.000005f 	// Vh
#define MAX17261_SCALE_PERCENTAGE 256 		// %
#define MAX17261_SCALE_VOLTAGE 0.000078125f 	// V
#define MAX17261_SCALE_CURRENT 0.0000015625f	// V
#define MAX17261_SCALE_TEMPERATURE 256 		// °C
#define MAX17261_SCALE_RESISTANCE 4096 		// Ohm
#define MAX17261_SCALE_TIME 5.625f 			// Seconds


typedef struct {
	// Output registers
	uint16_t sid[8];	// Serial identification number
	uint16_t repCap;	// Battery Capacity mAh
	uint8_t repSOC;		// Battery Capacity %
	uint16_t tte;		// Time to empty
} max17261_t;

typedef struct {			// Must be set
	// Device parameters
	uint16_t nomCellCapacity;
} max17261_parameters_t;

typedef struct {			// Do not set
	// learned parameters
	uint16_t rComp0;
	uint16_t tempCo;
	uint16_t fullCapRep;
	uint16_t cycles;
	uint16_t fullCapNom;
} max17261_learned_t;

typedef struct {			// must be set
	int bus;
	uint8_t adress;
	uint16_t chargeVoltage; // mV
	uint16_t sid[8];	// Serial identification number
	max17261_t regs;
	max17261_parameters_t paramter;
	max17261_learned_t learned;
} max17261_device_t;


bool max17261_init(max17261_device_t* device);
void max17261_update(max17261_device_t* device);
void max17261_checkReset(max17261_device_t* device);
void max17261_restoreLearnedParameters(max17261_device_t* device);

uint16_t max17261_readRegister(max17261_device_t* device, uint8_t regAdr);
void writeAndVerifyRegister(max17261_device_t* device, uint8_t regAdr, uint16_t data);

bool max17261_write16(max17261_device_t* device, uint8_t regAdr, uint16_t data);

bool max17261_read(max17261_device_t* device, uint8_t regAdr, uint8_t* data);
bool max17261_read_serialID(max17261_device_t* device);
bool max17261_read_capacity(max17261_device_t* device, uint8_t* capacityArr, size_t size);
bool max17261_read_voltage(max17261_device_t* device, uint8_t* value, size_t size);
bool max17261_read_temperature(max17261_device_t* device, uint8_t* value, size_t size);
bool max17261_read_current(max17261_device_t* device, uint8_t* value, size_t size);

#endif
