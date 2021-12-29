/*
 * can_interface.c
 *
 * Created: 16.05.2015 19:48:01
 *  Author: philipp
 */ 

#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/can_interface.h"

#include "../../../../chickenFlap/dart/hal_wrapper/atmega32m1/can_drv.h"

#if defined(USE_ATMEGA32M1) && defined(HAL_ENABLE_CAN)

/**
 * \brief	Inits the CAN with a Baudrate of 1Mbit
 * 
 * \warning needs to be called before enabling the global interrupts.
 */ 

void can_init( void)
{
	CANGCON |= (1 << SWRES);			//can controller in reset mode
	
	CANBT1 = CONF_CANBT1;
	CANBT2 = CONF_CANBT2;
	CANBT3 = CONF_CANBT3;
	
	CANGIT = 0;						   
	CANGIE |= (1 << ENTX);			   //enable TX interrupt
	CANGIE |= (1 << ENRX);			   //enable RX interrupt
	CANGIE |= (1 << ENIT);
	
	//disable all mobs
	for(int i = 0; i < NO_MOBS ; i++)
	{
		CANPAGE = (i << 4);
		CANCDMOB &= 0;
		CANSTMOB &= 0;
	}
	
	CANGCON |= (1 << ENASTB);		   //enable the can controller
}


/**
 * \brief	Sets the defined Filter and writes the Registers correctly
 * 
 * 
 */
void can_set_filter(uint8_t number, const CAN_filterType *filter)
{
	if (number < NO_TX_MOBS)
		return;
	if (number > NO_MOBS)
		return;
	
	enter_standby_mode();
	
	CANPAGE = number << 4;
	
	CANSTMOB = 0;			//reset the MOB
	CANCDMOB = 0;
	
	CANIDT4 = 0;
	CANIDT3 = 0;
	CANIDT2 = (uint8_t)  filter->id << 5;
	CANIDT1 = (uint16_t) filter->id >> 3;
	
	CANIDM4 = (1 << IDEMSK);
	CANIDM3 = 0;
	CANIDM2 = (uint8_t)  filter->mask << 5;
	CANIDM1 = (uint16_t) filter->mask >> 3;
	
	
	//enable reception of MOB
	CANCDMOB |= (1 << CONMOB1);
	
	//enable MOB interrupts
	CANIE1 = 0;
	CANIE2 |= (1 << number);
	
	leave_standby_mode();
}


/**
 * \brief		Sends the given CAN message ONCE 
 * 
 */
void can_tx(CAN_messageType *msg, uint8_t wait)
{
	uint8_t i;
	static uint8_t mob = 0;

	//select MOb to be sent
	//enable MOb number mob, auto increment index, start with index = 0
	CANPAGE = (mob++ << 4);
		
	if (mob >= NO_TX_MOBS)
		mob = 0;
	
	//clear all flags
	CANSTMOB &= 0;
	
	CANCDMOB = msg->length;
	
	//write 11 Bit identifier to MOb-ID-Tag
	CANIDT4 = 0;
	CANIDT3 = 0;
	CANIDT2 = (uint8_t) (msg->id << 5);
	CANIDT1 = (uint16_t) (msg->id >> 3);
	
	//put data in mailbox
	for (i=0; i < msg->length; i++)
		CANMSG = msg->data[i];
	
	//wait until transmit possible (TXBSY==0)
	if (wait) {
		uint32_t timeout = 0x4000;
		while ((CANGSTA & (1<<TXBSY)) && timeout > 0);
			timeout--;
		
		//check if sending is possible, if not return
		if (timeout == 0)
			return;
	}
	
	//enable MOB interrupts
	CANIE2 |= (1 << mob);
	
	//enable transmission. After the tx is complete the TXOK bit is cleared in the ISR
	CANCDMOB |= (1<<CONMOB0);

}


/**
 * \brief		Checks if a message is waiting in the mob buffer
 *
 * \return		returns 1 if a message is waiting
 *						0 if there is no message 
 * 
 */
uint8_t can_check_message()
{
	if(_messages_waiting > 0)
		return 1;
	else
		return 0;
}


/**
 * \brief		Copies a received message into given address. If there is no message, the function does nothing
 *
 * \return		returns 1 if the message was successfully copied
 *						0 if there is no message to copy
 * 
 */
uint8_t can_get_message(CAN_messageType *message)
{
	
	uint8_t save_CANGIE = CANGIE;		//disable can interrupts and save register
	CANGIE &= 0;
	
	uint8_t mob;
	if(!can_check_message())
		return 0;
	
	//look for mob with a message
	for(mob = 0; mob < NO_MOBS; mob++)
	{
		CANPAGE = mob << 4;
		
		if(CANSTMOB & (1 << RXOK))
		{
			// clear flags
			CANSTMOB &= 0;
			break;
		}
	}
	
	can_copy_mob_to_message(message);
	
	cli();
	_messages_waiting--;
	sei();
	
	CANIE2 |= (1 << mob);		//reenable can interrupt for mob
	CANSTMOB &= ~(1<<RXOK);		//reset flag cause its not done in the ISR
	
	CANGIE |= save_CANGIE;		//restore CANGIE register and reenable general can interrupts
	
	CANCDMOB = (1 << CONMOB1) | (CANCDMOB & (1 << IDE));
	
	return mob+1;
}

#endif
