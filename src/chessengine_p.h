#ifndef CHESSENGINE_P
#define CHESSENGINE_P

#define EMPTY 0
#define WHITE 8
#define BLACK 16
#define MAXDEPTH 1

/* make unique integer from engine move representation */
#define PACK_MOVE 256*K + L;

/* convert intger argument back to engine move representation */
#define UNPACK_MOVE(A) K = (A)>>8 & 255; L = (A) & 255;



// private functions
/* (q,l)=window, e=current eval. score
 * E=e.p. sqr.z=prev.dest, n=depth; return score
 * Recursive minimax search, k=moving side
 */
short minimax(unsigned char k,short q,short l,short e,unsigned char E,unsigned char z,unsigned char n); 
int game_status_result();
int get_piece_count();
void mysrand(unsigned short r_);
unsigned short myrand(void);

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

int Computer, MaxTime, MaxMoves, TimeInc, sec, i;
char line[256], command[256];
int m, nr;

#endif