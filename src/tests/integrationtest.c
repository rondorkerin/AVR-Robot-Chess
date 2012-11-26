/*
 * integrationtest.c
 *
 * Created: 9/14/2012 11:58:35 AM
 *  Author: nul4x3r0000
 */ 
/*
 * \file iotest.c
 *
 * \brief Behavioral test suite for I/O devices - includes those with their own modules such as motor driver
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 

#include "integrationtest.h"
#include "../chessengine.h"
#include "../io.h"
#include "../motordriver.h"
#include <conf.h>
#include <stdio.h>

/**
 *	\brief initializes IO devices and engine for test
 */
void integration_test_init()
{
	init_engine();
	init_io();	
}


/**
 *	\brief tests button press functionality
 *  \note requires the HMI button panel for player 1 and LCD screen hooked up to port D
 *  LCD pin: D3
 *  Button pressed pin: D0
 *  Button binary pins: LSB: D1, Bit 2: D2, MSB: D4
 */
void button_press_test(int side)
{
	while(1)
	{
		lcd_message("Please press any button to begin", side);
		int x = get_button_press(side);
		unsigned char c = (unsigned char)(x+48);
		lcd_message(&c, side);
		get_button_press(side);
	}
}


/**
 *	\brief tests the get move process
 */
void get_move_test(int side)
{
	_delay_ms(5000);
	while(1)
	{
		// wait for a button press
		char msg[80];
		sprintf(msg, "location: [%d %d]", posx, posy);
		lcd_message(msg, Player1);
		get_button_press(side);
		lcd_message("Please press any button to begin", side);
		get_button_press(side);	
		char* response = get_player_move(side);	
		lcd_message(response, side);
		get_button_press(Player1);
		uint8_t* move = get_positions(response);
		char whatigot[80];
		sprintf(whatigot, "[%d %d] ->[%d %d]", move[0], move[1], move[2], move[3]);
		lcd_message(whatigot, Player1);
		get_button_press(Player1);
		move_piece(move[0], move[1], move[2], move[3]);
		get_button_press(Player1);
		
	}
}	


/**
 *	\brief tests integration of chess engine with human interface without motors or LED grid/hall effect grid
 *  \note requires the HMI button panel for player 1 and LCD screen hooked up to port D
 *  LCD pin: D3
 *  Button pressed pin: D0
 *  Button binary pins: LSB: D1, Bit 2: D2, MSB: D4
 */
void game_play_no_motors_test(int side)
{
	//char response[50];
	while (1)
	{
		lcd_message("Press any button to begin a new game.", side);
		get_button_press(side);
		
		init_game(PlayerVsAI);

		while (1)
		{
			int gameResult = 0, tookPieceFlag = 0,illegalMoveFlag = 0;
			
			char* mv = get_player_move(side);
			player_move(mv, &gameResult, &tookPieceFlag, &illegalMoveFlag);
			
			if (illegalMoveFlag)
			{
				//sprintf(response, "Illegal move: [%s]", mv);
				lcd_message_delay("Illegal move.", side , 3000);
				continue;
			}
			
			char resultMove[5];
			ai_move(&gameResult, &tookPieceFlag, resultMove);
			
			//sprintf(response, "AI responds: [%s]", resultMove);
			lcd_message_delay(resultMove, side, 3000);
			
			if (tookPieceFlag)
			{
				lcd_message_delay("AI took player piece.", side, 3000);
			}
			if (gameResult == StaleMate)
			{
				lcd_message_delay("Stalemate!", side, 3000);
				break;
			}
			else if (gameResult == BlackMates)
			{
				lcd_message_delay("Black mates!", side, 3000);
				break;
			}
			else if (gameResult == WhiteMates)
			{
				lcd_message_delay("White mates!", side, 3000);
				break;
			}

			lcd_message("Press any button to continue.", side);
			get_button_press(side);
		}
	}		
}


void xyz_stepper_test()
{
	while(1)
	{
		/*
		center_at_origin();
		_delay_ms(2000);
		move_crane(5, 5);
		move_z_stepper(Down);
		move_z_stepper(Up);
		move_crane(7,3);
		move_z_stepper(Down);
		move_z_stepper(Up);
		*/
		
		
		lcd_message("Z stepper up", Player1);
		move_z_stepper(Up);
		lcd_message("Move X left", Player1);
		move_x_stepper(3, CW);
		lcd_message("Move y forward", Player1);
		move_y_stepper(3, CW);
		lcd_message("Open clamp", Player1);
		open_clamp();
		lcd_message("Move z stepper down", Player1);
		move_z_stepper(Down);
		lcd_message("Close clamp", Player1);
		close_clamp();
		lcd_message("Move z stepper up", Player1);
		move_z_stepper(Up);
	
		lcd_message("Move z stepper down", Player1);
		move_z_stepper(Down);
		lcd_message("Z stepper up", Player1);
		move_z_stepper(Up);
		lcd_message("Move x stepper right", Player1);
		move_x_stepper(3, CCW);
		lcd_message("Move y stepper back", Player1);
		move_y_stepper(3, CCW);
		lcd_message("Move z stepper down", Player1);
		move_z_stepper(Down);
		lcd_message("Move z stepper up", Player1);
		move_z_stepper(Up);
		
	}
}


