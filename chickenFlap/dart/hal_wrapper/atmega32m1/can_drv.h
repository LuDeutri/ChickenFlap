/*
 * can_drv.h
 *
 * Created: 13.05.2015 14:21:56
 *  Author: philipp
 */ 


#ifndef CAN_DRV_H_
#define CAN_DRV_H_

#if defined(USE_ATMEGA32M1) && defined(HAL_ENABLE_CAN)

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/can_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NO_MOBS 6			//number of message objects
#define NOMOB	0xff

extern volatile uint8_t _messages_waiting;

uint8_t getmob(uint32_t bitmask);

void can_copy_mob_to_message(CAN_messageType *message);

void enter_standby_mode(void);

void leave_standby_mode(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* CAN_DRV_H_ */
