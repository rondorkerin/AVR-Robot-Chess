
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
#include <stdio.h>


/**
 * \brief initializes i/o devices
 */
void init_io()
{
	//PORTC.DIRSET = 0xFF;
	// set LEDs to output
	//PORTE.DIRSET = 0xFF;
	
	// set clock to 32 mhz
	CCP = CCP_IOREG_gc;              // disable register security for oscillator update
	OSC.CTRL = OSC_RC32MEN_bm;       // enable 32MHz oscillator
	while(!(OSC.STATUS & OSC_RC32MRDY_bm)); // wait for oscillator to be ready
	CCP = CCP_IOREG_gc;              // disable register security for clock update
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // switch to 32MHz clock
	
	_delay_ms(5000);
	
	// configure buttons
	gpio_configure_pin(HMI1_BUTTON_PRESSED_PIN, IOPORT_DIR_INPUT | IOPORT_PULL_UP);
	PORTD.PIN0CTRL = PORT_ISC_RISING_gc;
	gpio_configure_pin(HMI1_BUTTON_PIN1, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	gpio_configure_pin(HMI1_BUTTON_PIN2, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	gpio_configure_pin(HMI1_BUTTON_PIN3, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);	
	
	// configure LCD screen
	
	gpio_configure_pin(HMI1_LCD_PIN, IOPORT_DIR_OUTPUT);
	
	
	//int bsel = 245; // 9600 @ 32Mhz as calculated from ProtoTalk Calc
	//uint8_t bscale = 207;
	//USARTD0.BAUDCTRLA = (uint8_t) bsel;
	//USARTD0.BAUDCTRLB = (bscale << 4) | (bsel >> 8);
	//USARTD0.BAUDCTRLA = 12;
	// enable only TX
	//USARTD0.CTRLB |= USART_TXEN_bm;
	
	USARTD0_BAUDCTRLB = 204;
	USARTD0_BAUDCTRLA = 245;
	//USARTD0_CTRLA = 0x30; //receive interrupt
	USARTD0_CTRLB = USART_TXEN_bm;//0x18; //enable tx rx
	//USARTD0_CTRLC = 0x03;
	USARTD0.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc; // Set USART Format to 8bit, no parity, 1 stop bit
	//PORTD_DIR |= 0x08;
	//PORTF_OUT = 0x08;
	//PORTD.DIRCLR = 0x04; 
	//set PMIC high level enable
	//PMIC.CTRL |= 0x04;
	
	// initialize LCD
	//unsigned char initCommands[11] = {0xFE, 0x41, 0xFE, 0x51, 0xFE, 0x52, 50, 0xFE, 0x53, 4, '\0'};
	//lcd_putstring(initCommands, Player1);
	lcd_putchar(0xFE, Player1); lcd_putchar(0x41, Player1); lcd_putchar(0xFE, Player1); lcd_putchar(0x51, Player1); lcd_putchar(0xFE, Player1); lcd_putchar(0x52, Player1); lcd_putchar(50, Player1); lcd_putchar(0xFE, Player1); lcd_putchar(0x53, Player1); lcd_putchar(4, Player1);
	//lcd_putstring(initCommands, Player2);
	
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
		char move[5];
		move[4] = '\0';
		int button;
	
		// get user to press From button
		while (1)
		{
			lcd_message("Please press 'From' to begin your move", side);
			button = get_button_press(side);
			if (button == From)
			{
				break;
			}
		}
		
		// get letter
		while (1)
		{
			lcd_message("Specify letter corresponding to the piece you wish to move", side);
			button = get_button_press(side);
			
			if (button == To || button == From || button == Go || button == Clr)
			{
				lcd_message_delay("Invalid letter.", side, 3000);
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
			lcd_message("Specify number corresponding to the piece you wish to move", side);
			button = get_button_press(side);
			
			if (button == To || button == From || button == Go || button == Clr)
			{
				lcd_message_delay("Invalid number.", side, 3000);
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
			lcd_message("Please press 'To' to begin your move", side);
			button = get_button_press(side);
			if (button == To)
			{
				break;
			}
		}
		
		// get move to location letter
		while (1)
		{
			lcd_message("Specify letter corresponding to the piece you wish to move", side);
			button = get_button_press(side);
			
			if (button == To || button == From || button == Go || button == Clr)
			{
				lcd_message_delay("Invalid letter.", side, 3000);
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
			lcd_message("Specify number corresponding to the piece you wish to move to", side);
			button = get_button_press(side);
			
			if (button == To || button == From || button == Go || button == Clr)
			{
				lcd_message_delay("Invalid number.", side, 3000);
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
		
		char response[80];
		sprintf(response, "You chose [%s]. Is this correct? Press 'Go' or 'Clear'", move);
		
		// get user confirmation
		while (1)
		{
			lcd_message(response, side);
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
			else
			{
				continue;	
			}
		}
	}
	return "";
}

/**
 * \brief sends a message to the lcd display of the user
 */
void lcd_message(unsigned char* msg, int side)
{
	//unsigned char resetCommand[3] = {0xFE, 0x51, '\0'}; // clear screen
	//lcd_putstring(resetCommand, side);
	lcd_putchar(0xFE, side);
	lcd_putchar(0x51, side);
	lcd_putstring(msg, side);
}


/**
 * \brief sends a message to the lcd display of the user
 * \note delay param is used incase we have multiple lcd messages in series without
 * requiring a button press
 * \param delay delay in ms
 */
void lcd_message_delay(unsigned char* msg, int side, int delay)
{
	//unsigned char resetCommand[3] = {0xFE, 0x51, '\0'}; // clear screen
	//lcd_putstring(resetCommand, side);
	lcd_putchar(0xFE, side);
	lcd_putchar(0x51, side);
	lcd_putstring(msg, side);
	delay_ms(delay);
}


/**
 * \brief activates an LED given by the integer position
 * \param position position in [0,63] on board, 
 */
void activate_led(uint8_t posx, uint8_t posy)
{
	if (posx == 0)
		PORTE.OUT = 0xff;	
	else
		PORTE.OUT = 0x00;
}


/**
 * \brief returns an integer representing the button pressed by the user.
 */
uint8_t get_button_press(uint8_t side)
{
	// block while waiting for button press
	// \note eventually we will && with the other HMI button pressed pins
	//while( gpio_pin_is_low(HMI1_BUTTON_PRESSED_PIN) && gpio_pin_is_low(HMI_SHARED_BUTTON_PRESSED_PIN)){}
	while( gpio_pin_is_low(HMI1_BUTTON_PRESSED_PIN)){}
	
	// decode binary to decimal
	int selection = 0;
	//if (gpio_pin_is_high(HMI1_BUTTON_PRESSED_PIN))
	//{
	if (gpio_pin_is_low(HMI1_BUTTON_PIN1))
	{
		selection += 1;	
	}		
	if (gpio_pin_is_low(HMI1_BUTTON_PIN2))
	{
		selection += 2;	
	}		
	if (gpio_pin_is_low(HMI1_BUTTON_PIN3))
	{
		selection += 4;	
	}		

	// delay 250 ms so the user doesn't accidently "hold" the button for multiple button presses.
	// Otherwise we have to test for edges...
	_delay_ms(250);

	switch(selection)
	{
		case 0:
		return A1;
		case 1:
		return B2;
		case 2:
		return C3;
		case 3:
		return D4;
		case 4:
		return E5;
		case 5:
		return F6;
		case 6:
		return G7;
		case 7:
		return H8;
	}
	return ButtonError;
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


void lcd_putchar(unsigned char TXdata, int side)
{

	while ( !(USARTD0.STATUS & USART_DREIF_bm) ){}

	/* Put data into buffer, sends the data */
	USARTD0_DATA = TXdata;

}


/**
 * \brief writes the specified string to the LCD
 * \note since strings automatically print on lines 1, 3, 2, 4,
 *		 we need to manually set the cursor every 20 characters.
 *	     later we might want to create an algorithm to wrap text.
 */
void lcd_putstring(unsigned char* pcString, int side)
{
	int i = 0; // character # in sequence
	int j = 0; // current display line #
	while(*pcString)
	{
		if (i%20 == 0)
		{
			// set cursor to new line 
			lcd_putchar(0xFE, side);
			lcd_putchar(0x45, side);
			int line_addr = j == 0 ? 0 : j == 1 ? 0x40 : j == 2 ? 0x14 : 0x54;
			lcd_putchar(line_addr, side);
			j++;
		}
		
		lcd_putchar(*pcString++, side);
		i++;
	}
}


void delay_ms(int count) {
	while(count--) {
		_delay_ms(1);

	}
}
