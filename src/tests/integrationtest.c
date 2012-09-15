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
#include "test.h"
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
	_delay_ms(5000);
	while(1)
	{
		lcd_message("Please press any button to begin", side);
		int x = get_button_press(side);
		unsigned char c = (unsigned char)(x+48);
		lcd_message(&c, side);
	}
}


/**
 *	\brief tests the get move process
 *  \note requires the HMI button panel for player 1 and LCD screen hooked up to port D
 *  LCD pin: D3
 *  Button pressed pin: D0
 *  Button binary pins: LSB: D1, Bit 2: D2, MSB: D4
 */
void get_move_test(int side)
{
	_delay_ms(5000);
	while(1)
	{
		// wait for a button press
		lcd_message("Please press any button to begin", side);
		get_button_press(side);	
		char* response = get_player_move(side);	
		lcd_message(response, side);
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

