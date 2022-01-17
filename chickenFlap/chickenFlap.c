#include "chickenFlap.h"

void chickenFlap_main(){
	chickenFlap_init();


	while(1)
		chickenFlap_update();
}

void chickenFlap_init(){
	// HAL init
	dartDigital_init();
	dartAnalog_init();
	dartUART_init();
	dartI2C_init();

	button_init();
	display_init();
	flap_init();
	bms_init();
	stateMachine_init();
	timer_init();
	watch_init();
}

void chickenFlap_update(){
	// HAL update
	dartDigital_update();
	dartAnalog_update();
	dartUART_update();
	dartI2C_update();

	i2cTest();

	stateMachine_update();
	if (stateMachine.state> STATE_INIT) {
		bms_update();
		watch_update();
		button_update();
		stateMachine_update();
		display_update();
	}
}

void i2cTest(){
	dartUART_writeLine(UART_LOG, "Scan I2C bus: ");
	for (int i = 0; i < I2C_MAX_SLAVE_COUNT; i++) {
		if (i2c_slave_ready(0, i))
			dartUART_format(UART_LOG, "0x%x", i);
		else
			dartUART_write(UART_LOG, ".");
	}
	dartUART_writeLine(UART_LOG, "");
}
