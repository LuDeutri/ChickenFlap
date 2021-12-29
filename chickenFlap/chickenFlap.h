/**
 * This is an self developed chickenflap.
 *
 * Author: Luca Deutrich
 * Date: 23.12.2021
 * Version: 1.0
 */

/** Electrical Information
 * PA0 = ADC: Battery voltage with voltage divider
 * PA1 = Motor rotation: open
 * PA2 = Motor rotation: close
 * PA3 = Button: Menu back (left-top)
 * PA4 = Button: Menu forward (rigth-top)
 * PA5 = Button: Menu left (left-bottom)
 * PA6 = Button: Menu right (right-bottom)
 * PB7 = SDA
 * PB6 = SCL
 *
 * Display: SSD1309 2.4 Zoll OLED display 128x64 px
 */

#ifndef CHICKENFLAP_H_
#define CHICKENFLAP_H_

#include "components/button.h"
#include "components/config.h"
#include "components/display.h"
#include "components/stateMachine.h"
#include "components/timer.h"
#include "components/watch.h"

/*
 * Main function
 */
void chickenFlap_main();

void chickenFlap_init();
/*
 * State machine
 */
void chickenFlap_update();

#endif
