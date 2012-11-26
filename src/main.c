/**
 * \file main.c
 *
 * \brief Main application module (controller in MVC context)
 *
 */

// define DEBUG if you want serial debugging on port D
//#define DEBUG 1

#include <asf.h>
#include "chessengine.h"
#include "io.h"

// test includes

#include "tests/enginetest.h"
#include "tests/iotest.h"
#include "tests/integrationtest.h"

int main (void)
{	
	//board_init();
	
	//*************INTEGRATION TESTS*******
	integration_test_init();
	//button_press_test(Player1);
	//get_move_test(Player1);
	//game_play_no_motors_test(Player1);
	//xy_stepper_test();
	//xyz_stepper_test();
	//move_piece_test();
	game_play_test(Player1);
	//move_piece_test();
	

	//*********IO TESTS *******************
	//led_test();
	//x_stepper_test();
	//serial_debug_test();
	//x_stepper_test();
	//y_stepper_test();
	//lcd_test();
	//button_press_test(1);
	//z_stepper_test();
	//clamp_servo_test();
	//pawn_rush_test();	
}

