/*
 * Here are functions to overwatch the battery
 *
 * Author: Luca Deutrich
 * Date: 26.12.2021
 */

#ifndef COMPONENTS_BMS_H_
#define COMPONENTS_BMS_H_

#include "config.h"
#include "statemachine.h"
#include "flap.h"
#include "../dart/hal_wrapper/hal_wrapper.h"

typedef struct{
	uint8_t batteryCapapcityPercentage;
	uint16_t batteryVoltage;
} bms_t;
extern bms_t bms;

static const uint16_t batteryCapacity[22] = {
	0x00U,	0x19U,	0x57U,	0x96U,	//1.55V, 1.5V, 1.45V, 1.4V
	0x11FU,	0x219U, 0x3C2U, 0x520U,	//1.35V, 1.3V, 1.25V, 1.2V
	0x6D6,	0x7D0U, 0x84D,	0x88BU,	//1.15V, 1.1V, 1.05V, 1V
	0x8FC,	0x947U, 0x9B7U, 0x9F6U, //0.95V, 0.9V, 0.85V, 0.8V
	0xA66U, 0xAA5,	0xACAU, 0xAF0,	//0.75V, 0.7V, 0.65V, 0.6V
	0xB15U, 0xB54U					//0.55V, 0.5V
	};

// Loop values used to calculate an averaged battery capacity value
extern uint8_t loopRound;
extern uint32_t tmpSum;

void bms_init();
void bms_update();

/*
 * Measure battery Voltage about using an ADC input port
 * of the MCU and calculate it so that the voltage will
 * safe in the bms struct in mV.
 */
void readBatteryVoltage();

/*
 * Calculate the Capacity with the given batteryVoltage
 * and the OCV curve, safed in the lookUp-Table 'batteryCapacity'
 */
void getBatteryCapacityInPercentage();

/*
 * ------------Actually not used--------------
 *
 * Round given value for the lookUpTable
 * @param values about 1800 or below 500 will result in an error. The return would be 0.
 */
uint16_t roundForTable(uint16_t value);

#endif
