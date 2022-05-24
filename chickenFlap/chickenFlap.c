#include "chickenFlap.h"

void chickenFlap_main(){
	digitalWrite(DISPLAY_ENABLE, LOW);
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

	delayMillis(1000);
	digitalWrite(DISPLAY_ENABLE, HIGH);

	button_init();
	display_init();
	flap_init();
	bms_init();
	stateMachine_init();
	timer_init();
	watch_init();
	error_init();
}

void chickenFlap_update(){
	// HAL update
	dartDigital_update();
	dartAnalog_update();
	dartUART_update();
	dartI2C_update();


	ssd1306StartAnimation();
	// Wait with normal operation until the start animation is finished
	if(startAnimation.enable)
		return;

	stateMachine_update();
	if (stateMachine.state> STATE_INIT) {
		bms_update();
		watch_update();
		button_update();
		display_update();
		error_update();
	}

	// Update error LED
	if (stateMachine.state == STATE_ERROR)
		digitalWrite(ERROR_LED, LOW);					// Error LED on
	else if (stateMachine.state == STATE_INIT)
		digitalWrite(ERROR_LED, millis() % 1000 > 500);	// Error LED blinks
	else{
		if (millis()>3000)
			digitalWrite(ERROR_LED, HIGH);				// Error LED after start sequence off
	}
	digitalWrite(DEBUG_LED, HIGH); // LED off

	// Update debug LED
	#ifdef ENABLE_DEBUG_LED
		if (stateMachine.state==STATE_ERROR)
			digitalWrite(DEBUG_LED, millis() % 300 > 150);
		 else if (stateMachine.state == STATE_INIT)
			digitalWrite(DEBUG_LED, millis() % 500 > 250);
		 else
			digitalWrite(DEBUG_LED, millis() % 3000 > 1500);
	#endif

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
