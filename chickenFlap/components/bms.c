#include "bms.h"

bms_t bms;

// Loop values used to calculate an averaged battery capacity value
uint8_t loopRound;
uint32_t tmpSum;

void bms_init(){
	bms.soc = 0;
	bms.adcBatteryVoltage = 0;

	loopRound = 0;
	tmpSum = 0;
}

void bms_update(){
	// If no battery is used --> exit
	#ifndef USE_BATTERY
		return;
	#endif

	// Dont read voltage if motor is running (voltage drop)
	if(flap.lastTimeMotorRuns + 10000 > millis())
		return;

	readBatteryVoltage();
	calculateSOC();
}

int voltage;
void readBatteryVoltage() {
	// Sum of sampled values to calculate an averaged one


	tmpSum += (getADC(ADC_BATTERY_VOLTAGE)*((ADC_VOLTAGE_DIVIDER_R1+ADC_VOLTAGE_DIVIDER_R2)/ADC_VOLTAGE_DIVIDER_R2))*BMS_VOLATAGE_CORRECTION_FACTOR;
	loopRound++;

	// Check if enough values are sampled
	if(loopRound < 20)
		return;

	// Calculate averaged value
	bms.adcBatteryVoltage = tmpSum / loopRound;
	// Reset loop values
	loopRound = 0;
	tmpSum = 0;
}

void calculateSOC(){
	// Check that an adc value is available
	if(bms.adcBatteryVoltage == 0){
		bms.soc = 0;
		return;
	}
	// Calculate cell voltage
	uint16_t cellVoltage = bms.adcBatteryVoltage / CELL_NUMBER_12V_CAR_BATTERY;

	// Check cell voltage for plausibilty
	if (cellVoltage > CELL_OVERVOLTAGE_CAR_BATTERY || cellVoltage < CELL_UNDERVOLTAGE_CAR_BATTERY){ // Check for valid value
		#ifdef ENABLE_BATTERY_ERR_CHECK
			error.emptyBattery = true;
		#endif
	} else if (cellVoltage <= ocvCarBattery[0]) // Check for an empty battery
		bms.soc = 0;
	else if (cellVoltage >= ocvCarBattery[100]) // Check for full Battery
		bms.soc = 100;
	else
		// Get table value for cell voltage value
		bms.soc = findNearestValueIdentifier(cellVoltage);
}

uint8_t findNearestValueIdentifier(uint16_t cellVoltage) {
    uint16_t nearestValue = ocvCarBattery[0];
    uint8_t identifier = 0;
    uint16_t minDifference = abs(cellVoltage - nearestValue);

    // Search the next value for the measured ADC battery voltage and return the identifier of that value
    for (int i = 1; i < TABLE_SIZE_OCV_CAR_BATTERY; i++) {
        uint32_t difference = abs(cellVoltage - ocvCarBattery[i]);
        if (difference < minDifference) {
            minDifference = difference;
            nearestValue = ocvCarBattery[i];
            identifier = i;
        }
    }
    return identifier;
}
