#ifndef DART_DRIVER_EVE2_DISPLAY_H_
#define DART_DRIVER_EVE2_DISPLAY_H_

#include "../../../../../chickenFlap/dart/hal_wrapper/hal_wrapper.h"

#ifdef USE_EVE2_DISPLAY

#include "../../../../../chickenFlap/dart/spi.h"
#include "../../../../../chickenFlap/dart/digital.h"
#include "../../../../../chickenFlap/dart/driver/graphics/eve2/EVE_config.h"
#include "../../../../../chickenFlap/dart/driver/graphics/eve2/EVE_target.h"
#include "../../../../../chickenFlap/dart/driver/graphics/eve2/EVE_commands.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int bus;
	int cs;
	int pdn_pin;
} eve2_display_t;

void eve2_select(eve2_display_t* display);
bool eve2_init(eve2_display_t* display);

// Target functions for EVE driver
void DELAY_MS(int ms);
void EVE_pdn_set();
void EVE_pdn_clear();
void EVE_cs_set();
void EVE_cs_clear();
uint8_t fetch_flash_byte(const uint8_t* data);

void spi_transmit(uint8_t data);
void spi_transmit_async(uint8_t data);
uint8_t spi_receive(uint8_t data);



#ifdef __cplusplus
}
#endif


#endif
#endif
