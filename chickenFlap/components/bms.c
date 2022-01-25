#include "bms.h"

void bms_init(){
	bms.batteryCapapcityPercentage = 0;
	bms.batteryVoltage = 0;

	loopRound = 0;
	tmpSum = 0;
}

void bms_update(){
	// Dont read voltage if motor is running (voltage drop)
	if(flap.lastTimeMotorRuns + 10000 > millis())
		return;

	readBatteryVoltage();
	getBatteryCapacityInPercentage();
}

void readBatteryVoltage() {
	// Sum of sampled values to calculate an averaged one
	tmpSum += getADC(ADC_BATTERY_VOLTAGE)*((ADC_VOLTAGE_DIVIDER_R1+ADC_VOLTAGE_DIVIDER_R2)/ADC_VOLTAGE_DIVIDER_R2);
	loopRound++;

	// Check if enough values are sampled
	if(loopRound < 50)
		return;

	// Calculate averaged value
	bms.batteryVoltage = tmpSum / loopRound;
	// Reset loop values
	loopRound = 0;
	tmpSum = 0;
}

void getBatteryCapacityInPercentage(){
	uint16_t cellVoltage = bms.batteryVoltage / CELL_NUMBER_9V_BLOCK;

	if (cellVoltage > 1800) // Check for valid value
		nextState(STATE_ERROR);
	else if (cellVoltage < 500) // Check for an empty battery
		bms.batteryCapapcityPercentage = 0;
	else if (cellVoltage >= 1550) // Check for full Battery
		bms.batteryCapapcityPercentage = 100;
	else{
		bms.batteryCapapcityPercentage = (uint8_t)((float)(1-((float)(batteryCapacity[abs((cellVoltage/50)-31)]) / CELL_MAX_CAPACITY))*100);

	}
}

uint16_t roundForTable(uint16_t value) {

	if (value > CELL_MAX_VOLTAGE || value < 500) // Check for valid value or an empty battery
		return 0;
	else if (value > 1550) // Make value valid if its a little bit to high
		return 1550;

	uint8_t modValue = value % 5;
	if (modValue == 0) // Return value if its not needed to round
		return value;
	else
		return value -= modValue; // Round down to the next valid value
}
