#include "chickenFlap.h"

void chickenFlap_main(){
	digitalWrite(DISPLAY_RST, LOW);
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
	digitalWrite(DISPLAY_RST, HIGH);

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

		setMotorSpeed();
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

	// Update debug LED
	#ifdef ENABLE_DEBUG_LED
		util_time_t tmp = millis();
		if(millis() < 10000) {
			// LED start animation
			digitalWrite(DEBUG_LED_BLUE, tmp > 1000 && tmp < 6000);
			digitalWrite(DEBUG_LED_GREEN, tmp > 2000 && tmp < 7000);
			digitalWrite(DEBUG_LED_YELLOW, tmp > 3000 && tmp < 8000);
			digitalWrite(DEBUG_LED_ORANGE, tmp > 4000 && tmp < 9000);
			digitalWrite(DEBUG_LED_RED, tmp > 5000 && tmp < 9500);
		} else if (stateMachine.state==STATE_ERROR){
			// LED error state
			digitalWrite(DEBUG_LED_BLUE, 0);
			digitalWrite(DEBUG_LED_GREEN, 0);
			digitalWrite(DEBUG_LED_YELLOW, 0);
			digitalWrite(DEBUG_LED_ORANGE, 0);
			digitalWrite(DEBUG_LED_RED, millis() % 1000 > 500);
		} else if (stateMachine.state == STATE_INIT){
			//LED init state
			digitalWrite(DEBUG_LED_BLUE, 1);
			digitalWrite(DEBUG_LED_GREEN, 0);
			digitalWrite(DEBUG_LED_YELLOW, 1);
			digitalWrite(DEBUG_LED_ORANGE, millis() % 500 > 1000);
			digitalWrite(DEBUG_LED_RED, 0);
		 } else {
			// LED normal operation state
			digitalWrite(DEBUG_LED_BLUE, tmp % 5000 > 0);
			digitalWrite(DEBUG_LED_GREEN, tmp % 5000 > 1000);
			digitalWrite(DEBUG_LED_YELLOW, tmp % 5000 > 2000);
			digitalWrite(DEBUG_LED_ORANGE, tmp % 5000 > 3000);
			digitalWrite(DEBUG_LED_RED, tmp % 5000 > 4000);
		 }
	#endif

}

void i2cTest(){
	dartUART_writeLine(3, "Scan I2C bus: ");
	for (int i = 0; i < I2C_MAX_SLAVE_COUNT; i++) {
		if (i2c_slave_ready(0, i))
			dartUART_format(3, "0x%x", i);
		else
			dartUART_write(3, ".");
	}
	dartUART_writeLine(3, "");
}
