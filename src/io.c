
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
#include "lcddriver.h"
#include <stdio.h>


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
	
	PORTD.PIN0CTRL = PORT_ISC_RISING_gc;
	
	// configure buttons
	gpio_configure_pin(HMI1_BUTTON_PRESSED_PIN1, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	gpio_configure_pin(HMI_FUNCTION_BUTTON_PRESSED_PIN, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);

	gpio_configure_pin(HMI1_BUTTON_PIN1, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	gpio_configure_pin(HMI1_BUTTON_PIN2, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	gpio_configure_pin(HMI1_BUTTON_PIN3, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);	
	
	gpio_configure_pin(HMI_FUNCTION_BUTTON_PIN1, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	gpio_configure_pin(HMI_FUNCTION_BUTTON_PIN2, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	gpio_configure_pin(HMI_FUNCTION_BUTTON_PIN3, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	
	// configure LCD screen
	
	gpio_configure_pin(HMI1_LCD_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LED_CLOCK_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LED_DATA_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LED_LOAD_PIN, IOPORT_DIR_OUTPUT);
	
	// Initialize LCD 1 serial
	/*
	Cancelled due to our serial LCD breaking!
	USARTD0_BAUDCTRLB = 204;
	USARTD0_BAUDCTRLA = 245;
	USARTD0_CTRLB = USART_TXEN_bm; //0x18; //enable tx rx
	USARTD0.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc; // Set USART Format to 8bit, no parity, 1 stop bit
	*/
	
	// Initialize LCD 2 serial
	USARTD0_BAUDCTRLB = 204;
	USARTD0_BAUDCTRLA = 245;
	USARTD0_CTRLB = USART_TXEN_bm; //0x18; //enable tx rx
	USARTD0.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc; // Set USART Format to 8bit, no parity, 1 stop bit
	
	// initialize LCD 1
	/*
	lcd_putchar(0xFE, Player1);
	lcd_putchar(0x41, Player1);
	lcd_putchar(0xFE, Player1);
	lcd_putchar(0x51, Player1);
	lcd_putchar(0xFE, Player1);
	lcd_putchar(0x52, Player1);
	lcd_putchar(50, Player1);
	lcd_putchar(0xFE, Player1);
	lcd_putchar(0x53, Player1);
	lcd_putchar(4, Player1);
	*/
	// initialize LCD 2
	
	lcd2_putchar(0xFE, Player2); 
	lcd2_putchar(0x41, Player2); 
	lcd2_putchar(0xFE, Player2); 
	lcd2_putchar(0x51, Player2); 
	lcd2_putchar(0xFE, Player2); 
	lcd2_putchar(0x52, Player2); 
	lcd2_putchar(50, Player2); 
	lcd2_putchar(0xFE, Player2); 
	lcd2_putchar(0x53, Player2); 
	lcd2_putchar(4, Player2);
	
	
	gpio_configure_pin(LCD_RS_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LCD_ENABLE_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LCD_DATA0_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LCD_DATA1_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LCD_DATA2_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LCD_DATA3_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LCD_DATA4_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LCD_DATA5_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LCD_DATA6_PIN, IOPORT_DIR_OUTPUT);
	gpio_configure_pin(LCD_DATA7_PIN, IOPORT_DIR_OUTPUT);
	
	lcd_init();
	// clear display
	lcd_clear_and_home();
	
	// initialize LED grid
	led_send(LED_REG_SCANLIMIT, 0x07, All);
	led_send(LED_REG_DECODEMODE, 0x00, All);  // using an led matrix (not digits)
	led_send(LED_REG_SHUTDOWN, 0x01, All);    // not in shutdown mode
	led_send(LED_REG_DISPLAYTEST, 0x00, All); // no display test 
	
	for (int reg = 1; reg <= 8; reg++)
	{
		red_led_map[reg] = 0;
		green_led_map[reg] = 0;
		blue_led_map[reg] = 0;
	}
	
	// clear LED grid
	for (int x = 1; x <= 8; x++)
	{
		for (int y = 1; y <= 8; y++)
		{
			led_clear(x, y, All);
		}
	}						
			
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
		char* move = malloc(sizeof(char) * 5);
		move[4] = '\0';
		int button;
	
		// get user to press From button
		while (1)
		{
			lcd_message("Press 'From' to     begin your move", side);
			button = get_button_press(side);
			if (button == From)
			{
				break;
			}
		}
		
		// get letter
		while (1)
		{
			lcd_message("Specify letter for  source tile", side);
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
			lcd_message("Specify number for  source tile", side);
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
		
		blink_led(move[1] - '1' + 1, move[0]-'a' + 1, Red);
		
		// get user to press to button
		while (1)
		{
			lcd_message("Press 'To' to begin your move", side);
			button = get_button_press(side);
			if (button == To)
			{
				break;
			}
		}
		
		// get move to location letter
		while (1)
		{
			lcd_message("Specify letter for  destination tile", side);
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
			lcd_message("Specify number for  destination tile", side);
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
		
		blink_led(move[3] - '1' + 1,move[2]-'a' + 1, Red);
		
		char response[80];
		sprintf(response, "You chose [%s].   Hit Go if correct", move);
		
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
	unsigned char resetCommand[3] = {0xFE, 0x51, '\0'}; // clear screen
	
	if (side == Player1)
	{
		lcd_clear_and_home();
		lcd_write_string_0(msg);	
	}
	else if (side == Player2)
	{
		lcd2_putstring(resetCommand, side);
		lcd2_putstring(msg, side);
	}
	
}


/**
 * \brief sends a message to the lcd display of the user
 * \note delay param is used incase we have multiple lcd messages in series without
 * requiring a button press
 * \param delay delay in ms
 */
void lcd_message_delay(unsigned char* msg, int side, int delay)
{
	unsigned char resetCommand[3] = {0xFE, 0x51, '\0'}; // clear screen

	if (side == Player1)
	{
		lcd_clear_and_home();
		lcd_write_string_0(msg);
	}
	else if (side == Player2)
	{
		lcd2_putstring(resetCommand, side);
		lcd2_putstring(msg, side);
	}
	delay_ms(delay);
}


/**
 * Light up an LED at a certain position (Unable to light up all LEDs at once)
 * \param posx x position in [1, 8]
 * \param posy y position in [1, 8]
 * \param rgb rgb value {r = 0, g = 1, b = 2, all = 3}
 */
void led_set(uint8_t posx, uint8_t posy, uint8_t rgb)
{
    unsigned char reg = posx;
    unsigned char col = 1 << (8-posy);
	unsigned char sendVal;
	if (rgb == Red)
	{
		red_led_map[reg] |= col;
		sendVal = red_led_map[reg];
		led_send(reg, sendVal, Red);
		//led_clear(posx, posy, Blue);
		//led_clear(posx, posy, Green);
	}
	if (rgb == Blue || rgb == All)
	{
		blue_led_map[reg] |= col;
		sendVal = blue_led_map[reg];
		led_send(reg, sendVal, Blue);
		//led_clear(posx, posy, Red);
		//led_clear(posx, posy, Green);
	}
	if (rgb == Green || rgb == All)
	{
		green_led_map[reg] |= col;
		sendVal = green_led_map[reg];	
		led_send(reg, sendVal, Green);
		//led_clear(posx, posy, Blue);
		//led_clear(posx, posy, Red);
	}		
}


void led_clear(uint8_t posx, uint8_t posy, uint8_t rgb)
{
	unsigned char reg = posx;
	unsigned char col = 1 << (8 - posy);
	
	unsigned char sendVal;
	if (rgb == Red || rgb == All)
	{
		red_led_map[reg] &= ~col;
		sendVal = red_led_map[reg];
		led_send(reg, sendVal, Red);
	}
	if (rgb == Blue || rgb == All)
	{
		blue_led_map[reg] &= ~col;
		sendVal = blue_led_map[reg];
		led_send(reg, sendVal, Blue);
	}
	if (rgb == Green || rgb == All)
	{
		green_led_map[reg] &= ~col;
		sendVal = green_led_map[reg];
		led_send(reg, sendVal, Green);
	}
}


/**
 * \brief returns an integer representing the button pressed by the user.
 */
uint8_t get_button_press(uint8_t side)
{
	// block while waiting for button press
	while( gpio_pin_is_low(HMI1_BUTTON_PRESSED_PIN1) && gpio_pin_is_low(HMI_FUNCTION_BUTTON_PRESSED_PIN)){}

	// decode binary to decimal
	int selection;
	
	if (gpio_pin_is_high(HMI1_BUTTON_PRESSED_PIN1))
	{
		selection = 0;
		if (gpio_pin_is_low(HMI1_BUTTON_PIN1))
		{
			selection += 4;	
		}		
		if (gpio_pin_is_low(HMI1_BUTTON_PIN2))
		{
			selection += 2;	
		}		
		if (gpio_pin_is_low(HMI1_BUTTON_PIN3))
		{
			selection += 1;	
		}
	}				
	
	if (gpio_pin_is_high(HMI_FUNCTION_BUTTON_PRESSED_PIN))
	{
		selection = 8;
		if (gpio_pin_is_low(HMI_FUNCTION_BUTTON_PIN1))
		{
			selection += 4;
		}
		if (gpio_pin_is_low(HMI_FUNCTION_BUTTON_PIN2))
		{
			selection += 2;
		}
		if (gpio_pin_is_low(HMI_FUNCTION_BUTTON_PIN3))
		{
			selection += 1;
		}
	}
	
	// delay 250 ms so the user doesn't accidently "hold" the button for multiple button presses.
	// Otherwise we have to test for edges...
	
	while( gpio_pin_is_high(HMI1_BUTTON_PRESSED_PIN1) || gpio_pin_is_high(HMI_FUNCTION_BUTTON_PRESSED_PIN)){}
	_delay_ms(500);

	switch(selection)
	{
		case 7:
		return A1;
		case 6:
		return B2;
		case 5:
		return C3;
		case 4:
		return D4;
		case 3:
		return E5;
		case 2:
		return F6;
		case 1:
		return G7;
		case 0:
		return H8;
		case 8:
		return Clr;
		case 9:
		return Go;
		case 10:
		return To;
		case 11:
		return From;
		case 12:
		return Clr;
		case 13:
		return Go;
		case 14:
		return To;
		case 15:
		return From;
	}
	return ButtonError;
}


/**
 * \brief moves a piece from location to location
 *        assumes all locations are valid
 */
void move_piece(uint8_t from_posx, uint8_t from_posy, uint8_t to_posx, uint8_t to_posy)
{
		move_crane(from_posx, from_posy);
		open_clamp();
		move_z_stepper(Down);
		_delay_ms(1000);
		close_clamp();
		_delay_ms(1000);
		move_z_stepper(Up);
		move_crane(to_posx, to_posy);
		move_z_stepper(Down);
		_delay_ms(1000);
		open_clamp();
		move_z_stepper(Up);
}

/************************************************************************/
/* Refreshes LED grid (call after any LED function)                                                                     */
/************************************************************************/
void refresh_led_grid()
{
	for (int x = 1; x <= 8; x++)
	{
		for (int y = 1; y <= 8; y++)
		{	
			if (!(x % 2 == 1 && y % 2 == 1 || x % 2 == 0 && y % 2 == 0))
			{			
				led_clear(x, y, Red);
			}				
		}
	}
	
	for (int x = 1; x <= 8; x++)
	{
		for (int y = 1; y <= 8; y++)
		{
			if (x % 2 == 1 && y % 2 == 1 || x % 2 == 0 && y % 2 == 0)
			{
				led_set(x, y, Red);
			}
		}
	}					
		
}

void blink_led(uint8_t posx, uint8_t posy, uint8_t rgb)
{
	for (int i = 0; i < 5; i++)
	{
		led_set(posx, posy, rgb);
		delay_ms(1500);
		led_clear(posx, posy, rgb);
		delay_ms(1500);	
	}
	refresh_led_grid();
}


void lcd2_putchar(unsigned char data, int side)
{
	
	//lcd_putc(data);
	if (side == Player2)
	{
		while ( !(USARTD0.STATUS & USART_DREIF_bm) ){}
		/* Put data into buffer, sends the data */
		USARTD0_DATA = data;
		
	}/*
	else if (side == Player2)
	{
		while ( !(USARTC0.STATUS & USART_DREIF_bm) ){}

		USARTC0_DATA = data;
	}*/

}


/**
 * \brief writes the specified string to the LCD
 * \note since strings automatically print on lines 1, 3, 2, 4,
 *		 we need to manually set the cursor every 20 characters.
 *	     later we might want to create an algorithm to wrap text.
 */

void lcd2_putstring(unsigned char* pcString, int side)
{
	
	int i = 0; // character # in sequence
	int j = 0; // current display line #
	while(*pcString)
	{
		if (i%20 == 0)
		{
			// set cursor to new line 
			lcd2_putchar(0xFE, side);
			lcd2_putchar(0x45, side);
			int line_addr = j == 0 ? 0 : j == 1 ? 0x40 : j == 2 ? 0x14 : 0x54;
			lcd2_putchar(line_addr, side);
			j++;
		}
		
		lcd2_putchar(*pcString++, side);
		i++;
	}
	
}


void led_putchar(unsigned char data)
{
	 uint8_t i = 0x08;
	 unsigned char mask;
	 while(i > 0) 
	 {
		mask = 0x01 << (i - 1);      // get bitmask
		gpio_set_pin_low(LED_CLOCK_PIN);
		_delay_us(50);
		if (data & mask)
		{
			gpio_set_pin_high(LED_DATA_PIN);
		}
		else
		{
			gpio_set_pin_low(LED_DATA_PIN);
		}
		_delay_us(50);
		gpio_set_pin_high(LED_CLOCK_PIN);  
		_delay_us(50);
		--i;                         // move to lesser bit
	}
}


void led_send(unsigned char reg, unsigned char col, uint8_t rgb)
{
	
	gpio_set_pin_low(LED_LOAD_PIN);
	_delay_us(100);
	
	if (rgb = All)
	{
		// send the same command to every LED
		for (int i = 0; i < All; i++)
		{
			led_putchar(reg);
			_delay_us(100);
			led_putchar(col);
			_delay_us(100);
		}
	}
	else 
	{		
		// do a NOP for any LED before the one we picked
		for (int i = All; i > rgb; i--)
		//for (int i = 0; i < rgb; i++)
		{
			// NOP
			led_putchar(0x00);
			_delay_us(100);
			led_putchar(0x00);
			_delay_us(100);
		}		
		
		led_putchar(reg);
		_delay_us(100);
		led_putchar(col);
		_delay_us(100);
		
		for (int i = rgb-1; i >= 1; i--)
		//for (int i = rgb + 1; i < All; i++)
		{
			// NOP
			led_putchar(0x00);
			_delay_us(100);
			led_putchar(0x00);
			_delay_us(100);
		}
	
	}
	
	gpio_set_pin_high(LED_LOAD_PIN);
	_delay_us(100);		
}


void delay_ms(int count) 
{
	while(count--) 
	{
		_delay_us(50);
	}
}

/*
* Translates a character string of the form 'A1B2' to a position array
* \note A1 = (3, 0) because of the 2 graveyard squares. 
*/ 
uint8_t* get_positions(char* move)
{
	uint8_t* pos = malloc(sizeof(uint8_t) * 4);
	pos[0] = (uint8_t)move[0] - 0x61 + 3;
	pos[1] = (uint8_t)move[1] - 0x31;
	pos[2] = (uint8_t)move[2] - 0x61 + 3;
	pos[3] = (uint8_t)move[3] - 0x31;
	
	return pos;
}


/*
* Translates a character string of the form 'A1B2' to a array of indices for the LED matrix
* \note A1 = (1, 1)
*/
uint8_t* get_ledindices(char* move)
{
	uint8_t* pos = malloc(sizeof(uint8_t) * 4);
	pos[0] = (uint8_t)move[0] - 0x61 + 1;
	pos[1] = (uint8_t)move[1] - 0x31 + 1;
	pos[2] = (uint8_t)move[2] - 0x61 + 1;
	pos[3] = (uint8_t)move[3] - 0x31 + 1;
	
	return pos;
}