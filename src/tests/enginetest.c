/*
 * \file enginetest.c
 *
 * \brief Behavioral test for game engine
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 


#include "enginetest.h"
#include "test.h"
#include "../chessengine.h"
#include "../io.h"

void pawn_rush_test()
{
	init_io();
	
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

