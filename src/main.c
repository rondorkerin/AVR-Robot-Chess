/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
//#include <avr/io.h>
//#include <stdbool.h>
#include "chessengine.h"
#include "io.h"


/*
 * TEST MAIN: 
 * Issues a sequence of pawn movements and blinks an LED if a piece was taken. 
 * sequence: a2-a4, a4-a5, a5-a6
 *
 */ 

int main (void)
{
	board_init();

	io_init();
	
	debug("init game engine.");
			
	while (1){		
		
		init_engine();
		init_game(PlayerVsAI);

		int gameResult = 0, tookPieceFlag = 0,illegalMoveFlag = 0;
		char resultMove[4];
		
		char cmd[3][100] = {"a2a4", "a4a5", "a5a6"};
		int i;
		for (i = 0; i < 5; i++)
		{
			char* mv = get_player_move(1);
			player_move(cmd[i], &gameResult, &tookPieceFlag, &illegalMoveFlag);
			debug("player mv:");
			debug(cmd[i]);
			
			if (illegalMoveFlag)
			{
				debug("illegal move");
				continue;
			}
			ai_move(&gameResult, &tookPieceFlag, resultMove);
					
			if (!tookPieceFlag)
			{
				debug("ai mv:");
				debug(resultMove);
			}
			else
			{
				debug("AI took player piece");
				for (;;) {						
				}	
			}
		}


		
		
	}
	
	
}

