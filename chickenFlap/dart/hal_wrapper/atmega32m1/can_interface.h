/*
 * can_interface.h
 *
 * Created: 16.05.2015 19:48:12
 *  Author: philipp
 */ 


#ifndef CAN_INTERFACE_H_
#define CAN_INTERFACE_H_

#if defined(USE_ATMEGA32M1) && defined(HAL_ENABLE_CAN)

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define NO_TX_MOBS 2
#define NO_MOBS 6			//number of message objects
#define NOMOB	0xff
#define CAN_BAUDRATE 500 

// ----------
#ifndef FOSC
#  error  You must define FOSC
#endif
// ----------
#ifndef CAN_BAUDRATE
#  error  You must define CAN_BAUDRATE 
#endif
// ----------
#if FOSC == 16000             //!< Fclkio = 16 MHz, Tclkio = 62.5 ns
#   if   CAN_BAUDRATE == 100       //!< -- 100Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x12       // Tscl  = 10x Tclkio = 625 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 125       //!< -- 125Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x0E       // Tscl  = 8x Tclkio = 500 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 200       //!< -- 200Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x08       // Tscl  = 5x Tclkio = 312.5 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 250       //!< -- 250Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x06       // Tscl  = 4x Tclkio = 250 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 500       //!< -- 500Kb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x06       // Tscl = 4x Tclkio = 250 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 1000      //!< -- 1 Mb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 125 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   else
#       error This CAN_BAUDRATE value is not in "CAN_driver.h" file
#   endif

#elif FOSC == 12000           //!< Fclkio = 12 MHz, Tclkio = 83.333 ns
#   if   CAN_BAUDRATE == 100       //!< -- 100Kb/s, 20x Tscl, sampling at 75%
#       define CONF_CANBT1  0x0A       // Tscl  = 6x Tclkio = 500 ns
#       define CONF_CANBT2  0x0E       // Tsync = 1x Tscl, Tprs = 8x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x4B       // Tpsh1 = 6x Tscl, Tpsh2 = 5x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 125       //!< -- 125Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x0A       // Tscl  = 6x Tclkio = 500 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 200       //!< -- 200Kb/s, 20x Tscl, sampling at 75%
#       define CONF_CANBT1  0x04       // Tscl  = 3x Tclkio = 250 ns
#       define CONF_CANBT2  0x0E       // Tsync = 1x Tscl, Tprs = 8x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x4B       // Tpsh1 = 6x Tscl, Tpsh2 = 5x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 250       //!< -- 250Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x04       // Tscl  = 3x Tclkio = 250 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 500       //!< -- 500Kb/s, 12x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 166.666 ns
#       define CONF_CANBT2  0x08       // Tsync = 1x Tscl, Tprs = 5x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x25       // Tpsh1 = 3x Tscl, Tpsh2 = 3x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 1000      //!< -- 1 Mb/s, 12x Tscl, sampling at 75%
#       define CONF_CANBT1  0x00       // Tscl  = 1x Tclkio = 83.333 ns
#       define CONF_CANBT2  0x08       // Tsync = 1x Tscl, Tprs = 5x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x25       // Tpsh1 = 3x Tscl, Tpsh2 = 3x Tscl, 3 sample points
#   else
#       error This CAN_BAUDRATE value is not in "can_drv.h" file
#   endif

#elif FOSC == 8000              //!< Fclkio = 8 MHz, Tclkio = 125 ns
#   if   CAN_BAUDRATE == 100       //!< -- 100Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x08       // Tscl  = 5x Tclkio = 625 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 125       //!< -- 125Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x06       // Tscl  = 4x Tclkio = 500 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 200       //!< -- 200Kb/s, 20x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 250 ns
#       define CONF_CANBT2  0x0E       // Tsync = 1x Tscl, Tprs = 8x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x4B       // Tpsh1 = 6x Tscl, Tpsh2 = 5x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 250       //!< -- 250Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 250 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 500       //!< -- 500Kb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 250 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 1000      //!< -- 1 Mb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x00       // Tscl  = 1x Tclkio = 125 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   else
#       error This CAN_BAUDRATE value is not in "can_drv.h" file
#   endif

#else
#   error This FOSC value is not in "can_drv.h" file
#endif

typedef struct
{
	uint16_t id;			// message ID 11bit for CAN 2.0 A
	uint32_t msk;        	// ID Mask for message
	uint8_t length;   		// length of data DLC
	uint8_t data[8];  		// data vector
} CAN_messageType;

typedef struct
{
	uint16_t id;
	uint16_t mask;
	uint8_t rtr;
} CAN_filterType;


void can_set_filter(uint8_t number, const CAN_filterType *filter);

void can_init(void);

void can_tx(CAN_messageType *msg, uint8_t wait);

uint8_t can_check_message(void);

uint8_t can_get_message(CAN_messageType *message);

#endif

#endif
