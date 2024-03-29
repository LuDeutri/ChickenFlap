/*
 * This is used to set the user defined configurations
 *
 * Author: Luca Deutrich
 * Date: 26.12.21
 */

#ifndef COMPONENTS_CONFIG_H_
#define COMPONENTS_CONFIG_H_


// ------------ Enable flags ------------
// If not used comment out the corresponding lines

// Enable start animation
#define SSD13006_ENABLE_START_ANIMATION
// Enable start animation slogan
#define SSD13006_START_ANIMATION_SLOGAN
// Enable start animation movie scene
#define SSD13006_START_ANIMATION_MOVIE_SCENE

// Enable DEBUG LED
//#define ENABLE_DEBUG_LED

// Enable RTC using otherwise the system clk is used for the watch
//#define USE_RTC

// Define if DS3231 module is used for rtc time
#define USE_DS3231

// Only relevant if DS3231 is used. Prevent that the time is overwritten at every restart of the uC
// Should be defined if changes in the watch code were done
//#define FIRST_SOFTWARE_FLASH

// Define if a battery is used to activate error check and display monitoring of SOC
#define USE_BATTERY
//#define ENABLE_BATTERY_ERR_CHECK

//---------- Watch ----------------
#define FIRST_SOFTWARE_FLASH // Only relevant if DS3231 is used

//---------- Language ----------------
// If no other language is enabled, english is used
// Use german language for error and warning descriptions
//#define ENABLE_GERMAN_LANGUAGE

/* -------------------------------------------------------------------
 * Dont comment out the constants below this line. Only adjust of the defined value is permitted
 * -------------------------------------------------------------------
 */

// ------------ Timeouts ------------
// Error
#define TIMEOUT_ERROR_TIMER_TIMES 600000 	// 10 minutes
#define TIMEOUT_ERROR_DISPLAY_INIT 10000 	// 10 seconds
#define TIMEOUT_ERROR_MAX_MOTOR_RUNNING_TIME 120000 // ms <=> 120 seconds
#define TIMEOUT_ERROR_BATTERY_EMPTY 600000 // 100 seconds

// Warning
#define TIMEOUT_WARN_NO_BATTERY_DATA 300000
#define TIMEOUT_WARN_BATTERY_LOW 400000

// Define time without any pressed buttons until the statemachine switch into Sleepmode and turn off the display
#define TIMEOUT_SLEEP_MODE 60000 // ms (1 min)
// Define max. duration of the start animation
#define TIMEOUT_MAX_START_ANIMATION_TIME 40000 // 40 seconds
// Define time after that an error or warning notification can show again
#define TIMEOUT_NOTIFICATION_SHOWN 1200000 // 20 minutes


// ------------ Battery ------------
// Number of cells in the battery
#define CELL_NUMBER_12V_CAR_BATTERY 1
// Define the error threshold for an empty battery
#define ERROR_BATTERY_EMPTY_PERCENTAGE 10	// %
// Define the warning threshold for a low battery level
#define WARNING_BATTERY_LOW_PERCENTAGE 30	// %
// Overvoltage threshold
#define CELL_OVERVOLTAGE_CAR_BATTERY 14000 // mV
// Undervoltage threshold
#define CELL_UNDERVOLTAGE_CAR_BATTERY 10000 // mV
// Correction factor
#define BMS_VOLATAGE_CORRECTION_FACTOR 0.533601492239f // ADC value 4095 means 2.045V + inaccuracies in voltage divider resistors

// ------------ PCB Hardware ------------
// Resistor values of the voltage divider of the ADC input
#define ADC_VOLTAGE_DIVIDER_R1 10000 // Ohm
#define ADC_VOLTAGE_DIVIDER_R2 1820	// Ohm


// ------------ Motor ------------
// Define default motor running time if no battery data are used
#define DEFAULT_MOTOR_RUNNING_TIME 2000 // ms
#define MOTOR_CLOSING_RUNNING_TIME_CORRECTION_FACTOR 0.6f //ms


// ------------ Flap timer ------------
// Default opening time --> 08:00 o'watch
#define DEFAULT_OPEN_FLAP_TIME_HOUR 8
#define DEFAULT_OPEN_FLAP_TIME_MINUTE 0
// Default closing time --> 18:00 o'watch
#define DEFAULT_CLOSE_FLAP_TIME_HOUR 18
#define DEFAULT_CLOSE_FLAP_TIME_MINUTE 0
// If the button is pressed longer than here given, the digits of the watch and the timer rise/sink faster when you set them, also used for the timer digits
#define PRESSED_BUTTON_TIME_FOR_FAST_NUMBER_RISE 3000 //ms

#endif
