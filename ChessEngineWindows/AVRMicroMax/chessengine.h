#ifndef _CHESSENGINE
#define _CHESSENGINE

/***************************************************************************/
/*                               micro-Max,                                */
/* A chess program smaller than 2KB (of non-blank source), by H.G. Muller  */
/* Port to Atmel ATMega and AVR GCC, by Andre Adrian                       */
/***************************************************************************/
/* version 4.8 (~1900 characters) features:                                */
/* - recursive negamax search                                              */
/* - all-capture quiescence search with MVV/LVA priority                   */
/* - (internal) iterative deepening                                        */
/* - best-move-first 'sorting'                                             */
/* - a hash table storing score and best move                              */
/* - futility pruning                                                      */
/* - king safety through magnetic frozen king                              */
/* - null-move pruning                                                     */
/* - Late-move reductions                                                  */
/* - full FIDE rules (expt minor promotion) and move-legality checking     */
/* - keep hash + rep-draw detect                                           */
/* - end-game Pawn-push bonus, new piece values, gradual promotion         */

/* Rehash sacrificed, simpler retrieval. Some characters squeezed out.     */
/* No hash-table clear, single-call move-legality checking based on K==I   */

/* fused to generic Winboard driver */

/* 26nov2008 no hash table */
/* 29nov2008 pseudo random generator  */

int StartKey;

#define EMPTY 0
#define WHITE 8
#define BLACK 16

#define STATE 64

/* make unique integer from engine move representation */
#define PACK_MOVE 256*K + L;

/* convert intger argument back to engine move representation */
#define UNPACK_MOVE(A) K = (A)>>8 & 255; L = (A) & 255;

enum GameMode {
	PlayerVsAI = 0,
	AIVsAI,
	PlayerVsPlayer
};

// Public functions
void InitEngine();
void InitGame(int gameMode);
void Think();
void DoMove();
void ReadMove();
int Legal();
void ClearBoard();
void PutPiece();

// private functions
/* (q,l)=window, e=current eval. score
 * E=e.p. sqr.z=prev.dest, n=depth; return score
 * Recursive minimax search, k=moving side
 */
short D(unsigned char k,short q,short l,short e,unsigned char E,unsigned char z,unsigned char n); 
void CopyBoard(int s);
void PrintMove();

/* Global variables visible to engine. Normally they */
/* would be replaced by the names under which these  */
/* are known to your engine, so that they can be     */
/* manipulated directly by the interface.            */

int Side;
int Move;
int PromPiece;
int Result;
int TimeLeft;
int MovesLeft;
int MaxDepth;
int Post;
int Fifty;
int UnderProm;

int Ticks, tlim;

int GameHistory[1024];
char HistoryBoards[1024][STATE];
int GamePtr, HistPtr;

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

/* 16bit pseudo random generator by Andre Adrian */
#define MYRAND_MAX 65535

unsigned short r = 1;                     /* pseudo random generator seed */

short _gameMode = 0;

void mysrand(unsigned short r_) {
 r = r_;
}

unsigned short myrand(void) {
 return r=((r<<11)+(r<<7)+r)>>1;
}

#endif