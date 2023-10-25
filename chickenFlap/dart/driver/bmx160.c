#include "../../../chickenFlap/dart/driver/bmx160.h"

bool bmx160_read_register(bmx160_device_t* device, uint8_t addr, uint8_t* data, size_t size) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	if (!spi_begin(device->bus))
		return false;
	digitalWrite(device->cs, LOW);

	addr |= 0x80;

	bool ret = true;
	ret &= spi_write(device->bus, &addr, sizeof(addr));
	ret &= spi_read(device->bus, data, size);
	digitalWrite(device->cs, HIGH);
	ret &= spi_end(device->bus);
	return ret;
}

bool bmx160_write_register(bmx160_device_t* device, uint8_t addr, uint8_t data) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	if (!spi_begin(device->bus))
		return false;
	digitalWrite(device->cs, LOW);

	addr &= ~0x80;

	bool ret = true;
	ret &= spi_write(device->bus, &addr, sizeof(addr));
	ret &= spi_write(device->bus, &data, sizeof(data));
	digitalWrite(device->cs, HIGH);
	ret &= spi_end(device->bus);
	return ret;
}

bool bmx160_init(bmx160_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	// Reset device
	if (!bmx160_reset(device))
		return false;

	// Check if Chip ID is correct
	uint8_t chip_id = 0;
	if (!bmx160_get_chip_id(device, &chip_id))
		return false;

	if (chip_id != BMX160_CHIP_ID)
		return false;

	if(!bmx160_write_config(device))
		return false;

	return true;
}

bool bmx160_reset(bmx160_device_t* device) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	for(uint16_t i = 0; i < sizeof(device->config)/sizeof(device->config[0]);i++){
		if(bmx160_write_register(device, device->config[i].address, device->config[i].default_value)){
			device->config[i].current_value = device->config[i].default_value;
		}else{
			return false;
		}
	}

	return true;
}

bool bmx160_get_chip_id(bmx160_device_t* device, uint8_t* id) {
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(id, DART_ERROR_INVALID_VALUE, false);

	*id = 0;
	return bmx160_read_register(device, BMX160_REG_CHIP_ID, id, sizeof(uint8_t));
}

bool bmx160_write_config(bmx160_device_t* device){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	for(uint16_t i = 0; i < sizeof(device->config)/sizeof(device->config[0]);i++){
		if(!bmx160_write_register(device, device->config[i].address, device->config[i].current_value)){
			return false;
		}
	}

	return true;

}

bool bmx160_read_acc(bmx160_device_t* device, float* acc_x, float* acc_y, float* acc_z){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(acc_x, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(acc_y, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(acc_z, DART_ERROR_INVALID_VALUE, false);

	bmx160_type_conversion_t data = {0};

	if(!bmx160_read_register(device, BMX160_REG_ACC_X_7_0, data.value_uint8, sizeof(data))){
		return false;
	}

	uint8_t acc_range = 2;

	if(bmx160_get_acc_range(device, &acc_range, false)){

		*acc_x = ((data.value_uint16[1])/(0xFFFF))* 2 * acc_range-acc_range;
		*acc_y = ((data.value_uint16[2])/(0xFFFF))* 2 * acc_range-acc_range;
		*acc_z = ((data.value_uint16[3])/(0xFFFF))* 2 * acc_range-acc_range;

		return true;
	}

	return false;

}

bool bmx160_read_gyr(bmx160_device_t* device, float* gyr_x, float* gyr_y, float* gyr_z){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(gyr_x, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(gyr_y, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(gyr_z, DART_ERROR_INVALID_VALUE, false);

	bmx160_type_conversion_t data = {0};

	if(!bmx160_read_register(device, BMX160_REG_GYR_X_7_0, data.value_uint8, sizeof(data))){
		return false;
	}

	uint16_t gyr_range = 2;

	if(bmx160_get_gyr_range(device, &gyr_range, false)){

		*gyr_x = ((data.value_uint16[1])/(0xFFFF))* 2 * gyr_range-gyr_range;
		*gyr_y = ((data.value_uint16[2])/(0xFFFF))* 2 * gyr_range-gyr_range;
		*gyr_z = ((data.value_uint16[3])/(0xFFFF))* 2 * gyr_range-gyr_range;

		return true;
	}

	return false;

}

bool bmx160_read_mag(bmx160_device_t* device, float* mag_x, float* mag_y, float* mag_z){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(mag_x, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(mag_y, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(mag_z, DART_ERROR_INVALID_VALUE, false);

	bmx160_type_conversion_t data = {0};

	if(!bmx160_read_register(device, BMX160_REG_MAG_X_7_0, data.value_uint8, sizeof(data))) {
		return false;
	}

	uint16_t mag_range = 1300;
	uint16_t magz_range = 2500;

	*mag_x = (data.value_uint16[0] / (float)0xFFFF) * 2 * mag_range - mag_range;
	*mag_y = (data.value_uint16[1] / (float)0xFFFF) * 2 * mag_range - mag_range;
	*mag_z = (data.value_uint16[2] / (float)0xFFFF)* 2 * magz_range - magz_range;

	return true;

}


bool bmx160_get_index_of_address(bmx160_device_t* device, uint8_t* index, uint8_t address){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(index, DART_ERROR_INVALID_VALUE, false);

	*index = -1;

	if(!(address<=0x7E && address>=0x40)){
		return false;
	}
	for(int i = address-0x40; i > address-0x05 ;i--){
		if(device->config[i].address == address){
			*index = i;
			return true;
		}
	}
	return false;
}


bool bmx160_get_acc_range(bmx160_device_t* device, uint8_t* range, bool default_value){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(range, DART_ERROR_INVALID_VALUE, false);

	uint8_t index;

	if(!bmx160_get_index_of_address(device, &index, BMX160_REG_ACC_RANGE)){
		return false;
	}

	uint8_t value = 0;

	if(default_value){
		value = device->config[index].default_value;
	}else{
		value = device->config[index].current_value;
	}

	if(value == BMX160_REG_ACC_RANGE_4G){
		*range = 4;
		return true;
	}
	if(value == BMX160_REG_ACC_RANGE_8G){
		*range = 8;
		return true;
	}
	if(value == BMX160_REG_ACC_RANGE_16G){
		*range = 16;
		return true;
	}
	if(value == BMX160_REG_ACC_RANGE_2G){
		*range = 2;
		return true;
	}

	return false;
}


bool bmx160_get_gyr_range(bmx160_device_t* device, uint16_t* range, bool default_value){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(range, DART_ERROR_INVALID_VALUE, false);

	uint8_t index;

	if(!bmx160_get_index_of_address(device, &index, BMX160_REG_GYR_RANGE)){
		return false;
	}

	uint16_t value = 0;

	if(default_value){
		value = device->config[index].default_value;
	}else{
		value = device->config[index].current_value;
	}

	if(value == BMX160_REG_GYR_RANGE_125){
		*range = 125;
		return true;
	}
	if(value == BMX160_REG_GYR_RANGE_250){
		*range = 250;
		return true;
	}
	if(value == BMX160_REG_GYR_RANGE_500){
		*range = 500;
		return true;
	}
	if(value == BMX160_REG_GYR_RANGE_1000){
		*range = 1000;
		return true;
	}
	if(value == BMX160_REG_GYR_RANGE_2000){
		*range = 2000;
		return true;
	}

	return false;
}



