
/**
 * \file io.c
 * \brief "view" component of model view controller framework. Implements peripheral I/O functionality
 *
 * Created: 8/21/2012 8:12:31 PM
 *  Author: nul4x3r0000
 */ 

#include "io.h"
#include "tests/iotest.h"
#include "tests/test.h"
#include "motordriver.h"


/**
 * \brief initializes i/o devices
 */
void init_io()
{
	// set clock to 32 mhz
	CCP = CCP_IOREG_gc;              // disable register security for oscillator update
	OSC.CTRL = OSC_RC32MEN_bm;       // enable 32MHz oscillator
	while(!(OSC.STATUS & OSC_RC32MRDY_bm)); // wait for oscillator to be ready
	CCP = CCP_IOREG_gc;              // disable register security for clock update
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // switch to 32MHz clock
	
	// init servo timers
	init_motordriver();
}



/**
 * \brief gets a player move in string format
 * \note example: 'a2a4'
 */
char* get_player_move(uint8_t side)
{
	while (1)
	{
		char move[4];
		int button;
	
		// get user to press From button
		while (1)
		{
			lcd_message("Please press 'From' to begin your move");
			button = get_button_press(side);
			if (button == From)
			{
				break;
			}
		}
		
		// get letter
		while (1)
		{
			lcd_message("Specify letter corresponding to the piece you wish to move");
			button = get_button_press(side);
			
			if (button == To || button == From || button == Go || button == Clr)
			{
				lcd_message("Invalid letter.");
				continue;
			}
			else if (button == A1)
			{
				move[0] = 'a';
			}
			else if (button == B2)
			{
				move[0] = 'b';
			}
			else if (button == C3)
			{
				move[0] = 'c';
			}
			else if (button == D4)
			{
				move[0] = 'd';
			}
			else if (button == E5)
			{
				move[0] = 'e';
			}
			else if (button == F6)
			{
				move[0] = 'f';
			}
			else if (button == G7)
			{
				move[0] = 'g';
			}
			else if (button == H8)
			{
				move[0] = 'h';
			}
			else
			{
				// error
				continue;
			}
			break;
		}
		
		// get move from location number
		while (1)
		{
			lcd_message("Specify number corresponding to the piece you wish to move");
			button = get_button_press(side);
			
			if (button == To || button == From || button == Go || button == Clr)
			{
				lcd_message("Invalid number.");
				continue;
			}
			else if (button == A1)
			{
				move[1] = '1';
			}
			else if (button == B2)
			{
				move[1] = '2';
			}
			else if (button == C3)
			{
				move[1] = '3';
			}
			else if (button == D4)
			{
				move[1] = '4';
			}
			else if (button == E5)
			{
				move[1] = '5';
			}
			else if (button == F6)
			{
				move[1] = '6';
			}
			else if (button == G7)
			{
				move[1] = '7';
			}
			else if (button == H8)
			{
				move[1] = '8';
			}
			else
			{
				// error input
				continue;
			}
			break;
		}
		
		// get user to press to button
		while (1)
		{
			lcd_message("Please press 'To' to begin your move");
			button = get_button_press(side);
			if (button == To)
			{
				break;
			}
		}
		
		// get move to location letter
		while (1)
		{
			lcd_message("Specify letter corresponding to the piece you wish to move");
			button = get_button_press(side);
			
			if (button == To || button == From || button == Go || button == Clr)
			{
				lcd_message("Invalid letter.");
				continue;
			}
			else if (button == A1)
			{
				move[2] = 'a';
			}
			else if (button == B2)
			{
				move[2] = 'b';
			}
			else if (button == C3)
			{
				move[2] = 'c';
			}
			else if (button == D4)
			{
				move[2] = 'd';
			}
			else if (button == E5)
			{
				move[2] = 'e';
			}
			else if (button == F6)
			{
				move[2] = 'f';
			}
			else if (button == G7)
			{
				move[2] = 'g';
			}
			else if (button == H8)
			{
				move[2] = 'h';
			}
			else
			{
				// error
				continue;
			}
			break;
		}
		
		// get move to location number
		while (1)
		{
			// get number
			lcd_message("Specify number corresponding to the piece you wish to move to");
			button = get_button_press(side);
			
			if (button == To || button == From || button == Go || button == Clr)
			{
				lcd_message("Invalid number.");
				continue;
			}
			else if (button == A1)
			{
				move[3] = '1';
			}
			else if (button == B2)
			{
				move[3] = '2';
			}
			else if (button == C3)
			{
				move[3] = '3';
			}
			else if (button == D4)
			{
				move[3] = '4';
			}
			else if (button == E5)
			{
				move[3] = '5';
			}
			else if (button == F6)
			{
				move[3] = '6';
			}
			else if (button == G7)
			{
				move[3] = '7';
			}
			else if (button == H8)
			{
				move[3] = '8';
			}
			else
			{
				// error input
				continue;
			}
			break;
		}
		
		// get user confirmation
		while (1)
		{
			lcd_message(move);
			lcd_message("Is this correct? Press 'Go' or 'Clear'");
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
 * \brief activates an LED given by the integer position
 * \param position position in [0,63] on board, 
 */
void activate_led(uint8_t posx, uint8_t posy)
{
	if (posx == 0)
		LED_IOPORT.OUT = 0xff;	
	else
		LED_IOPORT.OUT = 0x00;
}


/**
 * \brief returns an integer representing the button pressed by the user.
 * \note buttons in the low position are pressed, in the high position they are not pressed
 */
uint8_t get_button_press(uint8_t side)
{
	uint8_t mask = 0x00;

	// block while waiting for button press
	while (mask == 0x00)
	{
		mask = ~BUTTON_IOPORT_1.IN & BUTTON_BITMASK_1;
		mask |= (~BUTTON_IOPORT_2.IN & BUTTON_BITMASK_2) << BUTTON_BITPOSITION_2;
	}
	return mask;
	
	/* Use this kind of stuff when reading real values from buttons
	
		uint8_t mask = 0xF7;

		// block while waiting for button press
		while (mask == 0xF7)
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
		}
		return ButtonError;
		*/
}


/**
 * \brief moves a piece from location to location
 */
void move_piece(uint8_t from_posx, uint8_t from_posy, uint8_t to_posx, uint8_t to_posy)
{
		center_at_origin();
		/*
		move_x_stepper( from_posx );
		move_y_stepper( from_posy );
		open_clamp();
		move_z_stepper(0);
		close_clamp();
		move_z_stepper(1);
		center_at_origin();
		move_x_stepper( to_posx );
		move_y_stepper( to_posy );
		move_z_stepper(0);
		open_clamp();
		move_z_stepper(1);
		center_at_origin();
		*/
}