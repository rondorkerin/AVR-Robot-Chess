/**
 * \file
 *
 * \brief Empty user application template
 *
 */
#define LED_IOPORT            PORTE
/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <gpio.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>
#include "chessengine.h"

#ifdef __GNUC__
# include <util/delay.h>
#else
static inline void _delay_ms(uint16_t t)
{
	uint16_t i;

	for (i = 0; i < t; ++i) {
		__delay_cycles((UINT32_C)(sysclk_get_cpu_hz() / 6000));
	}
}
#endif


static inline void write_board_led_mask(uint8_t mask)
{
	LED_IOPORT.OUT = ~mask;
}

/************************************************************************/
/* Sends a debug message to the arduino serial monitor                  */
/* Hook pin D3 of the Xmega up to Pin 2 of the arduino	                */
/************************************************************************/
void debug_msg(char *message)
{	
	int i = 0;
    while(message[i] != '\0') //hacked because sizeof doesnt work???
    {
        // Transmit a character, first wait for the previous to be sent
        while( (USARTD0.STATUS & USART_DREIF_bm) == 0 ) {}
       	  // Now, transmit the character
        USARTD0.DATA = message[i];      

        i++;
    }
	USARTD0.DATA = '_';    
	while( (USARTD0.STATUS & USART_DREIF_bm) == 0 ) {}
	// carriage return and line feed dont work
}


/*
 * TEST MAIN: 
 * Issues a sequence of pawn movements and blinks an LED if a piece was taken. 
 * sequence: a2-a4, a4-a5, a5-a6
 *
 */ 

int main (void)
{
	board_init();

	// SET UP SERIAL DEBUGGING VIA ARDUINO (D3 is TX for Xmega) D3 -> D2, D2-> D3, Gnd->Gnd)
	PORTD.DIRSET   = PIN3_bm;   // Pin 3 (TXD0) as output.
	USARTD0.CTRLC = (uint8_t) USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | false; // USARTD0, 8 Data bits, No Parity, 1 Stop bit
	USARTD0.BAUDCTRLA = 12;
	USARTD0.CTRLB |= USART_TXEN_bm | USART_RXEN_bm; // Enable both RX and TX
	
	debug_msg("init game engine.");
			
	while (1){		
		
		InitEngine();
		InitGame(PlayerVsAI);

		int gameResult = 0, tookPieceFlag = 0,illegalMoveFlag = 0;
		char resultMove[4];

		
		char cmd[3][100] = {"a2a4", "a4a5", "a5a6"};
		int i;
		for (i = 0; i < 5; i++)
		{
			PlayerMove(cmd[i], &gameResult, &tookPieceFlag, &illegalMoveFlag);
			debug_msg("player mv:");
			debug_msg(cmd[i]);
			
			if (illegalMoveFlag)
			{
				debug_msg("illegal move");
				continue;
			}
			AIMove(&gameResult, &tookPieceFlag, resultMove);
					
			if (!tookPieceFlag)
			{
				debug_msg("ai mv:");
				debug_msg(resultMove);
			}
			else
			{
				debug_msg("AI took player piece");
				for (;;) {						
				}	
			}
		}


		
		
	}
	
	
}

