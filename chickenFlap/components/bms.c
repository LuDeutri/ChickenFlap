#include "bms.h"

void bms_init(){
	bms.batteryCapapcityPercentage = 0;
	bms.batteryVoltage = 0;
}

void bms_update(){
	readBatteryVoltage();
	getBatteryCapacityInPercentage();
}

void readBatteryVoltage() {
	// Read ADC value and calculate the voltage with the formula of the datasheet
	bms.batteryVoltage = (uint16_t)(VOLTAGE_REFERENCE/ADC_MAX_VALUE)*getADC(ADC_BATTERY_VOLTAGE);
}

void getBatteryCapacityInPercentage(){
	uint16_t cellVoltage = bms.batteryVoltage / (BATTERY_MAX_CELL_CAPACITY/1500);

	if (cellVoltage > 1800) // Check for valid value
		nextState(STATE_ERROR);
	else if (cellVoltage < 500) // Check for an empty battery
		bms.batteryCapapcityPercentage = 0;
	else if (cellVoltage >= 1550) // Check for full Battery
		bms.batteryCapapcityPercentage = 100;
	else
		bms.batteryCapapcityPercentage = (uint8_t)((1-batteryCapacity[roundForTable(abs((cellVoltage/50)-31))] / BATTERY_MAX_CELL_CAPACITY))*100;
}

uint16_t roundForTable(uint16_t value) {

	if (value > BATTERY_MAX_CELL_VOLTAGE || value < 500) // Check for valid value or an empty battery
		return 0;
	else if (value > 1550) // Make value valid if its a little bit to high
		return 1550;

	uint8_t modValue = value % 5;
	if (modValue == 0) // Return value if its not needed to round
		return value;
	else
		return value -= modValue; // Round down to the next valid value
}