void move_piece_test()
{
	while(1)
	{
		//lcd_message("Calibrate motors now and press a button", Player1);
		//get_button_press(Player1);
		lcd_message("(30->63)", Player1);
		center_at_origin();
		move_piece(3, 0, 6, 3);
		delay_ms(3000);
		lcd_message("(63->65)", Player1);
		move_piece(6, 3, 6, 5);
		delay_ms(3000);
		lcd_message("(65->30)", Player1);
		move_piece(6, 5, 3, 0);
		delay_ms(3000);
		
	}
}			
		/*
	lcd_message("Open clamp", Player1);
	open_clamp();
	lcd_message("Z stepper down", Player1);
	move_z_stepper(Down);
	lcd_message("Close clamp", Player1);
	close_clamp();
	lcd_message("Z stepper up", Player1);
	move_z_stepper(Up);
	lcd_message("Move X left", Player1);
	move_x_stepper(3, CW);
	lcd_message("Move y forward", Player1);
	move_y_stepper(3, CW);
	lcd_message("Move z stepper down", Player1);
	move_z_stepper(Down);
	lcd_message("Open clamp", Player1);
	open_clamp();
	lcd_message("Move z stepper up", Player1);
	move_z_stepper(Up);
	
	lcd_message("Move z stepper down", Player1);
	move_z_stepper(Down);
	lcd_message("Close clamp", Player1);
	close_clamp();
	lcd_message("Z stepper up", Player1);
	move_z_stepper(Up);
	lcd_message("Move x stepper right", Player1);
	move_x_stepper(3, CCW);
	lcd_message("Move y stepper back", Player1);
	move_y_stepper(3, CCW);
	lcd_message("Move z stepper down", Player1);
	move_z_stepper(Down);
	lcd_message("Open clamp", Player1);
	open_clamp();
	lcd_message("Move z stepper up", Player1);
	move_z_stepper(Up);
		*/	


/**
 *	\brief tests integration of chess engine with human interface without motors or LED grid/hall effect grid
 *  \note requires the HMI button panel for player 1 and LCD screen hooked up to port D
 *  LCD pin: D3
 *  Button pressed pin: D0
 *  Button binary pins: LSB: D1, Bit 2: D2, MSB: D4
 */
void game_play_test(int side)
{
	delay_ms(10);
	blink_led(8, 1, Red);
	blink_led(8, 8, Red);
	// create the checkered board
	refresh_led_grid();
	
	//char response[50];
	while (1)
	{
		center_at_origin();
		lcd_message("Press any button to begin a new game.", side);
		get_button_press(side);
		
		lcd_message("Prepare to lose,    mortal!", Player2);
		lcd_message("You think YOU can   defeat ME?", Player2);
		
		init_game(PlayerVsAI);

		while (1)
		{			
			int gameResult = 0, tookPieceFlag = 0,illegalMoveFlag = 0;
			
			char* mv = get_player_move(side);
			double msg = (double)rand() / (double)RAND_MAX;
			if (msg < .3)
			{
				lcd_message("You're new at this, aren't you?", Player2);
			}
			else if (msg < .6)
			{
				lcd_message("You're going to     regret that...", Player2);
			}
			else if (msg < .9)
			{
				lcd_message("I can't let you do  that...", Player2);
			}
			else
			{
				lcd_message("Learn 2 play,       N00b!", Player2);
			}
			
			player_move(mv, &gameResult, &tookPieceFlag, &illegalMoveFlag);
			
			if (illegalMoveFlag)
			{
				//sprintf(response, "Illegal move: [%s]", mv);
				lcd_message_delay("Illegal move.", side , 3000);
				continue;
			}
			
			uint8_t* positions = get_positions(mv);
			
			get_button_press(Player1);
			
			free(mv);
			if (tookPieceFlag)
			{
				lcd_message("Player takes AI     piece", side);
				move_piece(positions[2], positions[3], GRAVEYARD_XPOS, positions[3]);				
			}
			
			lcd_message("Moving player       piece...", side);
			move_piece(positions[0], positions[1], positions[2], positions[3]);
		
			free(positions);
			
			char resultMove[5];
			ai_move(&gameResult, &tookPieceFlag, resultMove);
			
			//sprintf(response, "AI responds: [%s]", resultMove);
			lcd_message_delay(resultMove, side, 3000);
			
			positions = get_positions(resultMove);
			
			blink_led(resultMove[1] - '1' + 1,resultMove[0]-'a' + 1, Red);
			blink_led(resultMove[3] - '1' + 1,resultMove[2]-'a' + 1, Red);
			
			
			if (tookPieceFlag)
			{
				lcd_message("AI takes player     piece.", side);
				lcd_message("Got one! Muahhahaha!", Player2);
				move_piece(positions[2], positions[3], GRAVEYARD_XPOS, positions[3]);
			}
			
			lcd_message("Moving AI piece...", side);
			lcd_message(resultMove, Player2);
			move_piece(positions[0], positions[1], positions[2], positions[3]);
			
			if (gameResult == StaleMate)
			{
				lcd_message_delay("Stalemate!", side, 3000);
				break;
			}
			else if (gameResult == BlackMates)
			{
				lcd_message_delay("Black mates!", side, 3000);
				break;
			}
			else if (gameResult == WhiteMates)
			{
				lcd_message_delay("White mates!", side, 3000);
				break;
			}

			lcd_message("Press any button to continue.", side);
			get_button_press(side);
		}
	}		
}
