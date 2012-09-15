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
	current_pos = 0; // at A1
	
	// init servo
	// NOTE: this was (1 << 0) and ic hanged it incase servo stops working change that first
	PORTC.DIRSET = PIN0_bm; // config PORTC:0 as output to servo
	PORTC.DIRSET = PIN1_bm; // config PORTC:1 as clock output to stepper
	PORTC.DIRSET = PIN2_bm; // config PORTC:2 as CW/CCW pin for motor control.

	// note: pin 0 corresponds to channel A, pin 3 corresponds to channel D (CCA-CCD)

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
	int max = 65;
	TCC0.CCA= 49; // 654/20 = 33	
}


/**
 *  \brief Sets the clamp to closed position
 */
void close_clamp()
{
	// warning: this is kind of dangerous because it can update CCA in the middle of a pulse.
	// but I'm not too worried since the chance is 1/20 of it doing that and it will reach a steady state
	TCC0.CCA = 33;	//655/10
}


/**
 *  \brief Moves x stepper to specified location in the board 
 *  \note Control is on pin1 of port C
 *  \param steps number of steps to move
 *  \param direction 1 for clockwise, 0 for counter clockwise. 
 */
void move_x_stepper(uint8_t steps, uint8_t direction)
{
	
	if (direction)
	{
		PORTC.OUTSET = PIN2_bm;

	}
	else
	{
		PORTC.OUTCLR = PIN2_bm;
	}
	
	
	// find the number of squares and go that many steps
	int num_steps = STEPS_PER_SQUARE * steps;
	uint8_t i;
	for (i = 0; i < num_steps; i++)
	{
		//
		// POLL for sensor feedback from motor controller
		//
		
		PORTC.OUTSET = PIN1_bm; //PIN1_bm;
		_delay_ms(200);
		
		PORTC.OUTCLR = PIN1_bm;
		_delay_ms(200);
	}
}


/**
 *  \brief Moves y stepper to specified location in the board 
 *  \note Control is on pin3 of port C
 *  \param steps number of steps to move
 *  \param direction 1 for clockwise, 0 for counter clockwise. 
 */
void move_y_stepper(uint8_t steps, uint8_t direction)
{
	
	if (direction)
	{
		PORTC.OUTSET = PIN4_bm;
	}
	else
	{
		PORTC.OUTCLR = PIN4_bm;
	}
	
	
	// find the number of squares and go that many steps
	int num_steps = STEPS_PER_SQUARE * steps;
	uint8_t i;
	for (i = 0; i < num_steps; i++)
	{
		//
		// POLL for sensor feedback from motor controller
		//
		
		PORTC.OUTSET = PIN3_bm; //PIN1_bm;
		_delay_ms(30);
		
		PORTC.OUTCLR = PIN3_bm;
		_delay_ms(30);
	}
}


/**
 *  \brief Moves z stepper to specified location in the board
 *  \note Control is on pin4 of port C
 *  \param direction 0 means lower the crane and 1 means raise the crane
 */
void move_z_stepper()
{
	// find the number of squares and go that many steps
	uint8_t num_steps = Z_STEPPER_STEPS;
	uint8_t i;
	for (i = 0; i < num_steps; i++)
	{
		// 
		// POLL for sensor feedback from motor controller
		// 
		
		
		// step the motor
		PORTC.OUTSET = PIN3_bm;
		_delay_ms(10);
		PORTC.OUTCLR = PIN3_bm;
		_delay_ms(10);
	}
}	



/**
 *  \brief centers the crane at the origin
 */
void center_at_origin()
{
	move_z_stepper();
	//move_x_stepper(100, 0);
	move_y_stepper(100, 0);
	close_clamp();
}