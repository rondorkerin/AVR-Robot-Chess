#ifndef _CHESSENGINE
#define _CHESSENGINE

/***************************************************************************/
/*                               micro-Max,                                */
/* A chess program smaller than 2KB (of non-blank source), by H.G. Muller  */
/* Port to Atmel ATMega and AVR GCC, by Andre Adrian                       */
/***************************************************************************/

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