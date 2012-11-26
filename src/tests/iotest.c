/*
 * \file iotest.c
 *
 * \brief Behavioral test suite for I/O devices - includes those with their own modules such as motor driver
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 

#include "iotest.h"
#include "../io.h"
#include "../motordriver.h"
#include "../lcddriver.h"
#include <conf.h>


/**
 *	\brief initializes IO devices for test
 */
void io_test_init()
{
	//debug("initializing I/O devices");
	
	init_io();	
}


/**
 *	\brief this test repeatedly opens and closes the clamp with a 3 second delay in between
 */
void clamp_servo_test()
{
	
	while(1)
	{
		
		open_clamp();
		_delay_ms(3000);
		
		close_clamp();
		_delay_ms(3000);	
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
		int i = 0;
		for (i = 1; i < 4; i++)
		{
			move_x_stepper(i, 1);
			_delay_ms(1000);
			move_x_stepper(i, 0);
			_delay_ms(1000);
		}
					
	}
}

void y_stepper_test()
{
	while(1)
	{
		// move
		int i = 0;
		for (i = 1; i < 4; i++)
		{
			move_y_stepper(i, 1);
			_delay_ms(1000);
			move_y_stepper(i, 0);
			_delay_ms(1000);
		}
		
	}
}


void xy_stepper_test()
{
	while(1)
	{
		center_at_origin();
		delay_ms(3000);
		move_piece(1,1,1,8);
		delay_ms(3000);
		move_piece(1,8,8,8);
		delay_ms(3000);
		move_piece(8,8,1,8);
		delay_ms(3000);
		move_piece(1,8,1,1);
		delay_ms(3000);
	}	
}

void z_stepper_test()
{
	while(1)
	{
		move_z_stepper(0);
		_delay_ms(1000);
		move_z_stepper(1);
		_delay_ms(1000);
	}
}

void lcd_test()
{
	while(1)
	{
		lcd_message("abcdefghijklmnopqrstuvwxyz", Player1);
		//lcd_puts("jfdksljflds");
		_delay_ms(1000);
		//lcd_putstr_P("iifodsofjdsf");
	}
}


void led_test()
{
		
	init_io();
		
	refresh_led_grid();
	
	while(1)
	{
	
		
		delay_ms(1000);	
		/*
		led_set(1, 1, All);
		led_set(1, 2, All);
		_delay_ms(1000);
		led_set(2, 2, All);
		led_set(2, 3, All);
		_delay_ms(1000);
		led_set(3,3, All);
		led_set(3, 4, All);
		delay_ms(1000);
		
		led_clear(1, 1, Red);
		led_clear(1, 2, Red);
		_delay_ms(1000);
		led_clear(2, 2, Green);
		led_clear(2, 3, Green);
		_delay_ms(1000);
		led_clear(3,3, Blue);
		led_clear(3, 4, Blue);
		*/
	}
	
}