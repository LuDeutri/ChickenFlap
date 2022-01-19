/*
 * This is used to set the user defined configurations
 *
 * Author: Luca Deutrich
 * Date: 26.12.21
 */

#ifndef COMPONENTS_CONFIG_H_
#define COMPONENTS_CONFIG_H_


/*----------------------------------------------------------------------------------------------------------
* ---------------------------------------------USER DEFINED VALUES------------------------------------------
* ----------------------------------------------------------------------------------------------------------
*/

// Define Time without any pressed Buttons until the statemachine switch into Sleepmode and turn off the display
#define TIME_TO_SLEEP_MODE 300000 // ms (5 min)
// Define max. battery voltage
#define BATTERY_MAX_VOLTAGE 9000 // mV
// Define max. battery cell capacity
#define BATTERY_MAX_CELL_CAPACITY 3000 // mA
// Define max. battery cell voltage
#define BATTERY_MAX_CELL_VOLTAGE 1800 //mV

// Defines the distance that the flap should move during opening and closing
#define FLAP_MOTION_DISTANCE 200 // mm
// Define motor speed with full charged battery
#define MAX_MOTOR_SPEED 4 // mm per second
// Define the Time the motor needs to open / close the door. This is depending on the battery voltage
#define MAX_MOTOR_RUNNING_TIME 120000 // ms <=> 120 seconds
// Define default motor running time if no battery data are used
#define DEFAULT_MOTOR_RUNNING_TIME 10000 // ms

// Define the number of selection blocks in the menu / timer interface on the display. 0 to number-1 is a valid value
#define MENU_SELECT_BLOCKS_NUMBER 2
#define TIMER_SELECT_BLOCKS_NUMBER 6
#define CONFIG_SELECT_BLOCKS_NUMBER 1

// If the button is pressed longer than here given, the digits of the watch and the timer rise/sink faster when you set them
#define PRESSED_BUTTON_TIME_FOR_FAST_NUMBER_RISE 3000 //ms

// default opening time --> 08:00 o'watch
#define DEFAULT_OPEN_FLAP_TIME_DEC_H 0
#define DEFAULT_OPEN_FLAP_TIME_ONE_H 8
#define DEFAULT_OPEN_FLAP_TIME_DEC_MIN 0
#define DEFAULT_OPEN_FLAP_TIME_ONE_MIN 0

// default closing time --> 18:00 o'watch
#define DEFAULT_CLOSE_FLAP_TIME_DEC_H 1
#define DEFAULT_CLOSE_FLAP_TIME_ONE_H 8
#define DEFAULT_CLOSE_FLAP_TIME_DEC_MIN 0
#define DEFAULT_CLOSE_FLAP_TIME_ONE_MIN 0

// 1: Timer is activated, 0: Timer is deactivated
#define TIMER_ACTIVATED 1


/*----------------------------------------------------------------------------------------------------------
* ------------------------------------------------SYSTEM VALUES---------------------------------------------
* ---------------------------------------------DON´T CHANGE ANYTHING----------------------------------------
* ----------------------------------------------------------------------------------------------------------
*/

#define VOLTAGE_REFERENCE 3300 // mV





#endif
