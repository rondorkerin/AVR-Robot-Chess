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

int StartKey;

#define EMPTY 0
#define WHITE 8
#define BLACK 16
#define MAXDEPTH 1

/* make unique integer from engine move representation */
#define PACK_MOVE 256*K + L;

/* convert intger argument back to engine move representation */
#define UNPACK_MOVE(A) K = (A)>>8 & 255; L = (A) & 255;

enum GameMode{
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

// private functions
/* (q,l)=window, e=current eval. score
 * E=e.p. sqr.z=prev.dest, n=depth; return score
 * Recursive minimax search, k=moving side
 */
short D(unsigned char k,short q,short l,short e,unsigned char E,unsigned char z,unsigned char n); 
void CopyBoard(int s);
int GameStatusResult();
int GetPieceCount();

/* Global variables visible to engine. Normally they */
/* would be replaced by the names under which these  */
/* are known to your engine, so that they can be     */
/* manipulated directly by the interface.            */

int Side;
int Move;
int PromPiece;

#define W while
#define M 0x88
#define S 128
#define I 8000

long N, T;                  /* N=evaluated positions+S, T=recursion limit */
short Q,O,K,R;

char L,
w[]={0,2,2,7,-1,8,12,23},                      /* relative piece values    */
o[]={-16,-15,-17,0,1,16,0,1,16,15,17,0,14,18,31,33,0, /* step-vector lists */
     7,-1,11,6,8,3,6,                          /* 1st dir. in o[] per piece*/
     6,3,5,7,4,5,3,6},                         /* initial piece setup      */
b[129],                                /* board, center piece table, dummy */

n[]=".?inkbrq?I?NKBRQ",            /* piece symbols on printout, Pawn is i */
Z;                                                  /* Z=recursion counter */

unsigned short r = 1;                     /* pseudo random generator seed */

void mysrand(unsigned short r_) {
 r = r_;
}

unsigned short myrand(void) {
 return r=((r<<11)+(r<<7)+r)>>1;
}

int Computer, MaxTime, MaxMoves, TimeInc, sec, i;
char line[256], command[256];
int m, nr;

#endif