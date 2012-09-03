/**
 * \file chessengine.h
 *
 * \brief contains public interface to chess engine
 *
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 


#ifndef _CHESSENGINE
#define _CHESSENGINE

// Public functions
void ai_move(int* outGameResult, int* outTookPieceFlag, char* outMove);
char player_move(char* move, int* outGameResult, int* outTookPieceFlag, int* outIllegalMoveFlag);
void init_engine();
void init_game();

enum GameMode {
	PlayerVsAI = 0,
	AIVsAI,
	PlayerVsPlayer
};

enum GameResult {
	InPlay = 0,
	StaleMate,
	BlackMates,
	WhiteMates
};

#endif