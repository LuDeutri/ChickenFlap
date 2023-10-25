/*
 * can_drv.c
 *
 * Created: 13.05.2015 14:22:17
 *  Author: philipp
 */ 
#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/can_drv.h"

#if defined(USE_ATMEGA32M1) && defined(HAL_ENABLE_CAN)
volatile uint8_t _messages_waiting = 0;

uint8_t getmob( uint32_t bitmask)
{
	uint8_t mob;

	if( bitmask==0) return NOMOB;

	//the bitmask is shifted to the right until the LSB is one
	//the number of such shift operations is the index of the MOb
	for( mob=0; (bitmask & 0x01)==0; bitmask >>= 1, ++mob);

	//security check
	if ( mob > NO_MOBS ) return NOMOB;
	else return mob;
}

void can_copy_mob_to_message(CAN_messageType *message)
{
	// read status
	uint8_t cancdmob = CANCDMOB;
	
	// read length
	message->length = cancdmob & 0b00001111;
	
	//get the ID
	message->id  = (uint8_t)  CANIDT2 >> 5;
	message->id |= (uint16_t) CANIDT1 << 3;
	
	// read data
	uint8_t *p = message->data;
	for (uint8_t i = 0; i < message->length; i++) {
		*p++ = CANMSG;
	}
}

void enter_standby_mode(void)
{
	// request abort
	CANGCON = (1 << ABRQ);
		
	// wait until receiver is not busy
	while (CANGSTA & (1 << RXBSY));
		
	// request standby mode
	CANGCON = 0;
		
	// wait until the CAN Controller has entered standby mode
	while (CANGSTA & (1 << ENFG));
}

void leave_standby_mode(void)
{
	// save CANPAGE register
	uint8_t canpage = CANPAGE;
		
	// reenable all MObs
	for (uint8_t i = 0; i < NO_MOBS; i++) {
		CANPAGE = i << 4;
		CANCDMOB = CANCDMOB;
	}
		
	// restore CANPAGE
	CANPAGE = canpage;
		
	// request normal mode
	CANGCON = (1 << ENASTB);
		
	// wait until the CAN Controller has left standby mode
	while ((CANGSTA & (1 << ENFG)) == 0);
}

SIGNAL(CAN_INT_vect)
{
	uint8_t save_canpage = CANPAGE;		// store current CANPAGE
	uint8_t mob;

	// get the index of the MOb for which the interrupt occured
	mob = getmob(CANSIT2 | (CANSIT1 << 8));
	if (mob == NOMOB)
		return;
   	
	//select concerned MOb page
	CANPAGE = mob << 4;	

	//Can message transmitted
	if (CANSTMOB & (1<<TXOK)) {	 	
		 CANSTMOB &= ~(1<<TXOK);	// reset INT flag
	}
	else if(CANSTMOB & (1<<RXOK)) {	//Can message received	
		_messages_waiting++;
		CANIE2 &= ~(1 << mob);		//disable interrupt for mob which received the message			
	}
	
	CANPAGE = save_canpage;   	// restore CANPAGE
}
#endif
