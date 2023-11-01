#include "DS3231RealTimeClkModule.h"

ds3231_t timeReg;

bool ds3231_init(){
	// The DS3231 module has an battery included, so the clk is continuing if the uC is restarting.
	// To prevent that the time must set again, the inital set of the clk time should only be used in the first flash
	// of a software revision
	#ifdef FIRST_SOFTWARE_FLASH
		if(ds3231_setTime(0, 0, 12, FRIDAY, 3, 11, 23) != HAL_OK)
			return false;
	#endif

	return true;
}

bool ds3231_setTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year){
	uint8_t set_time[7];
	set_time[0] = decToBcd(sec);
	set_time[1] = decToBcd(min);
	set_time[2] = decToBcd(hour);
	set_time[3] = decToBcd(dow);
	set_time[4] = decToBcd(dom);
	set_time[5] = decToBcd(month);
	set_time[6] = decToBcd(year);

	if (HAL_I2C_Mem_Write(&hi2c4, I2C_ADDRESSE_DS3231, 0x00, 1, set_time, 7, 1000) != HAL_OK)
		return false;
	return true;
}

bool ds3231_setTimeShort(uint8_t sec, uint8_t min, uint8_t hour){
	uint8_t set_time[3];
	set_time[0] = decToBcd(sec);
	set_time[1] = decToBcd(min);
	set_time[2] = decToBcd(hour);

	if (HAL_I2C_Mem_Write(&hi2c4, I2C_ADDRESSE_DS3231, 0x00, 1, set_time, 7, 1000) != HAL_OK)
		return false;
	return true;
}

bool ds3231_readTime(){
	uint8_t get_time[7];
	if (HAL_I2C_Mem_Read(&hi2c4, I2C_ADDRESSE_DS3231, 0x00, 1, get_time, 7, 1000) != HAL_OK)
		return false;

	timeReg.second = bcdToDec(get_time[0]);
	timeReg.minute = bcdToDec(get_time[1]);
	timeReg.hour = bcdToDec(get_time[2]);
	timeReg.dayOfWeek = bcdToDec(get_time[3]);
	timeReg.dayOfMonth = bcdToDec(get_time[4]);
	timeReg.month = bcdToDec(get_time[5]);
	timeReg.year = bcdToDec(get_time[6]);

	return true;
}

bool ds3231_readTemp(float *data){
	bool tmp = true;

	tmp &= forceTempDataConv();
	tmp &= ((*data = getTemp()) != -64);

	return tmp;
}

float getTemp(){
	uint8_t temp[2];
	if (HAL_I2C_Mem_Read(&hi2c4, I2C_ADDRESSE_DS3231, 0x11, 1, temp, 2, 1000) != HAL_OK)
		return -64;
	return ((temp[0])+(temp[1]>>6)/4.0);
}

uint8_t decToBcd(int val) {
  return (uint8_t)( (val/10*16) + (val%10) );
}


int bcdToDec(uint8_t val){
  return (int)( (val/16*10) + (val%16) );
}

bool forceTempDataConv(){
	uint8_t status=0;
	uint8_t control=0;
	if (HAL_I2C_Mem_Read(&hi2c4, I2C_ADDRESSE_DS3231, 0x0F, 1, &status, 1, 100) != HAL_OK)  // read status register
		return false;

	if (!(status&0x04)) { // if the BSY bit is not set
		if (HAL_I2C_Mem_Read(&hi2c4, I2C_ADDRESSE_DS3231, 0x0E, 1, &control, 1, 100) != HAL_OK  // read control register
		|| HAL_I2C_Mem_Write(&hi2c4, I2C_ADDRESSE_DS3231, 0x0E, 1, (uint8_t *)(control|(0x20)), 1, 100) != HAL_OK)  // write modified control register with CONV bit as'1'
			return false;
	}
	return true;
}
