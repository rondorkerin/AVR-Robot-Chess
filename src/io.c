
/*
 * io.c
 *
 * Created: 8/21/2012 8:12:31 PM
 *  Author: nul4x3r0000
 */ 

#include "io.h"
#include <asf.h>
#include <conf.h>

#ifdef __GNUC__
#  include <util/delay.h>
#else
static inline void _delay_ms(uint16_t t)
{
	uint16_t i;

	for (i = 0; i < t; ++i) {
		__delay_cycles((unsigned long)sysclk_get_cpu_hz() / 6000);
	}
}
#endif


/**
 * \brief initializes i/o devices
 */
void io_init()
{
	// Init Arduino serial debugging.
	PORTD.DIRSET   = PIN3_bm;   // Pin 3 (TXD0) as output.
	USARTD0.CTRLC = (uint8_t) USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | false; // USARTD0, 8 Data bits, No Parity, 1 Stop bit
	USARTD0.BAUDCTRLA = 12;
	USARTD0.CTRLB |= USART_TXEN_bm | USART_RXEN_bm; // Enable both RX and TX
}



/**
 * \brief gets a player move in string format
 * \note example: 'a2a4'
 */
char* get_player_move(int side)
{
	while (1)
	{
		lcd_message("Please enter a move");
		char move[4];
		int num_moves = 0; // total number of moves recorded (we want to record 2 moves before leaving this loop)
		int button;
		while (num_moves < 2)
		{
			lcd_message("Please press 'From', 'To', or 'Clear'");
			button = get_button_press(side);
		
			if (button == Clr)
			{
				int i;
				for (i = 0; i < 4; i++)
				{
					move[i] = ' ';
				}
				num_moves = 0;
				lcd_message("Cleared.");
				continue;
			}
		
			int offset = 0; // From: first 2 chars, To: second 2 chars
			if (button == To)
			{
				offset = 2;	
			}			
		
			// get letter
			lcd_message("Specify letter A-H");
			button = get_button_press(side);
		
			if (button == To || button == From || button == Go || button == Clr)
			{
				lcd_message("Invalid letter.");
				continue;
			}			
			else if (button == A1)
			{
				move[offset] = 'a';
			}
			else if (button == B2)
			{
				move[offset] = 'b';
			}
			else if (button == C3)
			{
				move[offset] = 'c';
			}
			else if (button == D4)
			{
				move[offset] = 'd';
			}
			else if (button == E5)
			{
				move[offset] = 'e';
			}
			else if (button == F6)
			{
				move[offset] = 'f';
			}
			else if (button == G7)
			{
				move[offset] = 'g';
			}
			else if (button == H8)
			{
				move[offset] = 'h';
			}
			else
			{
				// error
				continue;
			}
		
			// get number
			lcd_message("Specify number 1-8");
			button = get_button_press(side);
		
			if (button == To || button == From || button == Go || button == Clr)
			{
				lcd_message("Invalid number.");
				continue;
			}
			else if (button == A1)
			{
				move[offset+1] = '1';
			}
			else if (button == B2)
			{
				move[offset+1] = '2';
			}
			else if (button == C3)
			{
				move[offset+1] = '3';
			}
			else if (button == D4)
			{
				move[offset+1] = '4';
			}
			else if (button == E5)
			{
				move[offset+1] = '5';
			}
			else if (button == F6)
			{
				move[offset+1] = '6';
			}
			else if (button == G7)
			{
				move[offset+1] = '7';
			}
			else if (button == H8)
			{
				move[offset+1] = '8';
			}
			else
			{
				// error input
				continue;
			}
		
			num_moves++;
		}
	
		while (1)
		{		
			lcd_message(move);
			lcd_message("Press 'Go' or 'Clear'");
			button = get_button_press(side);
			if (button == Clr)
			{
				// start over
				break;
			}
			else if (button == Go)
			{
				return move;
			}
		}		
	}		
}



/**
 * \brief sends a message to the lcd display of the user
 */
void lcd_message(char* text)
{
	debug(text);
	_delay_ms(1000);
}


/**
 * \brief sends a debug message to Arduino serial monitor.
 */
void debug(char *message)
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
	while( (USARTD0.STATUS & USART_DREIF_bm) == 0 ) {}
	USARTD0.DATA = '\n';
}	


/**
 * \brief returns an integer representing the button pressed by the user.
 *  definitions can be found in the header file.
 */
int get_button_press(int side)
{
	int mask = 0;

	// block while waiting for button press
	while (mask == 0x0)
	{
		mask = ~BUTTON_IOPORT_1.IN & BUTTON_BITMASK_1;
		mask |= (~BUTTON_IOPORT_2.IN & BUTTON_BITMASK_2) << BUTTON_BITPOSITION_2;
	}
	
	switch(mask)
	{
		case 1 << 0:
		return A1;
		case 1 << 1:
		return B2;
		case 1 << 2:
		return C3;
		case 1 << 3:
		return D4;
		case 1 << 4:
		return E5;
		case 1 << 5:
		return F6;
		case 1 << 6:
		return G7;
		case 1 << 7:
		return H8;
		default:
		return IO_ERROR;
	}
}

