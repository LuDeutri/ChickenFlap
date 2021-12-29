#include "../../../chickenFlap/dart/driver/max11213.h"

bool max11213_init(max11213_device_t* device){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	digitalWrite(device->cs,1);
	return true;
}
bool max11213_get_Status(max11213_device_t* device,uint8_t* status){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	uint8_t addr = (uint8_t)(basicReadAddress|RegisterAddressStat1);

	return max11213_read_register(device, addr, status, sizeof(*status));
}
bool max11213_start_conversion(max11213_device_t* device, uint8_t rate){
	uint8_t command = (uint8_t)(basicConversion|rate);

	bool ret = true;
    ret &= spi_begin(device->bus);
    digitalWrite(device->cs,0);
    ret &= spi_write(device->bus, &command, sizeof(command));
    digitalWrite(device->cs,1);
    ret &= spi_end(device->bus);
    return ret;
}
bool max11213_is_conversion_finished(max11213_device_t* device, bool* finished){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);
	DART_NOT_NULL_RETURN(finished, DART_ERROR_INVALID_VALUE, false);

	uint8_t status;
	bool ret = true;
	ret &= spi_begin(device->bus);
	ret &= spi_read(device->bus, &status, sizeof(status));
	ret &= spi_end(device->bus);
	if(status == 0)
		*finished = true;
	else
		*finished = false;
	return ret;
}

bool max11213_get_data(max11213_device_t* device, uint16_t* data){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	bool ret = true;
	ret &= spi_begin(device->bus);
	digitalWrite(device->cs,0);
	ret &= spi_read(device->bus, (uint8_t*)data, sizeof(*data));
	digitalWrite(device->cs,1);
	ret &= spi_end(device->bus);
	return ret;
}

bool max11213_set_GPIOs(max11213_device_t* device, uint8_t* config){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	uint8_t addr = (uint8_t)(basicWriteAddress|RegisterAddressCtrl2);

	return max11213_write_register(device, addr, config, sizeof(*config));
}

bool max11213_setConfig(max11213_device_t* device, uint8_t* config){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	uint8_t addr = (uint8_t)(basicWriteAddress|RegisterAddressCtrl1);

	return max11213_write_register(device, addr, config, sizeof(*config));
}

bool max11213_setGain(max11213_device_t* device, uint8_t* config){
	DART_NOT_NULL_RETURN(device, DART_ERROR_INVALID_VALUE, false);

	uint8_t addr = (uint8_t)(basicWriteAddress|RegisterAddressCrtl3);

	return max11213_write_register(device, addr, config, sizeof(*config));
}

bool max11213_read_register(max11213_device_t* device,uint8_t addr, uint8_t* data, size_t size){
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	bool ret = true;
	ret &= spi_begin(device->bus);
	digitalWrite(device->cs,0);
	ret &= spi_write(device->bus,&addr,sizeof(addr));
	ret &= spi_read(device->bus,data, size);
	digitalWrite(device->cs,1);
	ret &= spi_end(device->bus);
	return ret;
}

bool max11213_write_register(max11213_device_t* device, uint8_t addr, uint8_t* data, size_t size){
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, false);

	bool ret = true;
	ret &= spi_begin(device->bus);
	digitalWrite(device->cs,0);
	ret &= spi_write(device->bus, &addr, sizeof(addr));
	ret &= spi_write(device->bus, data, size);
	digitalWrite(device->cs,1);
	ret &= spi_end(device->bus);
	return ret;
}


