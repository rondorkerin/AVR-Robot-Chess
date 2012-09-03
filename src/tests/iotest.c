/*
 * \file iotest.c
 *
 * \brief Behavioral test suite for I/O devices - includes those with their own modules such as motor driver
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 

#include "iotest.h"
#include "test.h"
#include "../io.h"
#include "../motordriver.h"
#include <conf.h>


/**
 *	\brief initializes IO devices for test
 */
void io_test_init()
{
	debug("initializing I/O devices");
	init_io();	
}


/**
 *	\brief this test repeatedly opens and closes the clamp with a 3 second delay in between
 */
void clamp_servo_test()
{
	debug("performing clamp servo test.");
	
	while(1)
	{
		debug("opening the clamp");
		open_clamp();
		// light up LEDs
		write_board_led_mask(0x00);
		_delay_ms(3000);
		
		debug("closing the clamp");
		close_clamp();
		write_board_led_mask(0xFF);
		_delay_ms(8000);	
	}	
}

/*
 * \brief this button waits for a button press and sends the resulting button press over the debug interface.
 * \note blinks LEDs if a button press was found as well.
 */
void button_press_test(uint8_t side)
{
	// NOTE: This test does not currently work
	while(1)
	{		
		uint8_t i = get_button_press(side);
		write_board_led_mask(i);
	}
}



/*
 * \brief moves x stepper and blinks some LEDs
 */
void x_stepper_test()
{
	while(1)
	{		
		// move
		move_x_stepper(300, 1);
		_delay_ms(1000);
		//move_x_stepper(300, 0);
		//_delay_ms(1000);			
	}
}