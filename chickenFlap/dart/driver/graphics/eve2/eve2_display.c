#include "../../../../../chickenFlap/dart/driver/graphics/eve2/eve2_display.h"

#ifdef USE_EVE2_DISPLAY

eve2_display_t* eve2_current = NULL;

void eve2_select(eve2_display_t* display) {
	eve2_current = display;
}

bool eve2_init(eve2_display_t* display) {
	DART_NOT_NULL_RETURN(display, DART_ERROR_INVALID_VALUE, false);

	eve2_select(display);
	//if (!spi_set_speed(display->bus, 100000)) // 100 kHz
	//	return false;
	if (EVE_init() == 0)
		return false;
	//return spi_set_speed(display->bus, 20000000); // 20 MHz
	return true;
}

void DELAY_MS(int ms) {
	delayMillis(ms);
}

void EVE_pdn_set() {
	DART_NOT_NULL(eve2_current, DART_ERROR_INVALID_VALUE);
	digitalWrite(eve2_current->pdn_pin, LOW);
}
void EVE_pdn_clear() {
	DART_NOT_NULL(eve2_current, DART_ERROR_INVALID_VALUE);
	digitalWrite(eve2_current->pdn_pin, HIGH);
}
void EVE_cs_set() {
	DART_NOT_NULL(eve2_current, DART_ERROR_INVALID_VALUE);
	digitalWrite(eve2_current->cs, LOW);
}
void EVE_cs_clear() {
	DART_NOT_NULL(eve2_current, DART_ERROR_INVALID_VALUE);
	digitalWrite(eve2_current->cs, HIGH);
}
uint8_t fetch_flash_byte(const uint8_t* data) {
	DART_NOT_NULL_RETURN(data, DART_ERROR_INVALID_VALUE, 0);
	return *data;
}

void spi_transmit(uint8_t data) {
	DART_NOT_NULL(eve2_current, DART_ERROR_INVALID_VALUE);
	spi_transfer8(eve2_current->bus, data);
}
void spi_transmit_async(uint8_t data) {
	spi_transmit(data);
}
uint8_t spi_receive(uint8_t data) {
	DART_NOT_NULL_RETURN(eve2_current, DART_ERROR_INVALID_VALUE, 0);
	return spi_transfer8(eve2_current->bus, data);
}
#endif
