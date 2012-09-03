/*
 * \file test.c
 *
 * \brief Contains tools relative to all tests such as debugging
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 

#include "test.h"
#include <conf.h>

void init_debug()
{
	// Init Arduino serial debugging.
	PORTD.DIRSET   = PIN3_bm;   // Pin 3 (TXD0) as output.
	USARTD0.CTRLC = (uint8_t) USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | false; // USARTD0, 8 Data bits, No Parity, 1 Stop bit
	USARTD0.BAUDCTRLA = 12;
	USARTD0.CTRLB |= USART_TXEN_bm | USART_RXEN_bm; // Enable both RX and TX
}

/**
 * \brief sends a debug message to Arduino serial monitor.
 * \note uses usartD0 (pin D3) for outputting debugging serial signals
 */
void debug(char *message)
{
	#ifdef DEBUG
	
	if (!debug_initialized)
	{
		init_debug();
	}
	
	int i = 0;
	while(message[i] != '\0') //hacked because sizeof doesnt work???
	{
		// Transmit a character, first wait for the previous to be sent
		while( (USARTD0.STATUS & USART_DREIF_bm) == 0 ) {}
		// Now, transmit the character
		USARTD0.DATA = message[i];

		i++;
	}
	while( (USARTD0.STATUS & USART_DREIF_bm) == 0 ) {}
	USARTD0.DATA = '\n';
	
	#endif
}	
