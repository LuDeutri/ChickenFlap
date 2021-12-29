#ifndef DART_COMMON_XI2020_LVPACK_H_
#define DART_COMMON_XI2020_LVPACK_H_

#include "../../../../chickenFlap/dart/standard_include.h"

#pragma pack(push, 1)

// CAN Transmit messages structure

typedef struct {
	bool connected : 1;
	bool charging : 1;
	bool limitsOverwrote : 1;
	bool error : 1;
	uint16_t serialID;
	uint8_t deviceNumber;
	uint8_t numberOfPacks;
	uint8_t stateStatemachine;
	uint8_t stateOfCharge;
	uint8_t capacity;
} lvPack_statusPackage_t;

typedef struct {
	uint16_t voltageDifference1; // Difference between pack 1 and pack 2
	uint16_t voltageDifference2; // Difference between pack 1 and pack 3
	uint16_t voltageDifference3; // Difference between pack 2 and pack 3
} lvPack_voltDifference_package_t;

typedef struct {
	uint16_t voltPackage_1[4];
} lvPack_voltPackage1_t;

typedef struct {
	uint16_t voltPackage_2[4];
} lvPack_voltPackage2_t;

typedef struct {
	int32_t current_ina240;
	int32_t current_max17261;
} lvPack_currentPackage_t;

typedef struct {
	int16_t tempPackage_1[4];
} lvPack_tempPackage1_t;

typedef struct {
	int16_t tempPackage_2[4];
} lvPack_tempPackage2_t;

// LTC errors
typedef struct {
	bool pec_error : 1;
	bool self_test_cell_voltages_fail : 1;
	bool self_test_aux_voltages_fail : 1;
	bool self_test_status_fail : 1;
	bool thermal_shutdown : 1;
	bool ref_voltage_wrong : 1;
	bool analog_power_supply_wrong : 1;
	bool digital_power_supply_wrong : 1;
	bool multiplexer_self_test_fail : 1;
	bool cell_voltage_error : 1;
	bool cell_voltage_any_invalid : 1;
	bool cell_voltage_invalid_0 : 1;
	bool cell_voltage_invalid_1 : 1;
	bool cell_voltage_invalid_2 : 1;
	bool cell_voltage_invalid_3 : 1;
	bool cell_voltage_invalid_4 : 1;
	bool cell_voltage_invalid_5 : 1;
	bool open_wire_error : 1;
	bool open_wire_c_0 : 1;
	bool open_wire_c_1 : 1;
	bool open_wire_c_2 : 1;
	bool open_wire_c_3 : 1;
	bool open_wire_c_4 : 1;
	bool open_wire_c_5 : 1;
	bool open_wire_c_6 : 1;
} ltc6810_err_t;

// LV pack errors
typedef struct {
	bool packsNotBalanced : 1;
	bool ltc : 1;
	bool cellOvervoltage : 1;
	bool cellUndervoltage : 1;
	bool cellOvertemperature : 1;
	bool cellUndertemperature : 1;
	bool overCurrentDischarge : 1;
	bool overCurrentCharge : 1;
	bool statemachine : 1;
	bool deviceNumber : 1;
	bool numberOfPacks : 1;
	bool parallelPackNotConnected : 1;
	bool internal : 1;
	ltc6810_err_t ltcErr;
} lvPack_error_t;


// CAN Receive message by VCU structure
typedef struct {
	bool shutdownBit0 : 1; // Shutdown config: 1001
	bool shutdownBit1 : 1;
	bool shutdownBit2 : 1;
	bool shutdownBit3 : 1;
	bool reset : 1;
	bool setLimits : 1;
	bool checkConnectionParallelPacks : 1;
	bool dontCare1 : 1;
	uint16_t limitOvervoltage;		// mV
	uint16_t limitUndervoltage;		// mV
	uint8_t limitDischarge;			// A
	uint8_t limitCharge;			// A
} lvPack_controlPackage_t;

typedef struct {
	lvPack_statusPackage_t statusPackage;
	lvPack_voltDifference_package_t voltDiffPackage;
	lvPack_voltPackage1_t voltPackage_1;
	lvPack_voltPackage2_t voltPackage_2;
	lvPack_currentPackage_t currentPackage;
	lvPack_tempPackage1_t tempPackage_1;
	lvPack_tempPackage2_t tempPackage_2;
	lvPack_controlPackage_t controlData;
	lvPack_error_t error;
} lvPack_data_t;

#define CAN_ID_LVPACK1_STATUS 0x10
#define CAN_ID_LVPACK1_VOLTAGE_1 0x102
#define CAN_ID_LVPACK1_VOLTAGE_2 0x103
#define CAN_ID_LVPACK1_CURRENT 0x104
#define CAN_ID_LVPACK1_TEMPERATURE_1 0x105
#define CAN_ID_LVPACK1_TEMPERATURE_2 0x106
#define CAN_ID_LVPACK1_ERROR 0x107
#define CAN_ID_LVPACK1_VOLTDIFF 0x108

#define CAN_ID_LVPACK2_STATUS 0x111
#define CAN_ID_LVPACK2_VOLTAGE_1 0x112
#define CAN_ID_LVPACK2_VOLTAGE_2 0x113
#define CAN_ID_LVPACK2_CURRENT 0x114
#define CAN_ID_LVPACK2_TEMPERATURE_1 0x115
#define CAN_ID_LVPACK2_TEMPERATURE_2 0x116
#define CAN_ID_LVPACK2_ERROR 0x117
#define CAN_ID_LVPACK2_VOLTDIFF 0x118

#define CAN_ID_LVPACK3_STATUS 0x121
#define CAN_ID_LVPACK3_VOLTAGE_1 0x122
#define CAN_ID_LVPACK3_VOLTAGE_2 0x123
#define CAN_ID_LVPACK3_CURRENT 0x124
#define CAN_ID_LVPACK3_TEMPERATURE_1 0x125
#define CAN_ID_LVPACK3_TEMPERATURE_2 0x126
#define CAN_ID_LVPACK3_ERROR 0x127
#define CAN_ID_LVPACK3_VOLTDIFF 0x128

#define CAN_ID_CONTROL 0x129

#pragma pack(pop)
#endif
