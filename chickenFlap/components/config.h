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

// Enable battery capacity check, a low battery will be ignored if defined
//#define ENABLE_BATTERY_CAPACITY_CHECK
// Enable start animation
//#define SSD13006_ENABLE_START_ANIMATION
// Enable start animation slogan
#define SSD13006_START_ANIMATION_SLOGAN
// Enable start animation movie scene
#define SSD13006_START_ANIMATION_MOVIE_SCENE
// Enable DEBUG LED
//#define ENABLE_DEBUG_LED
// Enable battery load status in the top right of the display, should only be defined, if an accumulator is used
//#define ENABLE_BATTERY_LOAD_STATUS

//---------- One language must be defined ----------------
// Use german language for error and warning descriptions
//#define ENABLE_GERMAN_LANGUAGE
// Use english language for error and warning descitptions
#define ENABLE_ENGLISH_LANGUAGE

/* -------------------------------------------------------------------
 * Dont comment out the constants below this line. Only adjust of the defined value is permitted
 * -------------------------------------------------------------------
 */

// ------------ Timeouts ------------
// Error
#define TIMEOUT_ERROR_TIMER_TIMES 600000 	// 10 minutes
#define TIMEOUT_ERROR_DISPLAY_INIT 10000 	// 10 seconds
#define TIMEOUT_ERROR_MAX_MOTOR_RUNNING_TIME 120000 // ms <=> 120 seconds
#define TIMEOUT_ERROR_BATTERY_EMPTY 50000 // 50 seconds

// Warning
#define TIMEOUT_WARN_NO_BATTERY_DATA 30000	// 30 seconds
#define TIMEOUT_WARN_BATTERY_LOW 40000 // 40 seconds

// Define time without any pressed buttons until the statemachine switch into Sleepmode and turn off the display
#define TIMEOUT_SLEEP_MODE 60000 // ms (1 min)
// Define max. duration of the start animation
#define TIMEOUT_MAX_START_ANIMATION_TIME 40000 // 40 seconds
// Define time after that an error or warning notification can show again
#define TIMEOUT_NOTIFICATION_SHOWN 1200000 // 20 minutes


// ------------ Battery ------------
// Number of 1.5V cells in the battery
#define CELL_NUMBER_9V_BLOCK 6
// Define max. cell capacity
#define CELL_MAX_CAPACITY 3000 //mA
// Define battery cell voltage
#define CELL_MAX_VOLTAGE (BATTERY_MAX_VOLTAGE/CELL_NUMBER_9V_BLOCK)
// Define max. battery voltage
#define BATTERY_MAX_VOLTAGE 9000 // mV
// Define the error threshold for an empty battery
#define ERROR_BATTERY_EMPTY_PERCENTAGE 10	// %
// Define the warning threshold for a low battery level
#define WARNING_BATTERY_LOW_PERCENTAGE 30	// %


// ------------ PCB Hardware ------------
// Resistor values of the voltage divider of the ADC input
#define ADC_VOLTAGE_DIVIDER_R1 1500 // Ohm
#define ADC_VOLTAGE_DIVIDER_R2 681	// Ohm


// ------------ Motor ------------
// Define default motor running time if no battery data are used
#define DEFAULT_MOTOR_RUNNING_TIME 1000 // ms
// Define max. Motor speed in percent (for the low side driver)
#define MOTOR_SPEED_PERCENTAGE 55


// ------------ Watch ------------
// Default opening time --> 08:00 o'watch
#define DEFAULT_OPEN_FLAP_TIME_DEC_H 0
#define DEFAULT_OPEN_FLAP_TIME_ONE_H 8
#define DEFAULT_OPEN_FLAP_TIME_DEC_MIN 0
#define DEFAULT_OPEN_FLAP_TIME_ONE_MIN 0

// Default closing time --> 18:00 o'watch
#define DEFAULT_CLOSE_FLAP_TIME_DEC_H 1
#define DEFAULT_CLOSE_FLAP_TIME_ONE_H 8
#define DEFAULT_CLOSE_FLAP_TIME_DEC_MIN 0
#define DEFAULT_CLOSE_FLAP_TIME_ONE_MIN 0

// If the button is pressed longer than here given, the digits of the watch and the timer rise/sink faster when you set them, also used for the timer digits
#define PRESSED_BUTTON_TIME_FOR_FAST_NUMBER_RISE 3000 //ms

// ------------ System ------------
// DONT CHANGE IF YOU DONT KNOW WHAT YOU DO
// Define the number of selection blocks in the menu / timer interface on the display. 0 to number-1 is a valid value
#define MENU_SELECT_BLOCKS_NUMBER 2
#define TIMER_SELECT_BLOCKS_NUMBER 6
#define CONFIG_SELECT_BLOCKS_NUMBER 1

#endif
