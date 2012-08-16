#ifndef _CHESSENGINE
#define _CHESSENGINE

/***************************************************************************/
/*                               micro-Max,                                */
/* A chess program smaller than 2KB (of non-blank source), by H.G. Muller  */
/* Port to Atmel ATMega and AVR GCC, by Andre Adrian                       */
/***************************************************************************/

// Public functions
void AIMove(int* outGameResult, int* outTookPieceFlag, char* outMove);
char PlayerMove(char* move, int* outGameResult, int* outTookPieceFlag, int* outIllegalMoveFlag);
void NewGame();
void InitEngine();
void InitGame();

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