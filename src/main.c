/**
 * \file main.c
 *
 * \brief Main application module (controller in MVC context)
 *
 */

// define DEBUG if you want serial debugging on port D
// #define DEBUG

#include <asf.h>
#include "chessengine.h"
#include "io.h"

// test includes

#include "tests/enginetest.h"
#include "tests/iotest.h"

int main (void)
{
	board_init();
	
	io_test_init();
	
	x_stepper_test();
	//button_press_test(1);
	//clamp_servo_test();
	//pawn_rush_test();	
}

