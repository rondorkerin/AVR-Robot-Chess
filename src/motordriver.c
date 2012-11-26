/**
 * \file motordriver.c
 *
 * \brief Implementation of motor driving routines 
 * \note Includes servos, stepper motors, and various sensors
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 

#include "motordriver.h"
#include "io.h"
#include <conf.h>
#include <asf.h>


/** 
 *  \brief This function initializes timers for use as servo PWM signal drivers.
 *
 *	\note the servo must have a signal pin on portC: 0 and it must be externally powered with a ground tied to 
 *	      the ground coming out of the port.
 *
 */
void init_motordriver()
{	
	posx = 3; // at A1
	posy = 0;
	
	// init servo
	gpio_configure_pin(SERVO_PWM_PIN, IOPORT_DIR_OUTPUT | IOPORT_MODE_TOTEM);
	gpio_configure_pin(X_STEPPER_EN_PIN, IOPORT_DIR_OUTPUT  | IOPORT_MODE_TOTEM);
	gpio_configure_pin(X_STEPPER_CLK_PIN, IOPORT_DIR_OUTPUT | IOPORT_MODE_TOTEM);
	gpio_configure_pin(X_STEPPER_CW_PIN, IOPORT_DIR_OUTPUT | IOPORT_MODE_TOTEM);
	gpio_configure_pin(Y_STEPPER_EN_PIN, IOPORT_DIR_OUTPUT | IOPORT_MODE_TOTEM);
	gpio_configure_pin(Y_STEPPER_CLK_PIN, IOPORT_DIR_OUTPUT | IOPORT_MODE_TOTEM);
	gpio_configure_pin(Y_STEPPER_CW_PIN, IOPORT_DIR_OUTPUT | IOPORT_MODE_TOTEM);
	gpio_configure_pin(Z_STEPPER_EN_PIN, IOPORT_DIR_OUTPUT | IOPORT_MODE_TOTEM);
	gpio_configure_pin(Z_STEPPER_CLK_PIN, IOPORT_DIR_OUTPUT | IOPORT_MODE_TOTEM);
	gpio_configure_pin(Z_STEPPER_CW_PIN, IOPORT_DIR_OUTPUT | IOPORT_MODE_TOTEM);
	
	gpio_configure_pin(MOTOR_HOME_PIN_X, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	gpio_configure_pin(MOTOR_HOME_PIN_Y, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	gpio_configure_pin(MOTOR_HOME_PIN_Z, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	
	TCC0.CTRLA = TC_CLKSEL_DIV1024_gc; // 32M/1024=32K
	//TCC0.CTRLB |= TC0_CCAEN_bm | TC0_WGMODE0_bm; // enable compare capture for pin 0
	TCC0.CTRLB |= TC0_CCAEN_bm | TC0_WGMODE1_bm | TC0_WGMODE0_bm; // enable compare capture for pin 0

	TCC0.PER =  654; //took 2^15*.02 - 1 approx 20ms
}


/**
 *  \brief Sets the clamp to open position
 */
void open_clamp()
{
	// warning: this is kind of dangerous because it can update CCA in the middle of a pulse.
	// but I'm not too worried since the chance is 1/20 of it doing that and it will reach a steady state
	int min = 33;
	int max = 65; //49
	TCC0.CCA= 65; // 654/20 = 33	
}


/**
 *  \brief Sets the clamp to closed position
 */
void close_clamp()
{
	// warning: this is kind of dangerous because it can update CCA in the middle of a pulse.
	// but I'm not too worried since the chance is 1/20 of it doing that and it will reach a steady state
	TCC0.CCA = 33;	//655/10 should be 33
}


/**
 *  \brief Moves x stepper to specified location in the board 
 *  \note Control is on pin1 of port C
 *  \param steps number of steps to move
 *  \param direction 1 for clockwise, 0 for counter clockwise. 
 */
Bool move_x_stepper(uint8_t squares, uint8_t direction)
{
	gpio_set_pin_high(X_STEPPER_EN_PIN);
	
	if (direction == CW)
	{
		gpio_set_pin_high(X_STEPPER_CW_PIN);
	}
	else if (direction == CCW)
	{
		gpio_set_pin_low(X_STEPPER_CW_PIN);
	}
	
	
	// find the number of squares and go that many steps
	int num_steps = STEPS_PER_SQUARE * squares;
	
	uint32_t i;
	for (i = 0; i < num_steps; i++)
	{		
		gpio_set_pin_high(X_STEPPER_CLK_PIN); //PIN1_bm;
		_delay_ms(6);
		
		gpio_set_pin_low(X_STEPPER_CLK_PIN);
		_delay_ms(6);
	}
	
	gpio_set_pin_low(X_STEPPER_EN_PIN);
	
	return true;
}


/**
 *  \brief Moves y stepper to specified location in the board 
 *  \note +0Control is on pin3 of port C
 *  \param steps number of steps to move
 *  \param direction 1 for clockwise, 0 for counter clockwise. 
 */
Bool move_y_stepper(uint8_t squares, uint8_t direction)
{	
	gpio_set_pin_high(Y_STEPPER_EN_PIN);
	
	if (direction == CW)
	{
		gpio_set_pin_high(Y_STEPPER_CW_PIN);
	}
	else if (direction == CCW)
	{
		gpio_set_pin_low(Y_STEPPER_CW_PIN);
	}
	
	
	// find the number of squares and go that many steps
	int num_steps = Y_STEPS_PER_SQUARE * squares;
	uint32_t i;
	for (i = 0; i < num_steps; i++)
	{
		gpio_set_pin_high(Y_STEPPER_CLK_PIN); //PIN1_bm;
		_delay_ms(18);
		
		gpio_set_pin_low(Y_STEPPER_CLK_PIN);
		_delay_ms(18);
	}
	
	gpio_set_pin_low(Y_STEPPER_EN_PIN);
	
	return true;
}


/**
 *  \brief Moves z stepper to specified location in the board
 *  \note Control is on pin4 of port C
 *  \param direction 0 means lower the crane and 1 means raise the crane
 */
Bool move_z_stepper(uint8_t direction)
{
	gpio_set_pin_high(Z_STEPPER_EN_PIN);
	
	int num_steps;
	
	if (direction == Up)
	{
		gpio_set_pin_low(Z_STEPPER_CW_PIN);
		// go up until we hit the limit switch
		num_steps = Z_STEPPER_STEPS * 2;
	}
	else if (direction == Down)
	{
		gpio_set_pin_high(Z_STEPPER_CW_PIN);
		num_steps = Z_STEPPER_STEPS;
	}
		
	uint32_t i;
	for (i = 0; i < num_steps; i++)
	{
		//
		// POLL for sensor feedback from motor controller
		//
		if (gpio_pin_is_high(MOTOR_HOME_PIN_Z) && direction == Up)
		{
			return;
		}			
			
		gpio_set_pin_high(Z_STEPPER_CLK_PIN); //PIN1_bm;
		_delay_ms(1);
			
		gpio_set_pin_low(Z_STEPPER_CLK_PIN);
		_delay_ms(1);
	}
	
	gpio_set_pin_low(Z_STEPPER_EN_PIN);
	return true;
}	

Bool move_crane(uint8_t to_posx, uint8_t to_posy)
{
	/*
	int d_posx = 6; //x position of E column
	if (to_posy != posy)
	{
		if (d_posx > posx)
		{
			move_x_stepper(d_posx - posx, CCW);
		}			
		else if (d_posx < posx)
		{
			move_x_stepper(posx - d_posx, CW);
		}			
	}
	*/
	int d_posx = posx;
	if (to_posx > d_posx)
	{
		move_x_stepper(to_posx - d_posx, CCW);
	}
	else if (to_posx < d_posx)
	{
		move_x_stepper(d_posx - to_posx, CW);
	}
	
	if (to_posy > posy)
	{
		move_y_stepper(to_posy - posy, CW);
	}
	else if (to_posy < posy)
	{
		move_y_stepper(posy- to_posy, CCW);
	}
	posy = to_posy;
	
	/*
	if (to_posx > posx)
	{
		move_x_stepper(to_posx - posx, CCW);
	}
	else
	{
		move_x_stepper(posx- to_posx, CW);
	}
	*/
	posx = to_posx;
		
	
	return true;
}


/**
 *  \brief centers the crane at the origin
 */
void center_at_origin()
{
	move_z_stepper(Up);
	
	int squares = 14;
	int direction = CCW;
	
	gpio_set_pin_high(Y_STEPPER_EN_PIN);
	
	gpio_set_pin_low(Y_STEPPER_CW_PIN);		
		
	// find the number of squares and go that many steps
	int num_steps = Y_STEPS_PER_SQUARE * squares;
	uint32_t i;
	for (i = 0; i < num_steps; i++)
	{
		//
		// POLL for sensor feedback from motor controller
		//
		if (gpio_pin_is_high(MOTOR_HOME_PIN_Y))
		{
			break;
		}
			
		gpio_set_pin_high(Y_STEPPER_CLK_PIN); //PIN1_bm;
		_delay_ms(18);
			
		gpio_set_pin_low(Y_STEPPER_CLK_PIN);
		_delay_ms(18);
	}
	
	gpio_set_pin_high(Y_STEPPER_CW_PIN);
	
	for (i = 0; i < Y_CALIBRATION_OFFSET; i++)
	{
			gpio_set_pin_high(Y_STEPPER_CLK_PIN); //PIN1_bm;
			_delay_ms(18);
			
			gpio_set_pin_low(Y_STEPPER_CLK_PIN);
			_delay_ms(18);
	}	
	
		
	gpio_set_pin_low(Y_STEPPER_EN_PIN);

	direction = CW;
	
	gpio_set_pin_high(X_STEPPER_EN_PIN);

	gpio_set_pin_high(X_STEPPER_CW_PIN);
	
	// find the number of squares and go that many steps
	num_steps = STEPS_PER_SQUARE * squares;
	
	for (i = 0; i < num_steps; i++)
	{
		//
		// POLL for sensor feedback from motor controller
		//
		if (gpio_pin_is_high(MOTOR_HOME_PIN_X))
		{
			break;
		}
		
		gpio_set_pin_high(X_STEPPER_CLK_PIN); //PIN1_bm;
		_delay_ms(6);
		
		gpio_set_pin_low(X_STEPPER_CLK_PIN);
		_delay_ms(6);
	}
	
	gpio_set_pin_low(X_STEPPER_CW_PIN);
		
	for (i = 0; i < X_CALIBRATION_OFFSET; i++)
	{
		gpio_set_pin_high(X_STEPPER_CLK_PIN); //PIN1_bm;
		_delay_ms(6);
		
		gpio_set_pin_low(X_STEPPER_CLK_PIN);
		_delay_ms(6);
	}
	
	gpio_set_pin_low(X_STEPPER_EN_PIN);
	
	posx = 0;
	posy = 0;
}